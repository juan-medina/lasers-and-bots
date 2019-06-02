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

#ifndef _APP_DELEGATE_H_
#define _APP_DELEGATE_H_

#include "utils/base/app/basic_app.h"

// forward declarations
class level_manager;
class input_controller;

// this game application
class laser_and_bots_app final : public basic_app
{
public:
  using base_class = basic_app;

  laser_and_bots_app();

  ~laser_and_bots_app();

  Scene* init_scene() override;

  Scene* game_scene(const unsigned short int level);
  Scene* main_menu_scene();
  Scene* play_menu_scene(const unsigned short int selected_level);
  Scene* options_menu_scene();

  void set_effects_muted(const bool effects_muted);
  void set_music_muted(const bool music_muted);
  void set_music_volume(const float music_volume);
  void set_effects_volume(const float effects_volume);

  void to_game(const unsigned short int level);
  void to_main_menu();
  void to_play_menu(const unsigned short int level);

  void applicationDidEnterBackground() override;

  level_manager* get_level_manager() const { return level_manager_; }

  void change_video_mode(const bool full_screen);

  bool want_a_restart() const { return want_restart_; }

  void close_with_restart();

  int run(const bool to_options);

  bool is_debug_grid() const { return debug_grid_; }

  void set_debug_grid(const bool debug_grid);

  bool is_debug_physics() const { return debug_physics_; }

  void set_debug_physics(const bool debug_physics);

private:
  void setup_level_manager();

  bool effects_muted_;
  bool music_muted_;
  bool debug_grid_;
  bool debug_physics_;

  float music_volume_;
  float effects_volume_;
  level_manager* level_manager_;
  bool want_restart_;
  bool to_options_;
};

#endif // _APP_DELEGATE_H_
