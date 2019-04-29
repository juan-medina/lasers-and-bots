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
#ifndef __MAIN_SCENE__
#define __MAIN_SCENE__

#include "../utils/utils.h"
#include "../utils/base/scene/physics_tiled_scene.h"

//foward declarations
class game_object;
class robot_object;
class switch_object;
class door_object;
class game_ui;

class game_scene final : public physics_tiled_scene
{
public:
  // base_class
  using base_class = physics_tiled_scene;

  // constructor
  game_scene();

  ~game_scene();

  // create the object
  static game_scene* create();

  // create the scene
  static Scene* scene();

  // init this object
  bool init() override;

  static constexpr short int bit_mask_world = 1;
  static constexpr short int bit_mask_robot = 2;
  static constexpr short int bit_mask_spikes = 4;
  static constexpr short int bit_mask_saw = 8;
  static constexpr short int bit_mask_acid = 16;
  static constexpr short int bit_mask_door = 32;
  static constexpr short int bit_mask_switch = 64;
  static constexpr short int bit_mask_blocks = 128;
  static constexpr short int bit_mask_barrel = 256;
  static constexpr short int bit_mask_box = 512;
  static constexpr short int bit_mask_feet = 1024;
  static constexpr short int bit_mask_step_objects = bit_mask_blocks | bit_mask_spikes | bit_mask_acid | bit_mask_saw |
    bit_mask_barrel | bit_mask_box;

  // pause our game
  void pause() override;

  // resume game
  void resume() override;

  // toggle pause
  void toggle_pause();

  // reload the level
  void reload();

  // enter scene
  virtual void onEnter() override;

protected:

  // provide a physics node for a titled gid
  virtual Node* provide_physics_node(const int gid) const override;

private:

  // update our game
  void update(float delta) override;

  // move the camera following the robot clamping on the map
  void update_camera(const float delta);

  // handle switch
  void handle_switch(switch_object* switch_game_object);

  // handle switch
  void handle_door(door_object* door_game_object);

  // check contact from objects
  bool on_contact_begin(PhysicsContact& contact);

  // get an object from a contact
  template <class Type>
  Type* get_object_from_contact(PhysicsContact& contact, unsigned short category);

  //our robot
  robot_object* robot_;

  // get the object center position
  static Vec2 get_object_center_position(const ValueMap& values);

  // get the object position
  static Vec2 get_object_position(const ValueMap& values);

  // add a laser
  bool add_laser(const ValueMap& values, Node* layer);

  // add a robot
  bool add_robot(const ValueMap& values, Node* layer);

  // add switch scenery item
  bool add_switch(const ValueMap& values, Node* layer);

  // add door scenery item
  bool add_door(const ValueMap& values, Node* layer);

  // add barrel scenery item
  bool add_barrel(const ValueMap& values, Node* layer);

  // add saw to the map
  bool add_saw(const ValueMap& values, Node* layer);

  // add a box to the map
  bool add_box(const ValueMap& values, Node* layer);

  // add an object to the map
  bool add_object(const vector<Value>::value_type& object);

  // add lasers to the game
  bool add_objects_to_game();

  // our game gravity
  static constexpr float gravity = -5000.0f;

  // calculate how many stars we got
  unsigned short int calculate_stars() const;

  // cache the robot explosion
  bool cache_robot_explosion();

  // make our robot to explode
  void explode_robot();

  // the game is over
  void game_over(const bool win);

  // when to start
  void delay_start();

  // count a number
  void count(Ref* sender, int value);

  // start the game
  void start();

  // the game ui
  game_ui* game_ui_;

  // game objects
  std::map<std::string, game_object *> game_objects_;

  // last position for the robot
  Vec2 last_robot_position_;

  // last position for the camera
  Vec2 last_camera_position_;

  // min camera pos
  Vec2 min_camera_pos_;

  // max camera pos
  Vec2 max_camera_pos_;

  // game is paused
  bool paused_;

  // final animation
  bool final_anim_;

  // the total time
  float total_time_;

  // count barrels
  unsigned short int barrel_count_;

  // the time limit
  unsigned int time_limit_;

  // the level name
  std::string level_name_;

  // start with delay
  bool delay_start_;

  // robot_fragments
  std::vector<Sprite*> robot_fragments_;
};

#endif // __MAIN_SCENE__
