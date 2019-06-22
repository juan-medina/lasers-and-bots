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

#include "on_off_object.h"

on_off_object::on_off_object() : on_(false), activated_(false), spot_(nullptr) {}

bool on_off_object::init(PhysicsShapeCache* physics_shape_cache, const std::string& shape,
                         const std::string& sprite_frame_name, const std::string& type, const Vec2& spot_pos,
                         const string& target /*= ""*/)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(physics_shape_cache, shape, sprite_frame_name, type));

    spot_ = createWithSpriteFrameName("10_spot.png");

    UTILS_BREAK_IF(spot_ == nullptr);

    spot_->setPosition(spot_pos.x, getContentSize().height - spot_pos.y);

    spot_->setVisible(true);
    spot_->setColor(Color3B::RED);
    spot_->setOpacity(255);
    spot_->setBlendFunc(BlendFunc::ADDITIVE);

    const auto fade_down = FadeTo::create(0.5, 90);
    const auto fade_up = FadeTo::create(0.5, 255);
    const auto blink = Sequence::create(fade_down, fade_up, nullptr);
    const auto repeat = RepeatForever::create(blink);

    spot_->runAction(repeat);

    addChild(spot_);

    target_ = target;

    ret = true;
  } while (false);

  return ret;
}

bool on_off_object::on()
{
  if (is_off())
  {
    on_ = true;
    change_spot_color(Color3B::GREEN);
    return true;
  }

  return false;
}

bool on_off_object::activate()
{
  if (is_unactivated())
  {
    activated_ = true;
    change_spot_color(Color3B::YELLOW);
    return true;
  }

  return false;
}

void on_off_object::change_spot_color(const Color3B& color) const
{
  spot_->setColor(color);
}
