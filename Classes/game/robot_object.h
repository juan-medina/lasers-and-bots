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

class virtual_joy_stick;

class robot_object final : public game_object
{
public:
  // base_class
  using base_class = game_object;

  // constructor
  robot_object();

  // create the object
  static robot_object* create(virtual_joy_stick* virtual_joy_stick, const int max_shield);

  // init this object
  bool init(virtual_joy_stick* virtual_joy_stick, const int max_shield);

  // update our robot
  void update(float delta) override;

  // land on a block
  void on_land_on_block();

  // get the current shield
  float get_shield_percentage() const;

  // damage our shield
  void damage_shield(const int amount);

  // object paused
  void pause() override;

  // object resume
  void resume() override;

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
  void jump(bool to_jump);

  // walk sound
  void walk_sound(const bool active);

  // we like to move to left
  bool to_left_;

  // we like to move to right
  bool to_right_;

  // we like to jump
  bool jumping_;

  // our normal movement
  static const Vec2 normal_movement;

  // our current state
  state current_state_;

  // calculate our state
  state decide_state() const;

  // change our state
  void change_state(state wanted_state);

  // move the robot
  void move_robot() const;

  // the joystick
  virtual_joy_stick* virtual_joy_stick_;

  // our repeatable walk sound
  int walk_sound_;

  // our current shield
  int current_shield_;

  // our max shield
  int max_shield_;

  // tag for our blink action
  static const int blink_on_damage_action_tag;

};

#endif // __ROBOT_CLASS__
