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

#include "virtual_joy_stick.h"
#include "on_screen_button.h"

virtual_joy_stick::virtual_joy_stick():
  key_left_(false),
  key_right_(false),
  key_up_(false),
  key_button_a_(false),
  key_button_b_(false),
  controller_left_(false),
  controller_right_(false),
  controller_up_(false),
  controller_axis_left_(false),
  controller_axis_right_(false),
  controller_axis_up_(false),
  controller_button_a_(false),
  controller_button_b_(false)
{
}

virtual_joy_stick* virtual_joy_stick::create()
{
  virtual_joy_stick* ret = nullptr;

  do
  {
    auto object = new virtual_joy_stick();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init())
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

bool virtual_joy_stick::init()
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init());

#if (GAME_PLATFORM == DESKTOP_GAME)
    UTILS_BREAK_IF(!create_keyboard_listener());
#else
    UTILS_BREAK_IF(!add_on_screen_buttons());
#endif

    UTILS_BREAK_IF(!create_controller_listener());

    ret = true;
  }
  while (false);

  return ret;
}

on_screen_button* virtual_joy_stick::add_on_screen_button(const on_screen_button::button_type& type,
                                                          const std::string& sprite_frame_name,
                                                          const std::string& label/* = ""*/)
{
  on_screen_button* button = nullptr;
  do
  {
    button = on_screen_button::create(type, sprite_frame_name, label);
    UTILS_BREAK_IF(button == nullptr);

    addChild(button);

    on_screen_buttons_.push_back(button);
  }
  while (false);

  return button;
}

bool virtual_joy_stick::add_on_screen_buttons()
{
  auto ret = false;
  do
  {
    const auto on_screen_button_left = add_on_screen_button(on_screen_button::button_type::left, "02_joystick_left");
    UTILS_BREAK_IF(on_screen_button_left == nullptr);
    const auto gap = Vec2(on_screen_button_left->getContentSize().width / 2,
                          on_screen_button_left->getContentSize().height / 4);
    const auto left_button_pos = Vec2(on_screen_button_left->getContentSize().width / 2,
                                      on_screen_button_left->getContentSize().height / 2) + gap;
    on_screen_button_left->setPosition(left_button_pos);

    const auto on_screen_button_right = add_on_screen_button(on_screen_button::button_type::right, "02_joystick_right");
    UTILS_BREAK_IF(on_screen_button_right == nullptr);

    const auto right_button_pos = left_button_pos + Vec2(on_screen_button_left->getContentSize().width + gap.x * 2,
                                                         0.f);
    on_screen_button_right->setPosition(right_button_pos);

    const auto on_screen_button_up = add_on_screen_button(on_screen_button::button_type::up, "02_joystick_up");
    UTILS_BREAK_IF(on_screen_button_up == nullptr);

    const auto up_button_pos = Vec2((on_screen_button_up->getContentSize().width / 2) + gap.x,
                                    on_screen_button_up->getContentSize().height / 2) + Vec2(gap.x * 2, gap.y * 10);
    on_screen_button_up->setPosition(up_button_pos);

    const auto on_screen_button_a = add_on_screen_button(on_screen_button::button_type::button_a,
                                                         "02_joystick_empty", "A");
    UTILS_BREAK_IF(on_screen_button_a == nullptr);

    const auto size = Director::getInstance()->getOpenGLView()->getVisibleSize();
    const auto a_button_pos = Vec2(size.width - (on_screen_button_a->getContentSize().width / 2),
                                   on_screen_button_a->getContentSize().height / 2) + Vec2(-gap.x, gap.y);
    on_screen_button_a->setPosition(a_button_pos);

    ret = true;
  }
  while (false);

  return ret;
}

void virtual_joy_stick::on_controller_key_down(Controller* controller, int key_code, Event* event)
{
  switch (key_code)
  {
  case controller_d_pad_up:
    controller_up_ = true;
    break;
  case controller_d_pad_left:
  case controller_left_shoulder:
    controller_left_ = true;;
    break;
  case controller_d_pad_right:
  case controller_right_shoulder:
    controller_right_ = true;
    break;
  case controller_button_a:
    controller_button_a_ = true;
    break;
  case controller_button_b:
    controller_button_b_ = true;
    break;
  default:
    break;
  }
}

void virtual_joy_stick::on_controller_key_up(Controller* controller, int key_code, Event* event)
{
  switch (key_code)
  {
  case controller_d_pad_up:
    controller_up_ = false;
    break;
  case controller_d_pad_left:
  case controller_left_shoulder:
    controller_left_ = false;;
    break;
  case controller_d_pad_right:
  case controller_right_shoulder:
    controller_right_ = false;
    break;
  case controller_button_a:
    controller_button_a_ = false;
    break;
  case controller_button_b:
    controller_button_b_ = false;
    break;
  default:
    break;
  }
}

void virtual_joy_stick::on_controller_axis(Controller* controller, int key_code, Event* event)
{
  const auto& value = controller->getKeyStatus(key_code).value;

  if (key_code == controller_left_axis_x)
  {
    if (fabs(value) > 0.5f)
    {
      controller_axis_left_ = value < 0.5f;
      controller_axis_right_ = value > 0.5f;
    }
    else
    {
      controller_axis_left_ = false;
      controller_axis_right_ = false;
    }
  }
  else if (key_code == controller_left_axis_y)
  {
    if (fabs(value) > 0.5f)
    {
      controller_axis_up_ = value > 0.5f;
    }
    else
    {
      controller_axis_up_ = false;
    }
  }
}

bool virtual_joy_stick::create_controller_listener()
{
  auto ret = false;

  do
  {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    const auto listener = EventListenerController::create();

    listener->onKeyDown = CC_CALLBACK_3(virtual_joy_stick::on_controller_key_down, this);
    listener->onKeyUp = CC_CALLBACK_3(virtual_joy_stick::on_controller_key_up, this);
    listener->onAxisEvent = CC_CALLBACK_3(virtual_joy_stick::on_controller_axis, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    Controller::startDiscoveryController();
#endif
    ret = true;
  }
  while (false);

  return ret;
}

bool virtual_joy_stick::create_keyboard_listener()
{
  auto result = false;

  do
  {
    auto listener = EventListenerKeyboard::create();
    UTILS_BREAK_IF(listener == nullptr);

    listener->onKeyPressed = CC_CALLBACK_2(virtual_joy_stick::on_key_pressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(virtual_joy_stick::on_key_released, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    result = true;
  }
  while (false);

  return result;
}

void virtual_joy_stick::on_key_pressed(const EventKeyboard::KeyCode key_code, Event*)
{
  switch (key_code)
  {
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
  case EventKeyboard::KeyCode::KEY_W:
    key_up_ = true;
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    key_left_ = true;;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    key_right_ = true;
    break;
  case EventKeyboard::KeyCode::KEY_SPACE:
    key_button_a_ = true;
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_CTRL:
  case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
    key_button_b_ = true;
    break;
  default:
    break;
  }
}

void virtual_joy_stick::on_key_released(const EventKeyboard::KeyCode key_code, Event*)
{
  switch (key_code)
  {
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
  case EventKeyboard::KeyCode::KEY_W:
    key_up_ = false;
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    key_left_ = false;;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    key_right_ = false;
    break;
  case EventKeyboard::KeyCode::KEY_SPACE:
    key_button_a_ = false;
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_CTRL:
  case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
    key_button_b_ = false;
    break;
  default:
    break;
  }
}

bool virtual_joy_stick::is_on_screen_pushed(const on_screen_button::button_type& type) const
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
  return key_left_ || controller_left_ || controller_axis_left_ || is_on_screen_pushed(
    on_screen_button::button_type::left);
}

bool virtual_joy_stick::right() const
{
  return key_right_ || controller_right_ || controller_axis_right_ || is_on_screen_pushed(
    on_screen_button::button_type::right);
}

bool virtual_joy_stick::up() const
{
  return key_up_ || controller_up_ || controller_axis_up_ || is_on_screen_pushed(on_screen_button::button_type::up);
}

bool virtual_joy_stick::button_a() const
{
  return key_button_a_ || controller_button_a_ || is_on_screen_pushed(on_screen_button::button_type::button_a);
}

bool virtual_joy_stick::button_b() const
{
  return key_button_b_ || controller_button_b_ || is_on_screen_pushed(on_screen_button::button_type::button_b);
}

void virtual_joy_stick::disabled(const bool disabled)
{
  for (const auto button : on_screen_buttons_)
  {
    button->disabled(disabled);
  }
}
