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
#ifndef __VIRTUAL_JOY_STICK_CLASS__
#define __VIRTUAL_JOY_STICK_CLASS__

#include "../utils/utils.h"

// forward declarations
class on_screen_button;
enum class button_type;
class InputController;

class virtual_joy_stick final : public Node
{
public:
  using base_class = Node;

  virtual_joy_stick();

  static virtual_joy_stick* create(InputController* input_controller);

  bool init(InputController* input_controller);

  bool left() const;

  bool right() const;

  bool jump() const;

  void disabled(const bool disabled);

private:
  on_screen_button* add_on_screen_button(const button_type& type, const std::string& sprite_frame_name,
                                         const std::string& label = "");

  bool add_on_screen_buttons();

  bool is_on_screen_pushed(const button_type& type) const;

  std::vector<on_screen_button*> on_screen_buttons_;

  InputController* input_controller_;
};

#endif // __VIRTUAL_JOY_STICK_CLASS__
