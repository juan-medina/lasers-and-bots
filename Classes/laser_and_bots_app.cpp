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

#include "laser_and_bots_app.h"
#include "misc/level_manager.h"
#include "scenes/loading_scene.h"
#include "scenes/menu_scene.h"
#include "utils/audio/AudioHelper.h"

laser_and_bots_app::laser_and_bots_app()
  : base_class("Lasers and Bots", 1920 * 4, 1080 * 4)
  , effects_muted_(false)
  , music_muted_(false)
  , debug_grid_(false)
  , debug_physics_(false)
  , music_volume_(1.f)
  , effects_volume_(1.f)
  , level_manager_(nullptr)
  , want_restart_(false)
  , to_options_(false)
{
#if (GAME_PLATFORM == DESKTOP_GAME)
  if (isDesktop())
  {
    const auto full_screen = UserDefault::getInstance()->getBoolForKey("full_screen", false);
    setFullScreen(full_screen);

    if (!full_screen)
    {
      const auto window_size = UserDefault::getInstance()->getFloatForKey("window_size", 0.75f);
      setWindowSize(window_size);
    }
  }
#endif
}

laser_and_bots_app::~laser_and_bots_app()
{
  delete level_manager_;
}

Scene* laser_and_bots_app::initScene()
{
  return to_options_ ? options_menu_scene() : license_scene();
}

Scene* laser_and_bots_app::game_scene(const unsigned short int level)
{
  effects_muted_ = UserDefault::getInstance()->getBoolForKey("effects_muted", effects_muted_);
  music_muted_ = UserDefault::getInstance()->getBoolForKey("music_muted", music_muted_);
  debug_grid_ = UserDefault::getInstance()->getBoolForKey("debug_grid", debug_grid_);
  debug_physics_ = UserDefault::getInstance()->getBoolForKey("debug_physics", debug_physics_);
  music_volume_ = UserDefault::getInstance()->getFloatForKey("music_volume", music_volume_);
  effects_volume_ = UserDefault::getInstance()->getFloatForKey("effects_volume", effects_volume_);

  getAudioHelper()->setEffectsMuted(effects_muted_);
  getAudioHelper()->setMusicMuted(music_muted_);
  getAudioHelper()->setMusicVolume(music_volume_);
  getAudioHelper()->setSoundVolume(effects_volume_);
  setup_level_manager();

  return loading_scene::game(this, debug_grid_, debug_physics_, level);
}

Scene* laser_and_bots_app::main_menu_scene()
{
  effects_muted_ = UserDefault::getInstance()->getBoolForKey("effects_muted", effects_muted_);
  music_muted_ = UserDefault::getInstance()->getBoolForKey("music_muted", music_muted_);
  music_volume_ = UserDefault::getInstance()->getFloatForKey("music_volume", music_volume_);
  effects_volume_ = UserDefault::getInstance()->getFloatForKey("effects_volume", effects_volume_);

  getAudioHelper()->setEffectsMuted(effects_muted_);
  getAudioHelper()->setMusicMuted(music_muted_);
  getAudioHelper()->setMusicVolume(music_volume_);
  getAudioHelper()->setSoundVolume(effects_volume_);
  setup_level_manager();

  return loading_scene::menu(this, menu_to_display::main_menu, 1);
}

Scene* laser_and_bots_app::license_scene()
{
  effects_muted_ = UserDefault::getInstance()->getBoolForKey("effects_muted", effects_muted_);
  music_muted_ = UserDefault::getInstance()->getBoolForKey("music_muted", music_muted_);
  music_volume_ = UserDefault::getInstance()->getFloatForKey("music_volume", music_volume_);
  effects_volume_ = UserDefault::getInstance()->getFloatForKey("effects_volume", effects_volume_);

  getAudioHelper()->setEffectsMuted(effects_muted_);
  getAudioHelper()->setMusicMuted(music_muted_);
  getAudioHelper()->setMusicVolume(music_volume_);
  getAudioHelper()->setSoundVolume(effects_volume_);
  setup_level_manager();

  return loading_scene::menu(this, menu_to_display::license_menu, 1);
}
Scene* laser_and_bots_app::play_menu_scene(const unsigned short int selected_level)
{
  effects_muted_ = UserDefault::getInstance()->getBoolForKey("effects_muted", effects_muted_);
  music_muted_ = UserDefault::getInstance()->getBoolForKey("music_muted", music_muted_);
  music_volume_ = UserDefault::getInstance()->getFloatForKey("music_volume", music_volume_);
  effects_volume_ = UserDefault::getInstance()->getFloatForKey("effects_volume", effects_volume_);

  setup_level_manager();

  getAudioHelper()->setEffectsMuted(effects_muted_);
  getAudioHelper()->setMusicMuted(music_muted_);
  getAudioHelper()->setMusicVolume(music_volume_);
  getAudioHelper()->setSoundVolume(effects_volume_);

  return loading_scene::menu(this, menu_to_display::play_menu, selected_level);
}

Scene* laser_and_bots_app::options_menu_scene()
{
  effects_muted_ = UserDefault::getInstance()->getBoolForKey("effects_muted", effects_muted_);
  music_muted_ = UserDefault::getInstance()->getBoolForKey("music_muted", music_muted_);
  music_volume_ = UserDefault::getInstance()->getFloatForKey("music_volume", music_volume_);
  effects_volume_ = UserDefault::getInstance()->getFloatForKey("effects_volume", effects_volume_);

  setup_level_manager();

  getAudioHelper()->setEffectsMuted(effects_muted_);
  getAudioHelper()->setMusicMuted(music_muted_);
  getAudioHelper()->setMusicVolume(music_volume_);
  getAudioHelper()->setSoundVolume(effects_volume_);

  return loading_scene::menu(this, menu_to_display::options_menu, 1);
}

void laser_and_bots_app::set_effects_muted(const bool effects_muted)
{
  effects_muted_ = effects_muted;
  UserDefault::getInstance()->setBoolForKey("effects_muted", effects_muted_);
  getAudioHelper()->setEffectsMuted(effects_muted_);
}

void laser_and_bots_app::set_music_muted(const bool music_muted)
{
  music_muted_ = music_muted;
  UserDefault::getInstance()->setBoolForKey("music_muted", music_muted_);
  getAudioHelper()->setMusicMuted(music_muted_);
}

void laser_and_bots_app::set_music_volume(const float music_volume)
{
  music_volume_ = music_volume;
  UserDefault::getInstance()->setFloatForKey("music_volume", music_volume_);
  getAudioHelper()->setMusicVolume(music_volume_);
}

void laser_and_bots_app::set_effects_volume(const float effects_volume)
{
  effects_volume_ = effects_volume;
  UserDefault::getInstance()->setFloatForKey("effects_volume", effects_volume_);
  getAudioHelper()->setSoundVolume(effects_volume_);
}

void laser_and_bots_app::to_game(const unsigned short int level)
{
  Director::getInstance()->replaceScene(game_scene(level));
}

void laser_and_bots_app::to_main_menu()
{
  Director::getInstance()->replaceScene(main_menu_scene());
}

void laser_and_bots_app::to_play_menu(const unsigned short int level)
{
  Director::getInstance()->replaceScene(play_menu_scene(level));
}

void laser_and_bots_app::applicationDidEnterBackground()
{
  const auto scene = Director::getInstance()->getRunningScene();
  scene->pause();
}

void laser_and_bots_app::change_video_mode(const bool full_screen)
{
  UserDefault::getInstance()->setBoolForKey("full_screen", full_screen);
  close_with_restart();
}

void laser_and_bots_app::close_with_restart()
{
  want_restart_ = true;
  close();
}

int laser_and_bots_app::run(const bool to_options)
{
  to_options_ = to_options;
  return base_class::run();
}

void laser_and_bots_app::set_debug_grid(const bool debug_grid)
{
  debug_grid_ = debug_grid;
  UserDefault::getInstance()->setBoolForKey("debug_grid", debug_grid);
}

void laser_and_bots_app::set_debug_physics(const bool debug_physics)
{
  debug_physics_ = debug_physics;
  UserDefault::getInstance()->setBoolForKey("debug_physics", debug_physics);
}

void laser_and_bots_app::setup_level_manager()
{
  if (level_manager_ == nullptr)
  {
    level_manager_ = new level_manager();
  }

  level_manager_->init();
}
