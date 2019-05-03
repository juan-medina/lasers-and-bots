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

#include "laser_and_bots_app.h"
#include "scenes/loading_scene.h"
#include "utils/audio/audio_helper.h"

laser_and_bots_app::laser_and_bots_app()
  : laser_and_bots_app(0, 0, true)
{
}

laser_and_bots_app::laser_and_bots_app(const int screen_width, const int screen_height, const bool full_screen)
  : base_class("Lasers and Bots", 1920 * 4, 1080 * 4,
               screen_width, screen_height, full_screen, false, false),
    effects_muted_(false),
    music_muted_(false),
    debug_grid_(false),
    debug_physics_(false)
{
  effects_muted_ = UserDefault::getInstance()->getBoolForKey("effects_muted", effects_muted_);
  music_muted_ = UserDefault::getInstance()->getBoolForKey("music_muted", music_muted_);
  debug_grid_ = UserDefault::getInstance()->getBoolForKey("debug_grid", debug_grid_);
  debug_physics_ = UserDefault::getInstance()->getBoolForKey("debug_physics", debug_physics_);

  audio_helper::get_instance()->set_effects_muted(effects_muted_);
  audio_helper::get_instance()->set_music_muted(music_muted_);
}

Scene* laser_and_bots_app::init_scene()
{
  return loading_scene::game(this, debug_grid_, debug_physics_);
}

void laser_and_bots_app::set_effects_muted(const bool effects_muted)
{
  effects_muted_ = effects_muted;
  UserDefault::getInstance()->setBoolForKey("effects_muted", effects_muted_);
  audio_helper::get_instance()->set_effects_muted(effects_muted_);
}

void laser_and_bots_app::set_music_muted(const bool music_muted)
{
  music_muted_ = music_muted;
  UserDefault::getInstance()->setBoolForKey("music_muted", music_muted_);
  audio_helper::get_instance()->set_music_muted(music_muted_);
}

void laser_and_bots_app::to_game()
{
  Director::getInstance()->replaceScene(init_scene());
}
