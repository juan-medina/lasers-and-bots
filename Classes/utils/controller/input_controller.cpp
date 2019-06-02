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

#include "input_controller.h"

input_controller::input_controller()
  : key_left_(false)
  , key_right_(false)
  , key_up_(false)
  , key_down_(false)
  , key_button_a_(false)
  , key_button_b_(false)
  , key_button_start_(false)
  , key_button_back_(false)
  ,
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
  was_controller_menu_pressed_(false)
  ,
#endif
  controller_left_(false)
  , controller_right_(false)
  , controller_up_(false)
  , controller_down_(false)
  , controller_axis_left_(false)
  , controller_axis_right_(false)
  , controller_axis_up_(false)
  , controller_axis_down_(false)
  , controller_button_a_(false)
  , controller_button_b_(false)
  , controller_button_start_(false)
  , controller_button_back_(false)
  , keyboard_listener_(nullptr)
  , controller_listener_(nullptr)
  , initiated_(false)
{
}

input_controller::~input_controller()
{
  if (initiated_)
  {
    end();
  }
}

input_controller* input_controller::create()
{
  input_controller* ret = nullptr;

  do
  {
    auto object = new input_controller();
    UTILS_BREAK_IF(object == nullptr);

    if (!object->init())
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  } while (false);

  return ret;
}

bool input_controller::init()
{
  auto ret = false;

  do
  {
#if (GAME_PLATFORM == DESKTOP_GAME)
    UTILS_BREAK_IF(!create_keyboard_listener());
    UTILS_BREAK_IF(!create_controller_listener());
#endif

    ret = true;
  } while (false);

  initiated_ = ret;

  return ret;
}

bool input_controller::end()
{
  if (initiated_)
  {
    if (keyboard_listener_ != nullptr)
    {
      keyboard_listener_->setEnabled(false);
      Director::getInstance()->getEventDispatcher()->removeEventListener(keyboard_listener_);
      keyboard_listener_ = nullptr;
    }
    if (controller_listener_ != nullptr)
    {
#if (GAME_PLATFORM == DESKTOP_GAME)
      Controller::stopDiscoveryController();
#endif
      controller_listener_->setEnabled(false);
      Director::getInstance()->getEventDispatcher()->removeEventListener(controller_listener_);
      controller_listener_ = nullptr;
    }
  }
  return true;
}

void input_controller::on_controller_key_down(Controller* controller, const int key_code, Event* event)
{
  switch (key_code)
  {
    case Controller::Key::BUTTON_DPAD_UP:
      controller_up_ = true;
      break;
    case Controller::Key::BUTTON_DPAD_DOWN:
      controller_down_ = true;
      break;
    case Controller::Key::BUTTON_DPAD_LEFT:
    case Controller::Key::BUTTON_LEFT_SHOULDER:
      controller_left_ = true;
      ;
      break;
    case Controller::Key::BUTTON_DPAD_RIGHT:
    case Controller::Key::BUTTON_RIGHT_SHOULDER:
      controller_right_ = true;
      break;
    case Controller::Key::BUTTON_A:
      controller_button_a_ = true;
      break;
    case Controller::Key::BUTTON_B:
      controller_button_b_ = true;
      break;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    case Controller::Key::BUTTON_PAUSE:
      was_controller_menu_pressed_ = true;
      break;
#endif
    case Controller::Key::BUTTON_START:
      controller_button_start_ = true;
      break;
    case Controller::Key::BUTTON_SELECT:
      controller_button_back_ = true;
      break;
    default:
      break;
  }
}

void input_controller::on_controller_key_up(Controller* controller, const int key_code, Event* event)
{
  switch (key_code)
  {
    case Controller::Key::BUTTON_DPAD_UP:
      controller_up_ = false;
      break;
    case Controller::Key::BUTTON_DPAD_DOWN:
      controller_down_ = false;
      break;
    case Controller::Key::BUTTON_DPAD_LEFT:
    case Controller::Key::BUTTON_LEFT_SHOULDER:
      controller_left_ = false;
      ;
      break;
    case Controller::Key::BUTTON_DPAD_RIGHT:
    case Controller::Key::BUTTON_RIGHT_SHOULDER:
      controller_right_ = false;
      break;
    case Controller::Key::BUTTON_A:
      controller_button_a_ = false;
      break;
    case Controller::Key::BUTTON_B:
      controller_button_b_ = false;
      break;
    case Controller::Key::BUTTON_START:
      controller_button_start_ = false;
      break;
    case Controller::Key::BUTTON_SELECT:
      controller_button_back_ = false;
      break;
    default:
      break;
  }
}

void input_controller::on_controller_axis(Controller* controller, const int key_code, Event* event)
{
  static const auto threshold = 0.5f;
  const auto& value = controller->getKeyStatus(key_code).value;

  if (key_code == Controller::Key::JOYSTICK_LEFT_X)
  {
    if (fabs(value) > threshold)
    {
      controller_axis_left_ = value < threshold;
      controller_axis_right_ = value > threshold;
    }
    else
    {
      controller_axis_left_ = false;
      controller_axis_right_ = false;
    }
  }
  else if (key_code == Controller::Key::JOYSTICK_LEFT_Y)
  {
    if (fabs(value) > 0.5f)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
      controller_axis_up_ = value < threshold;
      controller_axis_down_ = value > threshold;
#else
      controller_axis_up_ = value > threshold;
      controller_axis_down_ = value < threshold;
#endif
    }
    else
    {
      controller_axis_up_ = false;
      controller_axis_down_ = false;
    }
  }
}

bool input_controller::create_controller_listener()
{
  auto ret = false;

  do
  {
#if (GAME_PLATFORM == DESKTOP_GAME)
    controller_listener_ = EventListenerController::create();
    UTILS_BREAK_IF(controller_listener_ == nullptr);

    controller_listener_->onKeyDown = CC_CALLBACK_3(input_controller::on_controller_key_down, this);
    controller_listener_->onKeyUp = CC_CALLBACK_3(input_controller::on_controller_key_up, this);
    controller_listener_->onAxisEvent = CC_CALLBACK_3(input_controller::on_controller_axis, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(controller_listener_, 1);

    Controller::startDiscoveryController();
    Controller::stopDiscoveryController();
    Controller::startDiscoveryController();
#endif
    ret = true;
  } while (false);

  return ret;
}

bool input_controller::create_keyboard_listener()
{
  auto result = false;

  do
  {
#if (GAME_PLATFORM == DESKTOP_GAME)
    keyboard_listener_ = EventListenerKeyboard::create();
    UTILS_BREAK_IF(keyboard_listener_ == nullptr);

    keyboard_listener_->onKeyPressed = CC_CALLBACK_2(input_controller::on_key_pressed, this);
    keyboard_listener_->onKeyReleased = CC_CALLBACK_2(input_controller::on_key_released, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(keyboard_listener_, 1);
#endif
    result = true;
  } while (false);

  return result;
}

void input_controller::on_key_pressed(const EventKeyboard::KeyCode key_code, Event*)
{
  switch (key_code)
  {
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_W:
      key_up_ = true;
      break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_S:
      key_down_ = true;
      break;
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
      key_left_ = true;
      ;
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
    case EventKeyboard::KeyCode::KEY_F5:
    case EventKeyboard::KeyCode::KEY_ENTER:
    case EventKeyboard::KeyCode::KEY_RETURN:
    case EventKeyboard::KeyCode::KEY_KP_ENTER:
      key_button_start_ = true;
      break;
    case EventKeyboard::KeyCode::KEY_ESCAPE:
      key_button_back_ = true;
      break;
    default:
      break;
  }
}

void input_controller::on_key_released(const EventKeyboard::KeyCode key_code, Event*)
{
  switch (key_code)
  {
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_W:
      key_up_ = false;
      break;
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
      key_left_ = false;
      ;
      break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_S:
      key_down_ = false;
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
    case EventKeyboard::KeyCode::KEY_F5:
    case EventKeyboard::KeyCode::KEY_ENTER:
    case EventKeyboard::KeyCode::KEY_RETURN:
    case EventKeyboard::KeyCode::KEY_KP_ENTER:
      key_button_start_ = false;
      break;
    case EventKeyboard::KeyCode::KEY_ESCAPE:
      key_button_back_ = false;
      break;
    default:
      break;
  }
}

bool input_controller::left() const
{
  return key_left_ || controller_left_ || controller_axis_left_;
}

bool input_controller::right() const
{
  return key_right_ || controller_right_ || controller_axis_right_;
}

bool input_controller::up() const
{
  return key_up_ || controller_up_ || controller_axis_up_;
}

bool input_controller::down() const
{
  return key_down_ || controller_down_ || controller_axis_down_;
}

bool input_controller::single_press_left() const
{
  static auto pressed = false;

  if (left())
  {
    if (pressed)
    {
      return false;
    }
    pressed = true;
    return true;
  }
  pressed = false;
  return false;
}

bool input_controller::single_press_right() const
{
  static auto pressed = false;

  if (right())
  {
    if (pressed)
    {
      return false;
    }
    pressed = true;
    return true;
  }
  pressed = false;
  return false;
}

bool input_controller::single_press_down() const
{
  static auto pressed = false;

  if (down())
  {
    if (pressed)
    {
      return false;
    }
    pressed = true;
    return true;
  }
  pressed = false;
  return false;
}

bool input_controller::single_press_up() const
{
  static auto pressed = false;

  if (up())
  {
    if (pressed)
    {
      return false;
    }
    pressed = true;
    return true;
  }
  pressed = false;
  return false;
}

bool input_controller::button_a() const
{
  return key_button_a_ || controller_button_a_;
}

bool input_controller::button_b() const
{
  return key_button_b_ || controller_button_b_;
}

bool input_controller::button_start() const
{
  return key_button_start_ || controller_button_start_;
}

bool input_controller::button_back() const
{
  return key_button_back_ || controller_button_back_;
}

bool input_controller::single_press_button_a() const
{
  static auto pressed = false;

  if (button_a())
  {
    if (pressed)
    {
      return false;
    }
    pressed = true;
    return true;
  }
  pressed = false;
  return false;
}

bool input_controller::single_press_button_b() const
{
  static auto pressed = false;

  if (button_b())
  {
    if (pressed)
    {
      return false;
    }
    pressed = true;
    return true;
  }
  pressed = false;
  return false;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
bool input_controller::single_press_button_start()
{
  static auto pressed = false;
  if (button_start() || was_controller_menu_pressed_)
  {
    was_controller_menu_pressed_ = false;
    if (pressed)
    {
      return false;
    }
    pressed = true;

    return true;
  }
  pressed = false;

  return false;
}
#else
bool input_controller::single_press_button_start() const
{
  static auto pressed = false;
  if (button_start())
  {
    if (pressed)
    {
      return false;
    }
    pressed = true;

    return true;
  }
  pressed = false;

  return false;
}
#endif

bool input_controller::single_press_button_back() const
{
  static auto pressed = false;

  if (button_back())
  {
    if (pressed)
    {
      return false;
    }
    pressed = true;
    return true;
  }
  pressed = false;
  return false;
}
