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

#include "on_screen_button.h"

on_screen_button::on_screen_button():
  normal_sprite_(nullptr),
  pushed_sprite_(nullptr),
  disabled_sprite_(nullptr),
  label_button_(nullptr),
  pushed_(false),
  disabled_(false),
  saved_touch_id_(-1),
  touch_listener_(nullptr),
  type_(button_type::none)
{
}

on_screen_button* on_screen_button::create(const button_type& type, const std::string& sprite_frame_name,
                                           const std::string& label/* = ""*/)
{
  on_screen_button* ret = nullptr;

  do
  {
    auto object = new on_screen_button();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(type, sprite_frame_name, label))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  }
  while (false);

  return ret;
}

bool on_screen_button::init(const button_type& type, const std::string& sprite_frame_name, const std::string& label)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init());

    normal_sprite_ = Sprite::createWithSpriteFrameName(sprite_frame_name + "_01.png");
    UTILS_BREAK_IF(normal_sprite_==nullptr);

    normal_sprite_->setOpacity(90);

    addChild(normal_sprite_);

    pushed_sprite_ = Sprite::createWithSpriteFrameName(sprite_frame_name + "_02.png");
    UTILS_BREAK_IF(pushed_sprite_ == nullptr);

    addChild(pushed_sprite_);

    disabled_sprite_ = Sprite::createWithSpriteFrameName(sprite_frame_name + "_04.png");
    UTILS_BREAK_IF(disabled_sprite_ == nullptr);

    disabled_sprite_->setOpacity(127);

    addChild(disabled_sprite_);

    if (!label.empty())
    {
      label_button_ = Label::createWithTTF(label, "fonts/tahoma.ttf", 120);
      UTILS_BREAK_IF(label_button_ == nullptr);

      label_button_->enableOutline(Color4B(0, 0, 0, 255), 5);

      addChild(label_button_);
    }

    pushed(false);
    disabled(false);

    type_ = type;

    ret = true;
  }
  while (false);

  return ret;
}

const cocos2d::Size& on_screen_button::getContentSize() const
{
  return normal_sprite_->getContentSize();
}

void on_screen_button::disabled(const bool disabled)
{
  disabled_ = disabled;
  enable_touch(!disabled);
  on_status_change();
}

void on_screen_button::on_status_change() const
{
  normal_sprite_->setVisible(!(pushed_ || disabled_));
  pushed_sprite_->setVisible(pushed_);
  disabled_sprite_->setVisible(disabled_);

  if (label_button_ != nullptr)
  {
    label_button_->setOpacity(pushed_ ? 255 : 127);
  }
}

bool on_screen_button::is_touched_by_location(const Vec2& location) const
{
  const auto touch_location = location + Vec2(getContentSize().width / 2,
                                              getContentSize().height / 2);

  const auto camera = Camera::getVisitingCamera();
  if (camera != nullptr)
  {
    Rect rect;
    rect.size = getContentSize() * 2;
    rect.origin = rect.origin - (rect.size / 4);

    return isScreenPointInRect(touch_location, camera, getWorldToNodeTransform(), rect, nullptr);
  }

  return false;
}

void on_screen_button::pushed(const bool pushed)
{
  pushed_ = pushed;
  on_status_change();
}

bool on_screen_button::enable_touch(const bool enabled)
{
  auto ret = false;

  do
  {
    if (enabled)
    {
      if (touch_listener_ == nullptr)
      {
        touch_listener_ = EventListenerTouchAllAtOnce::create();
        UTILS_BREAK_IF(touch_listener_ == nullptr);

        touch_listener_->onTouchesBegan = CC_CALLBACK_2(on_screen_button::on_touches_began, this);
        touch_listener_->onTouchesEnded = CC_CALLBACK_2(on_screen_button::on_touches_ended, this);
        touch_listener_->onTouchesMoved = CC_CALLBACK_2(on_screen_button::on_touches_moved, this);
        touch_listener_->onTouchesCancelled = CC_CALLBACK_2(on_screen_button::on_touches_cancel, this);

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
  }
  while (false);

  return ret;
}


void on_screen_button::on_touches_began(const std::vector<Touch*>& touches, Event* unused_event)
{
  if (pushed_)
  {
    return;
  }

  for (const auto touch : touches)
  {
    const auto location = touch->getLocation();

    if (is_touched_by_location(location))
    {
      saved_touch_id_ = touch->getID();
      pushed(true);
      return;
    }
  }
}

void on_screen_button::on_touches_moved(const std::vector<Touch*>& touches, Event* unused_event)
{
  for (const auto touch : touches)
  {
    if (touch->getID() == saved_touch_id_)
    {
      const auto location = touch->getLocation();

      if (is_touched_by_location(location))
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

void on_screen_button::on_touches_ended(const std::vector<Touch*>& touches, Event* unused_event)
{
  for (const auto touch : touches)
  {
    if (touch->getID() == saved_touch_id_)
    {
      pushed(false);
      return;
    }
  }
}

void on_screen_button::on_touches_cancel(const std::vector<Touch*>& touches, Event* unused_event)
{
  on_touches_ended(touches, unused_event);
}
