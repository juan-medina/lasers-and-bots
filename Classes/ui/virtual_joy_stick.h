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
  using base_class = Node;

  virtual_joy_stick();

  static virtual_joy_stick* create();

  bool init() override;

  bool left() const;

  bool right() const;

  bool up() const;

  bool button_a() const;

  bool button_b() const;

  void disabled(const bool disabled);

  enum controller_codes
  {
    controller_d_pad_up = 10,
    controller_d_pad_down = 12,
    controller_d_pad_left = 13,
    controller_d_pad_right = 11,
    controller_button_a = 0,
    controller_button_b = 1,
    controller_left_shoulder = 4,
    controller_right_shoulder = 5,
    controller_start = 7,
    controller_back = 6,
    controller_left_axis_x = 0,
    controller_left_axis_y = 1,
    controller_right_axis_x = 2,
    controller_right_axis_y = 3
  };

private:

  on_screen_button* add_on_screen_button(const on_screen_button::button_type& type,
                                         const std::string& sprite_frame_name,
                                         const std::string& label = "");

  bool add_on_screen_buttons();

  void on_controller_key_down(Controller* controller, int key_code, Event* event);
  void on_controller_key_up(Controller* controller, int key_code, Event* event);
  void on_controller_axis(Controller* controller, int key_code, Event* event);

  bool create_controller_listener();

  bool create_keyboard_listener();

  void on_key_pressed(EventKeyboard::KeyCode key_code, Event* event);
  void on_key_released(EventKeyboard::KeyCode key_code, Event* event);

  bool is_on_screen_pushed(const on_screen_button::button_type& type) const;

  bool key_left_;
  bool key_right_;
  bool key_up_;
  bool key_button_a_;
  bool key_button_b_;

  bool controller_left_;
  bool controller_right_;
  bool controller_up_;
  bool controller_axis_left_;

  bool controller_axis_right_;
  bool controller_axis_up_;
  bool controller_button_a_;
  bool controller_button_b_;

  std::vector<on_screen_button*> on_screen_buttons_;
};

#endif // __VIRTUAL_JOY_STICK_CLASS__
