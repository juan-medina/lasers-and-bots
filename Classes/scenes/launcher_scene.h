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

#ifndef __LAUNCHER_SCENE_H__
#define __LAUNCHER_SCENE_H__

#include "../utils/base/scene/basic_scene.h"

class launcher_scene final : public basic_scene
{
public:
  using parent = basic_scene;

  static launcher_scene* create(basic_app* application);

  static Scene* scene(basic_app* application);

  launcher_scene();

  bool init(basic_app* application);

  void play(Ref* sender) const;

private:
  void screen_mode_click(Ref* sender);

  void resolution_click(Ref* sender);

  void effects_muted_click(Ref* sender);

  void music_muted_click(Ref* sender);

  void debug_grid_click(Ref* sender);

  void debug_physics_click(Ref* sender);

  void update_labels() const;

  bool full_screen_;
  Label* screen_mode_label_;

  int screen_width_;
  int screen_height_;
  Label* resolution_label_;

  bool debug_grid_;
  Label* debug_grid_label_;

  bool debug_physics_;
  Label* debug_physics_label_;

  bool effects_muted_;
  Label* effects_muted_label_;

  bool music_muted_;
  Label* music_muted_label_;

  void load_settings();
  void save_settings() const;
};

#endif // __LAUNCHER_SCENE_H__
