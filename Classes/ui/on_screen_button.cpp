/****************************************************************************
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
  pushed_(false)
{
}

on_screen_button* on_screen_button::create(const std::string& normal_sprite_frame_name,
                                           const std::string& pushed_sprite_frame_name)
{
  on_screen_button* ret = nullptr;

  do
  {
    auto object = new on_screen_button();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(normal_sprite_frame_name, pushed_sprite_frame_name))
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

  // return the object
  return ret;
}

bool on_screen_button::init(const std::string& normal_sprite_frame_name, const std::string& pushed_sprite_frame_name)
{
  auto ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first
    UTILS_BREAK_IF(!base_class::init());

    normal_sprite_ = Sprite::createWithSpriteFrameName(normal_sprite_frame_name);
    UTILS_BREAK_IF(normal_sprite_==nullptr);

    normal_sprite_->setOpacity(127);

    addChild(normal_sprite_);

    pushed_sprite_ = Sprite::createWithSpriteFrameName(pushed_sprite_frame_name);
    UTILS_BREAK_IF(pushed_sprite_ == nullptr);

    addChild(pushed_sprite_);

    pushed(false);

    ret = true;
  }
  while (false);

  return ret;
}

const cocos2d::Size& on_screen_button::getContentSize() const
{
  return normal_sprite_->getContentSize();
}

Rect on_screen_button::getBoundingBox() const
{
  return normal_sprite_->getBoundingBox();
}

bool on_screen_button::is_touched_by_location(const Vec2& location) const
{
  const auto pos = convertToNodeSpace(location);
  const auto box = normal_sprite_->getBoundingBox();
  return box.containsPoint(pos);
}

void on_screen_button::pushed(const bool pushed)
{
  normal_sprite_->setVisible(!pushed);
  pushed_sprite_->setVisible(pushed);

  pushed_ = pushed;
}
