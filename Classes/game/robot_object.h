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
#ifndef __ROBOT_CLASS__
#define __ROBOT_CLASS__

#include "../utils/utils.h"
#include "../utils/base/sprite/game_object.h"

class robot_object final : public game_object
{
public:
  // base_class
  using base_class = game_object;

  // constructor
  robot_object();

  // create the object
  static robot_object* create();

  // init this object
  bool init() override;

  // update our robot
  void update(float delta) override;

private:

  // robot states
  enum state
  {
    e_falling,
    e_jumping,
    e_idle,
    e_running
  };

  // we need to move to the left
  void move_to_left(bool to_left);

  // we need to move to the right
  void move_to_right(bool to_right);

  // we jump
  void jump() const;

  //we like to move to left
  bool to_left_;

  //we like to move to right
  bool to_right_;

  // our normal movement
  static const Vec2 normal_movement;

  // our current state
  state current_state_;

  // calculate our state
  state decide_state() const;

  // change our state
  void change_state(state wanted_state);

  // create a keyboard listener
  bool create_keyboard_listener();

  // on key press
  void on_key_pressed(EventKeyboard::KeyCode key_code, Event* event);

  // on key released
  void on_key_released(EventKeyboard::KeyCode key_code, Event* event);
};

#endif // __ROBOT_CLASS__
