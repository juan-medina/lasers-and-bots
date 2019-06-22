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

#include "InputController.h"

InputController::InputController()
  : _keyLeft(false)
  , _keyRight(false)
  , _keyUp(false)
  , _keyDown(false)
  , _keyButtonA(false)
  , _keyButtonB(false)
  , _keyButtonStart(false)
  , _keyButtonBack(false)
  ,
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
  _wasControllerMenuPressed(false)
  ,
#endif
  _controllerLeft(false)
  , _controllerRight(false)
  , _controllerUp(false)
  , _controllerDown(false)
  , _controllerAxisLeft(false)
  , _controllerAxisRight(false)
  , _controllerAxisUp(false)
  , _controllerAxisDown(false)
  , _controllerButtonA(false)
  , _controllerButtonB(false)
  , _controllerButtonStart(false)
  , _controllerButtonBack(false)
  , _keyboardListener(nullptr)
  , _controllerListener(nullptr)
  , _initiated(false)
{
}

InputController::~InputController()
{
  if (_initiated)
  {
    end();
  }
}

InputController* InputController::create()
{
  InputController* ret = nullptr;

  do
  {
    auto object = new InputController();
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

bool InputController::init()
{
  auto ret = false;

  do
  {
#if (GAME_PLATFORM == DESKTOP_GAME)
    UTILS_BREAK_IF(!createKeyboardListener());
    UTILS_BREAK_IF(!createControllerListener());
#endif

    ret = true;
  } while (false);

  _initiated = ret;

  return ret;
}

bool InputController::end()
{
  if (_initiated)
  {
    if (_keyboardListener != nullptr)
    {
      _keyboardListener->setEnabled(false);
      Director::getInstance()->getEventDispatcher()->removeEventListener(_keyboardListener);
      _keyboardListener = nullptr;
    }
    if (_controllerListener != nullptr)
    {
#if (GAME_PLATFORM == DESKTOP_GAME)
      Controller::stopDiscoveryController();
#endif
      _controllerListener->setEnabled(false);
      Director::getInstance()->getEventDispatcher()->removeEventListener(_controllerListener);
      _controllerListener = nullptr;
    }
  }
  return true;
}

void InputController::onControllerKeyDown(Controller* controller, const int keyCode, Event* event)
{
  switch (keyCode)
  {
    case Controller::Key::BUTTON_DPAD_UP:
      _controllerUp = true;
      break;
    case Controller::Key::BUTTON_DPAD_DOWN:
      _controllerDown = true;
      break;
    case Controller::Key::BUTTON_DPAD_LEFT:
    case Controller::Key::BUTTON_LEFT_SHOULDER:
      _controllerLeft = true;
      ;
      break;
    case Controller::Key::BUTTON_DPAD_RIGHT:
    case Controller::Key::BUTTON_RIGHT_SHOULDER:
      _controllerRight = true;
      break;
    case Controller::Key::BUTTON_A:
      _controllerButtonA = true;
      break;
    case Controller::Key::BUTTON_B:
      _controllerButtonB = true;
      break;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    case Controller::Key::BUTTON_PAUSE:
      was_controller_menu_pressed_ = true;
      break;
#endif
    case Controller::Key::BUTTON_START:
      _controllerButtonStart = true;
      break;
    case Controller::Key::BUTTON_SELECT:
      _controllerButtonBack = true;
      break;
    default:
      break;
  }
}

void InputController::onControllerKeyUp(Controller* controller, const int keyCode, Event* event)
{
  switch (keyCode)
  {
    case Controller::Key::BUTTON_DPAD_UP:
      _controllerUp = false;
      break;
    case Controller::Key::BUTTON_DPAD_DOWN:
      _controllerDown = false;
      break;
    case Controller::Key::BUTTON_DPAD_LEFT:
    case Controller::Key::BUTTON_LEFT_SHOULDER:
      _controllerLeft = false;
      ;
      break;
    case Controller::Key::BUTTON_DPAD_RIGHT:
    case Controller::Key::BUTTON_RIGHT_SHOULDER:
      _controllerRight = false;
      break;
    case Controller::Key::BUTTON_A:
      _controllerButtonA = false;
      break;
    case Controller::Key::BUTTON_B:
      _controllerButtonB = false;
      break;
    case Controller::Key::BUTTON_START:
      _controllerButtonStart = false;
      break;
    case Controller::Key::BUTTON_SELECT:
      _controllerButtonBack = false;
      break;
    default:
      break;
  }
}

void InputController::onControllerAxis(Controller* controller, const int keyCode, Event* event)
{
  static const auto THRESHOLD = 0.5f;
  const auto& value = controller->getKeyStatus(keyCode).value;

  if (keyCode == Controller::Key::JOYSTICK_LEFT_X)
  {
    if (fabs(value) > THRESHOLD)
    {
      _controllerAxisLeft = value < THRESHOLD;
      _controllerAxisRight = value > THRESHOLD;
    }
    else
    {
      _controllerAxisLeft = false;
      _controllerAxisRight = false;
    }
  }
  else if (keyCode == Controller::Key::JOYSTICK_LEFT_Y)
  {
    if (fabs(value) > 0.5f)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
      _controllerAxisUp = value < THRESHOLD;
      _controllerAxisDown = value > THRESHOLD;
#else
      _controllerAxisUp = value > THRESHOLD;
      _controllerAxisDown = value < THRESHOLD;
#endif
    }
    else
    {
      _controllerAxisUp = false;
      _controllerAxisDown = false;
    }
  }
}

bool InputController::createControllerListener()
{
  auto ret = false;

  do
  {
#if (GAME_PLATFORM == DESKTOP_GAME)
    _controllerListener = EventListenerController::create();
    UTILS_BREAK_IF(_controllerListener == nullptr);

    _controllerListener->onKeyDown = CC_CALLBACK_3(InputController::onControllerKeyDown, this);
    _controllerListener->onKeyUp = CC_CALLBACK_3(InputController::onControllerKeyUp, this);
    _controllerListener->onAxisEvent = CC_CALLBACK_3(InputController::onControllerAxis, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_controllerListener, 1);

    Controller::startDiscoveryController();
    Controller::stopDiscoveryController();
    Controller::startDiscoveryController();
#endif
    ret = true;
  } while (false);

  return ret;
}

bool InputController::createKeyboardListener()
{
  auto result = false;

  do
  {
#if (GAME_PLATFORM == DESKTOP_GAME)
    _keyboardListener = EventListenerKeyboard::create();
    UTILS_BREAK_IF(_keyboardListener == nullptr);

    _keyboardListener->onKeyPressed = CC_CALLBACK_2(InputController::onKeyPressed, this);
    _keyboardListener->onKeyReleased = CC_CALLBACK_2(InputController::onKeyReleased, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_keyboardListener, 1);
#endif
    result = true;
  } while (false);

  return result;
}

void InputController::onKeyPressed(const EventKeyboard::KeyCode keyCode, Event*)
{
  switch (keyCode)
  {
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_W:
      _keyUp = true;
      break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_S:
      _keyDown = true;
      break;
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
      _keyLeft = true;
      ;
      break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_D:
      _keyRight = true;
      break;
    case EventKeyboard::KeyCode::KEY_SPACE:
      _keyButtonA = true;
      break;
    case EventKeyboard::KeyCode::KEY_LEFT_CTRL:
    case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
      _keyButtonB = true;
      break;
    case EventKeyboard::KeyCode::KEY_F5:
    case EventKeyboard::KeyCode::KEY_ENTER:
    case EventKeyboard::KeyCode::KEY_RETURN:
    case EventKeyboard::KeyCode::KEY_KP_ENTER:
      _keyButtonStart = true;
      break;
    case EventKeyboard::KeyCode::KEY_ESCAPE:
      _keyButtonBack = true;
      break;
    default:
      break;
  }
}

void InputController::onKeyReleased(const EventKeyboard::KeyCode keyCode, Event*)
{
  switch (keyCode)
  {
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_W:
      _keyUp = false;
      break;
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
      _keyLeft = false;
      ;
      break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_S:
      _keyDown = false;
      break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_D:
      _keyRight = false;
      break;
    case EventKeyboard::KeyCode::KEY_SPACE:
      _keyButtonA = false;
      break;
    case EventKeyboard::KeyCode::KEY_LEFT_CTRL:
    case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
      _keyButtonB = false;
      break;
    case EventKeyboard::KeyCode::KEY_F5:
    case EventKeyboard::KeyCode::KEY_ENTER:
    case EventKeyboard::KeyCode::KEY_RETURN:
    case EventKeyboard::KeyCode::KEY_KP_ENTER:
      _keyButtonStart = false;
      break;
    case EventKeyboard::KeyCode::KEY_ESCAPE:
      _keyButtonBack = false;
      break;
    default:
      break;
  }
}

bool InputController::left() const
{
  return _keyLeft || _controllerLeft || _controllerAxisLeft;
}

bool InputController::right() const
{
  return _keyRight || _controllerRight || _controllerAxisRight;
}

bool InputController::up() const
{
  return _keyUp || _controllerUp || _controllerAxisUp;
}

bool InputController::down() const
{
  return _keyDown || _controllerDown || _controllerAxisDown;
}

bool InputController::singlePressLeft() const
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

bool InputController::singlePressRight() const
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

bool InputController::singlePressDown() const
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

bool InputController::singlePressUp() const
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

bool InputController::buttonA() const
{
  return _keyButtonA || _controllerButtonA;
}

bool InputController::buttonB() const
{
  return _keyButtonB || _controllerButtonB;
}

bool InputController::buttonStart() const
{
  return _keyButtonStart || _controllerButtonStart;
}

bool InputController::buttonBack() const
{
  return _keyButtonBack || _controllerButtonBack;
}

bool InputController::singlePressButtonA() const
{
  static auto pressed = false;

  if (buttonA())
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

bool InputController::singlePressButtonB() const
{
  static auto pressed = false;

  if (buttonB())
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
bool InputController::singlePressButtonStart()
{
  static auto pressed = false;
  if (buttonStart() || _wasControllerMenuPressed)
  {
    _wasControllerMenuPressed = false;
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
bool InputController::singlePressButtonStart() const
{
  static auto pressed = false;
  if (buttonStart())
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

bool InputController::singlePressButtonBack() const
{
  static auto pressed = false;

  if (buttonBack())
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
