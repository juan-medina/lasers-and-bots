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
#ifndef __INPUT_CONTROLLER_CLASS__
#define __INPUT_CONTROLLER_CLASS__

#include "../utils.h"

class InputController final : public Ref
{
public:
  using BaseClass = Ref;

  InputController();
  ~InputController();

  static InputController* create();

  bool init();
  bool end();

  bool left() const;
  bool right() const;
  bool up() const;
  bool down() const;

  bool singlePressLeft() const;
  bool singlePressRight() const;
  bool singlePressDown() const;
  bool singlePressUp() const;

  bool buttonA() const;
  bool buttonB() const;
  bool buttonStart() const;
  bool buttonBack() const;

  bool singlePressButtonA() const;
  bool singlePressButtonB() const;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
  bool singlePressButtonStart();
#else
  bool singlePressButtonStart() const;
#endif

  bool singlePressButtonBack() const;

private:
  bool createControllerListener();
  bool createKeyboardListener();

  void onControllerKeyDown(Controller* controller, int keyCode, Event* event);
  void onControllerKeyUp(Controller* controller, int keyCode, Event* event);
  void onControllerAxis(Controller* controller, int keyCode, Event* event);

  void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
  void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

  bool _keyLeft;
  bool _keyRight;
  bool _keyUp;
  bool _keyDown;
  bool _keyButtonA;
  bool _keyButtonB;
  bool _keyButtonStart;
  bool _keyButtonBack;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
  bool _wasControllerMenuPressed;
#endif

  bool _controllerLeft;
  bool _controllerRight;
  bool _controllerUp;
  bool _controllerDown;

  bool _controllerAxisLeft;
  bool _controllerAxisRight;
  bool _controllerAxisUp;
  bool _controllerAxisDown;

  bool _controllerButtonA;
  bool _controllerButtonB;
  bool _controllerButtonStart;
  bool _controllerButtonBack;

  EventListenerKeyboard* _keyboardListener;
  EventListenerController* _controllerListener;

  bool _initiated;
};

#endif // __INPUT_CONTROLLER_CLASS__
