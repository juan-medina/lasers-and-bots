/****************************************************************************
 Copyright (c) 2019 Juan Medina

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "SliderObject.h"

SliderObject::SliderObject()
  : _progress(nullptr)
  , _label(nullptr)
  , _enabled(true)
  , _touchListener(nullptr)
  , _touchBeginAt(Vec2::ZERO)
  , _percentBeginTouch(0)
  , _arrow(nullptr)
  , _lastPercentage(-1)
  , _callback(nullptr)
{
}

SliderObject* SliderObject::create(const std::string& background, const std::string& progress,
                                   const SliderChangeCallback& callback)
{
  SliderObject* ret = nullptr;

  do
  {
    auto object = new SliderObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(background, progress, callback))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  } while (false);

  return ret;
}

bool SliderObject::init(const std::string& background, const std::string& progress,
                        const SliderChangeCallback& callback)
{
  auto ret = false;

  do
  {
    _callback = callback;
    const auto normalSprite = Sprite::createWithSpriteFrameName(background);
    UTILS_BREAK_IF(normalSprite == nullptr);

    UTILS_BREAK_IF(!BaseClass::initWithNormalSprite(normalSprite, nullptr, nullptr, nullptr));

    const auto progressSprite = Sprite::createWithSpriteFrameName(progress);
    UTILS_BREAK_IF(progressSprite == nullptr);

    _progress = ProgressTimer::create(progressSprite);
    UTILS_BREAK_IF(_progress == nullptr);

    _progress->setType(ProgressTimer::Type::BAR);
    _progress->setMidpoint(Vec2(0, 0));
    _progress->setBarChangeRate(Vec2(1, 0));

    _progress->setPosition(getContentSize().width / 2, getContentSize().height / 2);

    _progress->setPosition(_progress->getPosition() + Vec2(10, 15));

    addChild(_progress);

    _label = Label::createWithTTF("100 %", "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(_label == nullptr);

    _label->setAnchorPoint(Vec2(1.f, .5f));
    _label->setHorizontalAlignment(TextHAlignment::RIGHT);
    _label->setTextColor(Color4B::WHITE);
    _label->enableOutline(Color4B(0, 0, 0, 255), 5);

    const auto labelPos = Vec2(getContentSize().width + 400,
                               (_label->getContentSize().height / 2) + (getContentSize().height / 4));
    _label->setPosition(labelPos);

    addChild(_label);

    _arrow = Sprite::createWithSpriteFrameName("16_arrow.png");
    UTILS_BREAK_IF(_arrow == nullptr);

    _arrow->setPosition(0.f, _progress->getContentSize().height);
    _arrow->setOpacity(190);
    _progress->addChild(_arrow);

    setCascadeOpacityEnabled(true);
    setCascadeColorEnabled(true);

    ret = true;
  } while (false);

  return ret;
}

void SliderObject::setPercentage(const float percentage)
{
  if (percentage != _lastPercentage)
  {
    _lastPercentage = percentage;
    onPercentageChange(percentage);
  }
}

float SliderObject::getPercentage() const
{
  return _progress->getPercentage();
}

void SliderObject::enable(const bool enabled)
{
  _enabled = enabled;
  const auto opacity = static_cast<GLubyte>(enabled ? 255 : 127);
  setOpacity(opacity);
  _progress->setOpacity(opacity);
  setEnabled(enabled);
  enableTouch(enabled);
}

void SliderObject::setColor(const Color3B& color)
{
  BaseClass::setColor(color);
  _arrow->setColor(color);
  _progress->setColor(color);
}

bool SliderObject::enableTouch(const bool enabled)
{
  auto ret = false;

  do
  {
    if (enabled)
    {
      if (_touchListener == nullptr)
      {
        _touchListener = EventListenerTouchOneByOne::create();
        UTILS_BREAK_IF(_touchListener == nullptr);

        _touchListener->onTouchBegan = CC_CALLBACK_2(SliderObject::onTouchBegan, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(SliderObject::onTouchEnded, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(SliderObject::onTouchMoved, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(SliderObject::onTouchCancel, this);

        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
      }
    }
    else
    {
      if (_touchListener != nullptr)
      {
        _eventDispatcher->removeEventListener(_touchListener);
        _touchListener = nullptr;
      }
    }
    ret = true;
  } while (false);

  return ret;
}

bool SliderObject::onTouchBegan(Touch* touch, Event* unusedEvent)
{
  const auto location = touch->getLocation();
  const auto touchProgress = isTouchedByLocation(_progress, touch->getLocation());
  if (touchProgress)
  {
    _touchBeginAt = location;
    const auto point = _progress->convertToNodeSpace(location);
    auto percent = static_cast<int>((point.x / _progress->getContentSize().width) * 100);
    if (percent < 0)
    {
      percent = 0;
    }
    else if (percent > 100)
    {
      percent = 100;
    }
    setPercentage(percent);
    _percentBeginTouch = percent;
  }
  else
  {
    _touchBeginAt = Vec2::ZERO;
  }
  return touchProgress;
}

void SliderObject::onTouchMoved(Touch* touch, Event* unusedEvent)
{
  updateLocation(touch->getLocation());
}

void SliderObject::onTouchEnded(Touch* touch, Event* unusedEvent)
{
  updateLocation(touch->getLocation());
  _touchBeginAt = Vec2::ZERO;
}

void SliderObject::onTouchCancel(Touch* touch, Event* unusedEvent)
{
  _touchBeginAt = Vec2::ZERO;
}

bool SliderObject::isTouchedByLocation(Node* node, const Vec2& location) const
{
  const auto camera = Camera::getVisitingCamera();
  if (camera != nullptr)
  {
    Rect rect;
    rect.size = node->getContentSize();
    const auto gap = node->getContentSize().height * 3;
    rect.size.height += gap;
    rect.size.width += gap;
    rect.origin.x -= (gap / 2);
    rect.origin.y -= (gap / 2);
    return isScreenPointInRect(location, camera, node->getWorldToNodeTransform(), rect, nullptr);
  }

  return false;
}

void SliderObject::updateLocation(const Vec2& location)
{
  if (_touchBeginAt != Vec2::ZERO)
  {
    const auto distance = -(_touchBeginAt.x - location.x);
    auto percent =
      _percentBeginTouch + static_cast<int>((distance / _progress->getContentSize().width) * 100);
    if (percent < 0)
    {
      percent = 0;
    }
    else if (percent > 100)
    {
      percent = 100;
    }
    setPercentage(percent);
  }
}

void SliderObject::onPercentageChange(const float percentage) const
{
  _progress->setPercentage(percentage);
  _label->setString(StringFormat("%3d %%", static_cast<int>(percentage)));
  _arrow->setPosition(_progress->getContentSize().width * (percentage / 100),
                      _progress->getContentSize().height);
  if (_callback != nullptr)
  {
    _callback(percentage);
  }
}
