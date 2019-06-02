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

// forward declarations
enum class button_type;

class input_controller final : public Ref
{
public:
  using base_class = Ref;

  input_controller();
  ~input_controller();

  static input_controller* create();

  bool init();
  bool end();

  bool left() const;
  bool right() const;
  bool up() const;
  bool down() const;

  bool single_press_left() const;
  bool single_press_right() const;
  bool single_press_down() const;
  bool single_press_up() const;

  bool button_a() const;
  bool button_b() const;
  bool button_start() const;
  bool button_back() const;

  bool single_press_button_a() const;
  bool single_press_button_b() const;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
  bool single_press_button_start();
#else
  bool single_press_button_start() const;
#endif

  bool single_press_button_back() const;

private:
  bool create_controller_listener();
  bool create_keyboard_listener();

  void on_controller_key_down(Controller* controller, int key_code, Event* event);
  void on_controller_key_up(Controller* controller, int key_code, Event* event);
  void on_controller_axis(Controller* controller, int key_code, Event* event);

  void on_key_pressed(EventKeyboard::KeyCode key_code, Event* event);
  void on_key_released(EventKeyboard::KeyCode key_code, Event* event);

  bool key_left_;
  bool key_right_;
  bool key_up_;
  bool key_down_;
  bool key_button_a_;
  bool key_button_b_;
  bool key_button_start_;
  bool key_button_back_;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
  bool was_controller_menu_pressed_;
#endif

  bool controller_left_;
  bool controller_right_;
  bool controller_up_;
  bool controller_down_;

  bool controller_axis_left_;
  bool controller_axis_right_;
  bool controller_axis_up_;
  bool controller_axis_down_;

  bool controller_button_a_;
  bool controller_button_b_;
  bool controller_button_start_;
  bool controller_button_back_;

  EventListenerKeyboard* keyboard_listener_;
  EventListenerController* controller_listener_;

  bool initiated_;
};

#endif // __INPUT_CONTROLLER_CLASS__
