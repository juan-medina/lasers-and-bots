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
#include "../utils/physics/physics_game_object.h"
#include "../utils/audio/audio_helper.h"

class virtual_joy_stick;

class robot_object final : public physics_game_object
{
public:
  using base_class = physics_game_object;

  robot_object();

  static robot_object* create(physics_shape_cache* physics_shape_cache, audio_helper* audio_helper,
                              virtual_joy_stick* virtual_joy_stick, const int max_shield);

  bool init(physics_shape_cache* physics_shape_cache, audio_helper* audio_helper, virtual_joy_stick* virtual_joy_stick,
            const int max_shield);

  void update(float delta) override;

  void on_land_on_block();

  void feet_touch_walk_object_start();
  void feet_touch_walk_object_end();

  float get_shield_percentage() const;

  void damage_shield(const int amount);

  void start_periodic_damage(const int amount);
  void stop_periodic_damage(const int amount);

  void pause() override;
  void resume() override;

private:

  enum state
  {
    e_falling,
    e_jumping,
    e_idle,
    e_running
  };

  void move_to_left(bool to_left);
  void move_to_right(bool to_right);
  void jump(bool to_jump);

  void walk_sound(const bool active);

  state decide_state() const;
  void change_state(state wanted_state);

  void move_robot() const;

  // our normal movement
  static const Vec2 normal_movement;
  static const int blink_on_damage_action_tag;

  bool to_left_;
  bool to_right_;
  bool jumping_;

  int walk_sound_;

  int current_shield_;
  int max_shield_;

  int periodic_damage_;

  bool feet_touch_anything_;
  int feet_touching_count_;

  state current_state_;
  virtual_joy_stick* virtual_joy_stick_;
  audio_helper* audio_helper_;
};

#endif // __ROBOT_CLASS__
