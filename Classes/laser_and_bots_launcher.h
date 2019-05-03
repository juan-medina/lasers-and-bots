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

#ifndef __LAUNCHER_H__
#define __LAUNCHER_H__

#include "utils/base/app/launcher_app.h"

class laser_and_bots_launcher final : public launcher_app
{
public:

  using base_class = launcher_app;

  laser_and_bots_launcher();

  ~laser_and_bots_launcher();

  int get_screen_width() const
  {
    return screen_width_;
  }

  int get_screen_height() const
  {
    return screen_height_;
  }

  bool is_full_screen() const
  {
    return full_screen_;
  }

  bool is_effects_muted() const
  {
    return effects_muted_;
  }

  bool is_music_muted() const
  {
    return music_muted_;
  }

  bool is_debug_grid() const
  {
    return debug_grid_;
  }

  bool is_debug_physics() const
  {
    return debug_physics_;
  }

  void set_screen_width(const int screen_width)
  {
    screen_width_ = screen_width;
  }

  void set_screen_height(const int screen_height)
  {
    screen_height_ = screen_height;
  }

  void set_full_screen(const bool full_screen)
  {
    full_screen_ = full_screen;
  }

  void set_effects_muted(const bool effects_muted)
  {
    effects_muted_ = effects_muted;
  }

  void set_music_muted(const bool music_muted)
  {
    music_muted_ = music_muted;
  }

  void set_debug_grid(const bool debug_grid)
  {
    debug_grid_ = debug_grid;
  }

  void set_debug_physics(const bool debug_physics)
  {
    debug_physics_ = debug_physics;
  }

protected:

  virtual Scene* init_scene() override;

private:

  int screen_width_;
  int screen_height_;
  bool full_screen_;
  bool effects_muted_;
  bool music_muted_;
  bool debug_grid_;
  bool debug_physics_;
};

#endif // __LAUNCHER_H__
