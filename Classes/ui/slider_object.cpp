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

#include "slider_object.h"

slider_object::slider_object()
  : progress_(nullptr)
  , label_(nullptr)
  , enabled_(true)
  , touch_listener_(nullptr)
  , touch_begin_at_(Vec2::ZERO)
  , percent_begin_touch_(0)
  , arrow_(nullptr)
  , last_percentage_(-1)
  , callback_(nullptr)
{
}

slider_object* slider_object::create(const std::string& background, const std::string& progress,
                                     const slider_change_callback& callback)
{
  slider_object* ret = nullptr;

  do
  {
    auto object = new slider_object();
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

bool slider_object::init(const std::string& background, const std::string& progress,
                         const slider_change_callback& callback)
{
  auto ret = false;

  do
  {
    callback_ = callback;
    const auto normal_sprite = Sprite::createWithSpriteFrameName(background);
    UTILS_BREAK_IF(normal_sprite == nullptr);

    UTILS_BREAK_IF(!base_class::initWithNormalSprite(normal_sprite, nullptr, nullptr, nullptr));

    const auto progress_sprite = Sprite::createWithSpriteFrameName(progress);
    UTILS_BREAK_IF(progress_sprite == nullptr);

    progress_ = ProgressTimer::create(progress_sprite);
    UTILS_BREAK_IF(progress_ == nullptr);

    progress_->setType(ProgressTimer::Type::BAR);
    progress_->setMidpoint(Vec2(0, 0));
    progress_->setBarChangeRate(Vec2(1, 0));

    progress_->setPosition(getContentSize().width / 2, getContentSize().height / 2);

    progress_->setPosition(progress_->getPosition() + Vec2(10, 15));

    addChild(progress_);

    label_ = Label::createWithTTF("100 %", "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label_ == nullptr);

    label_->setAnchorPoint(Vec2(1.f, .5f));
    label_->setHorizontalAlignment(TextHAlignment::RIGHT);
    label_->setTextColor(Color4B::WHITE);
    label_->enableOutline(Color4B(0, 0, 0, 255), 5);

    const auto label_pos = Vec2(getContentSize().width + 400,
                                (label_->getContentSize().height / 2) + (getContentSize().height / 4));
    label_->setPosition(label_pos);

    addChild(label_);

    arrow_ = Sprite::createWithSpriteFrameName("16_arrow.png");
    UTILS_BREAK_IF(arrow_ == nullptr);

    arrow_->setPosition(0.f, progress_->getContentSize().height);
    arrow_->setOpacity(190);
    progress_->addChild(arrow_);

    setCascadeOpacityEnabled(true);
    setCascadeColorEnabled(true);

    ret = true;
  } while (false);

  return ret;
}

void slider_object::set_percentage(const float percentage)
{
  if (percentage != last_percentage_)
  {
    last_percentage_ = percentage;
    on_percentage_change(percentage);
  }
}

float slider_object::get_percentage() const
{
  return progress_->getPercentage();
}

void slider_object::enable(const bool enabled)
{
  enabled_ = enabled;
  const auto opacity = static_cast<GLubyte>(enabled ? 255 : 127);
  setOpacity(opacity);
  progress_->setOpacity(opacity);
  setEnabled(enabled);
  enable_touch(enabled);
}

void slider_object::setColor(const Color3B& color)
{
  base_class::setColor(color);
  arrow_->setColor(color);
  progress_->setColor(color);
}

bool slider_object::enable_touch(const bool enabled)
{
  auto ret = false;

  do
  {
    if (enabled)
    {
      if (touch_listener_ == nullptr)
      {
        touch_listener_ = EventListenerTouchOneByOne::create();
        UTILS_BREAK_IF(touch_listener_ == nullptr);

        touch_listener_->onTouchBegan = CC_CALLBACK_2(slider_object::on_touch_began, this);
        touch_listener_->onTouchEnded = CC_CALLBACK_2(slider_object::on_touch_ended, this);
        touch_listener_->onTouchMoved = CC_CALLBACK_2(slider_object::on_touch_moved, this);
        touch_listener_->onTouchCancelled = CC_CALLBACK_2(slider_object::on_touch_cancel, this);

        _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener_, this);
      }
    }
    else
    {
      if (touch_listener_ != nullptr)
      {
        _eventDispatcher->removeEventListener(touch_listener_);
        touch_listener_ = nullptr;
      }
    }
    ret = true;
  } while (false);

  return ret;
}

bool slider_object::on_touch_began(Touch* touch, Event* unused_event)
{
  const auto location = touch->getLocation();
  const auto touch_progress = is_touched_by_location(progress_, touch->getLocation());
  if (touch_progress)
  {
    touch_begin_at_ = location;
    const auto point = progress_->convertToNodeSpace(location);
    auto percent = static_cast<int>((point.x / progress_->getContentSize().width) * 100);
    if (percent < 0)
    {
      percent = 0;
    }
    else if (percent > 100)
    {
      percent = 100;
    }
    set_percentage(percent);
    percent_begin_touch_ = percent;
  }
  else
  {
    touch_begin_at_ = Vec2::ZERO;
  }
  return touch_progress;
}

void slider_object::on_touch_moved(Touch* touch, Event* unused_event)
{
  update_location(touch->getLocation());
}

void slider_object::on_touch_ended(Touch* touch, Event* unused_event)
{
  update_location(touch->getLocation());
  touch_begin_at_ = Vec2::ZERO;
}

void slider_object::on_touch_cancel(Touch* touch, Event* unused_event)
{
  touch_begin_at_ = Vec2::ZERO;
}

bool slider_object::is_touched_by_location(Node* node, const Vec2& location) const
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

void slider_object::update_location(const Vec2& location)
{
  if (touch_begin_at_ != Vec2::ZERO)
  {
    const auto distance = -(touch_begin_at_.x - location.x);
    auto percent =
      percent_begin_touch_ + static_cast<int>((distance / progress_->getContentSize().width) * 100);
    if (percent < 0)
    {
      percent = 0;
    }
    else if (percent > 100)
    {
      percent = 100;
    }
    set_percentage(percent);
  }
}

void slider_object::on_percentage_change(const float percentage) const
{
  progress_->setPercentage(percentage);
  label_->setString(string_format("%3d %%", static_cast<int>(percentage)));
  arrow_->setPosition(progress_->getContentSize().width * (percentage / 100),
                      progress_->getContentSize().height);
  if (callback_ != nullptr)
  {
    callback_(percentage);
  }
}
