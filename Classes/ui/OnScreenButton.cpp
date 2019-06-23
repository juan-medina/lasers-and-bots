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

#include "OnScreenButton.h"

OnScreenButton::OnScreenButton()
  : _normalSprite(nullptr)
  , _pushedSprite(nullptr)
  , _disabledSprite(nullptr)
  , _labelButton(nullptr)
  , _pushed(false)
  , _disabled(false)
  , _savedTouchId(-1)
  , _touchListener(nullptr)
  , _type(ButtonType::None)
{
}

OnScreenButton* OnScreenButton::create(const ButtonType& type, const std::string& spriteFrameName,
                                       const std::string& label /* = ""*/)
{
  OnScreenButton* ret = nullptr;

  do
  {
    auto object = new OnScreenButton();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(type, spriteFrameName, label))
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

bool OnScreenButton::init(const ButtonType& type, const std::string& spriteFrameName,
                          const std::string& label)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init());

    _normalSprite = Sprite::createWithSpriteFrameName(spriteFrameName + "_01.png");
    UTILS_BREAK_IF(_normalSprite == nullptr);

    _normalSprite->setOpacity(90);

    addChild(_normalSprite);

    _pushedSprite = Sprite::createWithSpriteFrameName(spriteFrameName + "_02.png");
    UTILS_BREAK_IF(_pushedSprite == nullptr);

    addChild(_pushedSprite);

    _disabledSprite = Sprite::createWithSpriteFrameName(spriteFrameName + "_04.png");
    UTILS_BREAK_IF(_disabledSprite == nullptr);

    _disabledSprite->setOpacity(127);

    addChild(_disabledSprite);

    if (!label.empty())
    {
      _labelButton = Label::createWithTTF(label, "fonts/tahoma.ttf", 120);
      UTILS_BREAK_IF(_labelButton == nullptr);

      _labelButton->enableOutline(Color4B(0, 0, 0, 255), 5);

      addChild(_labelButton);
    }

    pushed(false);
    disabled(false);

    _type = type;

    ret = true;
  } while (false);

  return ret;
}

const cocos2d::Size& OnScreenButton::getContentSize() const
{
  return _normalSprite->getContentSize();
}

void OnScreenButton::disabled(const bool disabled)
{
  _disabled = disabled;
  enableTouch(!disabled);
  onStatusChange();
}

void OnScreenButton::onStatusChange() const
{
  _normalSprite->setVisible(!(_pushed || _disabled));
  _pushedSprite->setVisible(_pushed);
  _disabledSprite->setVisible(_disabled);

  if (_labelButton != nullptr)
  {
    _labelButton->setOpacity(_pushed ? 255 : 127);
  }
}

bool OnScreenButton::isTouchedByLocation(const Vec2& location) const
{
  const auto touchLocation = location + Vec2(getContentSize().width / 2, getContentSize().height / 2);

  const auto camera = Camera::getVisitingCamera();
  if (camera != nullptr)
  {
    Rect rect;
    rect.size = getContentSize() * 2;
    rect.origin = rect.origin - (rect.size / 4);

    return isScreenPointInRect(touchLocation, camera, getWorldToNodeTransform(), rect, nullptr);
  }

  return false;
}

void OnScreenButton::pushed(const bool pushed)
{
  _pushed = pushed;
  onStatusChange();
}

bool OnScreenButton::enableTouch(const bool enabled)
{
  auto ret = false;

  do
  {
    if (enabled)
    {
      if (_touchListener == nullptr)
      {
        _touchListener = EventListenerTouchAllAtOnce::create();
        UTILS_BREAK_IF(_touchListener == nullptr);

        _touchListener->onTouchesBegan = CC_CALLBACK_2(OnScreenButton::onTouchesBegan, this);
        _touchListener->onTouchesEnded = CC_CALLBACK_2(OnScreenButton::onTouchesEnded, this);
        _touchListener->onTouchesMoved = CC_CALLBACK_2(OnScreenButton::onTouchesMoved, this);
        _touchListener->onTouchesCancelled = CC_CALLBACK_2(OnScreenButton::onTouchesCancel, this);

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

void OnScreenButton::onTouchesBegan(const std::vector<Touch*>& touches, Event* unusedEvent)
{
  if (_pushed)
  {
    return;
  }

  for (const auto touch : touches)
  {
    const auto location = touch->getLocation();

    if (isTouchedByLocation(location))
    {
      _savedTouchId = touch->getID();
      pushed(true);
      return;
    }
  }
}

void OnScreenButton::onTouchesMoved(const std::vector<Touch*>& touches, Event* unusedEvent)
{
  for (const auto touch : touches)
  {
    if (touch->getID() == _savedTouchId)
    {
      const auto location = touch->getLocation();

      if (isTouchedByLocation(location))
      {
        pushed(true);
        return;
      }
      else
      {
        pushed(false);
        return;
      }
    }
  }
}

void OnScreenButton::onTouchesEnded(const std::vector<Touch*>& touches, Event* unusedEvent)
{
  for (const auto touch : touches)
  {
    if (touch->getID() == _savedTouchId)
    {
      pushed(false);
      return;
    }
  }
}

void OnScreenButton::onTouchesCancel(const std::vector<Touch*>& touches, Event* unusedEvent)
{
  onTouchesEnded(touches, unusedEvent);
}
