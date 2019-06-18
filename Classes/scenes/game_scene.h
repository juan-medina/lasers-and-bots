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
#ifndef __MAIN_SCENE__
#define __MAIN_SCENE__

#include "../utils/physics/physics_tiled_scene.h"

// forward declarations
class game_object;
class robot_object;
class switch_object;
class door_object;
class game_ui;
class robot_fragment;
class harm_object;
class level_manager;
class background_layer;
class custom_draw_node;

class game_scene final : public physics_tiled_scene
{
public:
  using base_class = physics_tiled_scene;

  game_scene() noexcept;

  static Scene* scene(basic_app* application, const bool debug_grid, const bool debug_physics,
                      const int level);

  static game_scene* create(basic_app* application, const bool debug_grid, const bool debug_physics,
                            const int level);

  bool init(basic_app* application, const bool debug_grid, const bool debug_physics, unsigned short level);

  bool create_game_ui();

  void calculate_camera_bounds();

  bool create_physics_contacts_callback();

  void set_map_bounds_contacts_settings() const;

  void pre_load_sounds() const;

  void get_level_settings();

  static void cache_objects_textures();

  enum class categories : unsigned short
  {
    world = 1,
    robot = 2,
    spikes = 4,
    saw = 8,
    acid = 16,
    door = 32,
    switches = 64,
    blocks = 128,
    barrel = 256,
    box = 512,
    feet = 1024,
    harm = spikes | acid | saw,
    walk_on = harm | blocks | barrel | box
  };

  void pause() override;

  void resume() override;

  void toggle_pause();

  void reload();

  void continue_button();

  // enter scene
  virtual void onEnter() override;

  void update_ui_position(const Vec2& final_pos) const;

  void update_background_position(const Vec2& final_pos) const;

  void close();

  bool is_paused() const noexcept { return paused_; }

  void will_enter_foreground() override;

private:
  // provide a physics node for a titled gid
  virtual Node* provide_physics_node(const int gid) override;

  void update_game_time(const float delta);

  bool update_robot_shield_and_check_if_depleted() const;

  void check_robot_movement(const float delta);

  bool constexpr do_we_need_game_updates() const { return !(paused_ || doing_final_anim_); }

  void update(float delta) override;

  // move the camera following the robot clamping on the map
  void camera_follow_robot(const Vec2& robot_position, const float delta);

  static void switch_activate_door(door_object* door);
  void switch_activate_switch(switch_object* switch_object);
  void switch_activate_target(game_object* target);

  bool is_switch_targeting_a_switch(switch_object* switch_object);

  void robot_touch_switch(switch_object* switch_object);
  void robot_touch_door(door_object* door_game_object);
  void robot_touch_harm_object_start(harm_object* harm_object) const;
  void robot_touch_harm_object_end(harm_object* harm_object) const;
  void robot_touch_object_start(const PhysicsContact& contact);
  void robot_touch_object_end(const PhysicsContact& contact) const;

  void feet_touch_object_start(const PhysicsContact& contact) const;
  void feet_touch_object_end(const PhysicsContact& contact) const;

  // two shapes start to contact
  bool on_contact_begin(PhysicsContact& contact);

  // two shapes stop to contact
  void on_contact_separate(PhysicsContact& contact) const;

  // get an game object from a contact
  template <class Type>
  static Type* get_object_from_contact(const PhysicsContact& contact, const categories category);

  bool add_laser(const ValueMap& values, Node* layer);
  bool add_robot(const ValueMap& values, Node* layer);
  bool add_switch(const ValueMap& values, Node* layer);
  bool add_door(const ValueMap& values, Node* layer);
  bool add_barrel(const ValueMap& values, Node* layer);
  bool add_saw(const ValueMap& values, Node* layer);
  bool add_box(const ValueMap& values, Node* layer);
  bool add_light(const ValueMap& values) const;

  bool add_object(const vector<Value>::value_type& object);
  bool add_objects_to_game();

  static constexpr float gravity = -5000.0f;

  unsigned short int calculate_stars() const;

  void move_fragments_to_robot();

  bool cache_robot_explosion();

  void explode_robot();

  void game_over(const bool win);

  void delay_start();

  void set_countdown_number_in_ui(Ref* sender, const int value) const;

  void start();

  robot_object* robot_;
  game_ui* game_ui_;
  std::map<std::string, game_object*> game_objects_;
  std::vector<robot_fragment*> robot_fragments_;

  Vec2 last_robot_position_;
  Vec2 last_camera_position_;

  Vec2 min_camera_pos_;
  Vec2 max_camera_pos_;

  bool paused_;
  bool doing_final_anim_;
  bool doing_delay_start_;
  bool closing_;

  float total_time_;
  unsigned int time_limit_;
  std::string level_name_;

  unsigned short int barrel_count_;
  unsigned short int level_;

  level_manager* level_manager_;
  std::string music_file_name_;
  background_layer* background_;
  custom_draw_node* lights_;

  static constexpr float light_distance = 1500.f;
};

#endif // __MAIN_SCENE__
