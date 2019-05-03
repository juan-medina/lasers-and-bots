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

#include "laser_and_bots_launcher.h"
#include "scenes/launcher_scene.h"

laser_and_bots_launcher::laser_and_bots_launcher():
  base_class("Lasers and Bots : Launcher", 500, 450),
  screen_width_(1920),
  screen_height_(1080),
  full_screen_(false),
  effects_muted_(false),
  music_muted_(false),
  debug_grid_(false),
  debug_physics_(false)
{
  full_screen_ = UserDefault::getInstance()->getBoolForKey("full_screen", full_screen_);
  screen_width_ = UserDefault::getInstance()->getIntegerForKey("screen_width", screen_width_);
  screen_height_ = UserDefault::getInstance()->getIntegerForKey("screen_height", screen_height_);
  effects_muted_ = UserDefault::getInstance()->getBoolForKey("effects_muted", effects_muted_);
  music_muted_ = UserDefault::getInstance()->getBoolForKey("music_muted", music_muted_);
  debug_grid_ = UserDefault::getInstance()->getBoolForKey("debug_grid", debug_grid_);
  debug_physics_ = UserDefault::getInstance()->getBoolForKey("debug_physics", debug_physics_);
}

laser_and_bots_launcher::~laser_and_bots_launcher()
{
  UserDefault::getInstance()->setBoolForKey("full_screen", full_screen_);
  UserDefault::getInstance()->setIntegerForKey("screen_width", screen_width_);
  UserDefault::getInstance()->setIntegerForKey("screen_height", screen_height_);
  UserDefault::getInstance()->setBoolForKey("effects_muted", effects_muted_);
  UserDefault::getInstance()->setBoolForKey("music_muted", music_muted_);
  UserDefault::getInstance()->setBoolForKey("debug_grid", debug_grid_);
  UserDefault::getInstance()->setBoolForKey("debug_physics", debug_physics_);
}

Scene* laser_and_bots_launcher::init_scene()
{
  return launcher_scene::scene(this);
}
