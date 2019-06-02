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

#include "virtual_joy_stick.h"
#include "../utils/controller/input_controller.h"
#include "on_screen_button.h"

virtual_joy_stick::virtual_joy_stick() : input_controller_(nullptr) {}

virtual_joy_stick* virtual_joy_stick::create(input_controller* input_controller)
{
  virtual_joy_stick* ret = nullptr;

  do
  {
    auto object = new virtual_joy_stick();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(input_controller))
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

bool virtual_joy_stick::init(input_controller* input_controller)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init());

    input_controller_ = input_controller;

#if (GAME_PLATFORM == MOBILE_GAME)
    UTILS_BREAK_IF(!add_on_screen_buttons());
#endif

    ret = true;
  } while (false);

  return ret;
}

on_screen_button* virtual_joy_stick::add_on_screen_button(const button_type& type,
                                                          const std::string& sprite_frame_name,
                                                          const std::string& label)
{
  on_screen_button* button = nullptr;
  do
  {
    button = on_screen_button::create(type, sprite_frame_name, label);
    UTILS_BREAK_IF(button == nullptr);

    addChild(button);

    on_screen_buttons_.push_back(button);
  } while (false);

  return button;
}

bool virtual_joy_stick::add_on_screen_buttons()
{
  auto ret = false;
  do
  {
    const auto on_screen_button_left = add_on_screen_button(button_type::left, "02_joystick_left");
    UTILS_BREAK_IF(on_screen_button_left == nullptr);
    const auto gap = Vec2(on_screen_button_left->getContentSize().width / 2,
                          on_screen_button_left->getContentSize().height / 4);
    const auto left_button_pos = Vec2(on_screen_button_left->getContentSize().width / 2,
                                      on_screen_button_left->getContentSize().height / 2) +
                                 gap;
    on_screen_button_left->setPosition(left_button_pos);

    const auto on_screen_button_right = add_on_screen_button(button_type::right, "02_joystick_right");
    UTILS_BREAK_IF(on_screen_button_right == nullptr);

    const auto right_button_pos =
      left_button_pos + Vec2(on_screen_button_left->getContentSize().width + gap.x * 2, 0.f);
    on_screen_button_right->setPosition(right_button_pos);

    const auto on_screen_button_a = add_on_screen_button(button_type::button_a, "02_joystick_empty", "A");
    UTILS_BREAK_IF(on_screen_button_a == nullptr);

    const auto size = Director::getInstance()->getOpenGLView()->getVisibleSize();
    const auto a_button_pos = Vec2(size.width - (on_screen_button_a->getContentSize().width / 2),
                                   on_screen_button_a->getContentSize().height / 2) +
                              Vec2(-gap.x, gap.y);
    on_screen_button_a->setPosition(a_button_pos);

    ret = true;
  } while (false);

  return ret;
}

bool virtual_joy_stick::is_on_screen_pushed(const button_type& type) const
{
  for (const auto button : on_screen_buttons_)
  {
    if (button->type() == type)
    {
      return button->is_pushed();
    }
  }
  return false;
}

bool virtual_joy_stick::left() const
{
  return input_controller_->left() || is_on_screen_pushed(button_type::left);
}

bool virtual_joy_stick::right() const
{
  return input_controller_->right() || is_on_screen_pushed(button_type::right);
}

bool virtual_joy_stick::jump() const
{
  return input_controller_->single_press_button_a() || input_controller_->up() ||
         is_on_screen_pushed(button_type::button_a);
}

void virtual_joy_stick::disabled(const bool disabled)
{
  for (const auto button : on_screen_buttons_)
  {
    button->disabled(disabled);
  }
}
