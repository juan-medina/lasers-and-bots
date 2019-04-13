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

class virtual_joy_stick final : public Node
{
public:
  // base_class
  using base_class = Node;

  // constructor
  virtual_joy_stick();

  // create the object
  static virtual_joy_stick* create(float blind_y);

  // init this object
  bool init(float blind_y);

  // get joystick velocity
  inline const Vec2& get_velocity() const
  {
    return velocity_;
  }

  bool get_left() const
  {
    return (velocity_.x < -0.5f) || key_left_;
  }

  constexpr bool get_right() const
  {
    return (velocity_.x > 0.5f) || key_right_;
  }

  constexpr bool get_up() const
  {
    return (velocity_.y > 0.5f) || key_up_;
  }

  constexpr bool get_down() const
  {
    return (velocity_.y < -0.5f) || key_down_;
  }

private:
  // get a touch location in node space
  Vec2 get_location_in_node_space(Touch* touch);

  // begin touch event
  bool on_touch_began(Touch* touch, Event* unused_event);

  // touch moved event
  void on_touch_moved(Touch* touch, Event* unused_event);

  // touch end event
  void on_touch_ended(Touch* touch, Event* unused_event);

  // touch cancel
  void on_touch_cancel(Touch* touch, Event* unused_event);

  // register touch events
  bool create_touch_listener();

  // update velocity
  void update_velocity(Point point);

  // joystick
  Sprite* joy_stick_;

  // thumb
  Sprite* thumb_;

  // velocity
  Vec2 velocity_;

  // joystick radius
  float joy_stick_radius_;

  // thumb radius
  float thumb_radius_;

  // create a keyboard listener
  bool create_keyboard_listener();

  // on key press
  void on_key_pressed(EventKeyboard::KeyCode key_code, Event* event);

  // on key released
  void on_key_released(EventKeyboard::KeyCode key_code, Event* event);

  // key left pressed
  bool key_left_;

  // key right pressed
  bool key_right_;

  // key up pressed
  bool key_up_;

  // key down pressed
  bool key_down_;

  // blind y
  float blind_y_;

};

#endif // __VIRTUAL_JOY_STICK_CLASS__