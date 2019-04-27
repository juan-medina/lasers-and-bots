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
#ifndef __VIRTUAL_JOY_STICK_CLASS__
#define __VIRTUAL_JOY_STICK_CLASS__

#include "../utils/utils.h"
#include "on_screen_button.h"

class virtual_joy_stick final : public Node
{
public:
  // base_class
  using base_class = Node;

  // constructor
  virtual_joy_stick();

  // create the object
  static virtual_joy_stick* create();

  // add one button
  on_screen_button* add_on_screen_button(const on_screen_button::button_type& type,
                                         const std::string& sprite_frame_name,
                                         const std::string& label = "");

  // add on screen buttons
  bool add_on_screen_buttons();

  // init this object
  bool init() override;

  // joystick left
  bool left() const;

  // joystick right
  bool right() const;

  // joystick up
  bool up() const;

  // joystick a button
  bool button_a() const;

  // joystick b button
  bool button_b() const;

  // disable virtual joystick
  void disabled(const bool disabled);

private:

  // create a keyboard listener
  bool create_keyboard_listener();

  // on key press
  void on_key_pressed(EventKeyboard::KeyCode key_code, Event* event);

  // on key released
  void on_key_released(EventKeyboard::KeyCode key_code, Event* event);

  // is a on screen button pushed
  bool is_on_screen_pushed(const on_screen_button::button_type& type) const;

  // key left pressed
  bool key_left_;

  // key right pressed
  bool key_right_;

  // key up pressed
  bool key_up_;

  // button a in the keyboard
  bool key_button_a_;

  // button b in the keyboard
  bool key_button_b_;

  // on screen buttons
  std::vector<on_screen_button*> on_screen_buttons_;
};

#endif // __VIRTUAL_JOY_STICK_CLASS__
