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

#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "../utils/base/scene/basic_scene.h"

// forward declarations
class main_menu;
class options_menu;
class play_menu;

enum class menu_to_display
{
  main_menu,
  play_menu,
  options_menu
};

class menu_scene final : public basic_scene
{
public:
  using base_class = basic_scene;

  menu_scene();

  ~menu_scene();

  static Scene* scene(basic_app* application, const menu_to_display menu);

  void go_to_game(const int level);
  void exit_app();
  void display_options_menu() const;
  void display_main_menu() const;
  void display_play_menu() const;
  void change_music(const bool disabled) const;
  void change_sound(const bool disabled) const;
  void change_music_volume(const float volume) const;
  void change_sound_volume(const float volume) const;

  void update(float delta) override;

  void pause() override;

  void resume() override;

  void did_enter_background() override;
  void will_enter_foreground() override;

  bool is_full_screen() const;

  void change_application_video_mode(const bool full_screen) const;

private:

  void delay_to_game() const;
  bool init(basic_app* application, const menu_to_display menu);

  bool add_background();
  bool add_robot();
  bool add_laser();

  main_menu* main_menu_;
  options_menu* options_menu_;
  play_menu* play_menu_;

  Node* background_;
  bool paused_;
  int saved_level_;
};

#endif // __MENU_SCENE_H__
