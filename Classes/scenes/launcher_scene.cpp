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

#include "launcher_scene.h"
#include "../laser_and_bots_launcher.h"

launcher_scene::launcher_scene()
  : full_screen_(false),
    screen_mode_label_(nullptr),
    screen_width_(1920),
    screen_height_(1080),
    resolution_label_(nullptr),
    debug_grid_(false),
    debug_grid_label_(nullptr),
    debug_physics_(false),
    debug_physics_label_(nullptr),
    effects_muted_(false),
    effects_muted_label_(nullptr),
    music_muted_(false),
    music_muted_label_(nullptr)
{
}

launcher_scene* launcher_scene::create(basic_app* application)
{
  auto scene = new launcher_scene();

  if (scene)
  {
    if (scene->init(application))
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }
  }

  return scene;
}

Scene* launcher_scene::scene(basic_app* application)
{
  auto scene = new launcher_scene();

  if (scene)
  {
    if (scene->init(application))
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }
  }

  return scene;
}

bool launcher_scene::init(basic_app* application)
{
  auto ret = false;

  do
  {
    ret = parent::init(application);
    UTILS_BREAK_IF(!ret);

    load_settings();

    auto title_font = Label::createWithTTF("Lasers and Bots : Launcher", "fonts/tahoma.ttf", 40);
    UTILS_BREAK_IF(!title_font);

    title_font->setPosition(Vec2(screen_size_.width / 2, screen_size_.height - 50));
    addChild(title_font);

    screen_mode_label_ = Label::createWithTTF("Screen Mode: Windowed", "fonts/tahoma.ttf", 20);
    UTILS_BREAK_IF(!screen_mode_label_);

    auto screen_mode_item = MenuItemLabel::create(screen_mode_label_,
                                                  CC_CALLBACK_1(launcher_scene::screen_mode_click, this));
    UTILS_BREAK_IF(!screen_mode_item);

    screen_mode_item->setPosition(Vec2(screen_size_.width / 2, screen_size_.height - 100));

    resolution_label_ = Label::createWithTTF("Size : 9999 x 9999", "fonts/tahoma.ttf", 20);
    UTILS_BREAK_IF(!resolution_label_);

    auto resolution_item = MenuItemLabel::create(resolution_label_,
                                                 CC_CALLBACK_1(launcher_scene::resolution_click, this));
    UTILS_BREAK_IF(!resolution_item);

    resolution_item->setPosition(Vec2(screen_size_.width / 2, screen_size_.height - 150));

    effects_muted_label_ = Label::createWithTTF("Sfx Sound: true ", "fonts/tahoma.ttf", 20);
    UTILS_BREAK_IF(!effects_muted_label_);

    auto effects_muted_item = MenuItemLabel::create(effects_muted_label_,
                                                    CC_CALLBACK_1(launcher_scene::effects_muted_click, this));
    UTILS_BREAK_IF(!effects_muted_item);

    effects_muted_item->setPosition(Vec2(screen_size_.width / 2, screen_size_.height - 200));

    music_muted_label_ = Label::createWithTTF("Music: true ", "fonts/tahoma.ttf", 20);
    UTILS_BREAK_IF(!music_muted_label_);

    auto music_muted_item = MenuItemLabel::create(music_muted_label_,
                                                  CC_CALLBACK_1(launcher_scene::music_muted_click, this));
    UTILS_BREAK_IF(!music_muted_item);

    music_muted_item->setPosition(Vec2(screen_size_.width / 2, screen_size_.height - 250));

    debug_grid_label_ = Label::createWithTTF("Debug Grid: true ", "fonts/tahoma.ttf", 20);
    UTILS_BREAK_IF(!debug_grid_label_);

    auto debug_grid_item = MenuItemLabel::create(debug_grid_label_,
                                                 CC_CALLBACK_1(launcher_scene::debug_grid_click, this));
    UTILS_BREAK_IF(!debug_grid_item);

    debug_grid_item->setPosition(Vec2(screen_size_.width / 2, screen_size_.height - 300));

    debug_physics_label_ = Label::createWithTTF("Debug Physics: true ", "fonts/tahoma.ttf", 20);
    UTILS_BREAK_IF(!debug_physics_label_);

    auto debug_physics_item = MenuItemLabel::create(debug_physics_label_,
                                                    CC_CALLBACK_1(launcher_scene::debug_physics_click, this));
    UTILS_BREAK_IF(!debug_physics_item);

    debug_physics_item->setPosition(Vec2(screen_size_.width / 2, screen_size_.height - 350));

    const auto play_label = Label::createWithTTF("PLAY!", "fonts/tahoma.ttf", 30);
    UTILS_BREAK_IF(!play_label);

    auto play_item = MenuItemLabel::create(play_label, CC_CALLBACK_1(launcher_scene::play, this));
    UTILS_BREAK_IF(!play_item);

    play_item->setPosition(Vec2(screen_size_.width / 2, screen_size_.height - 400));

    auto options_menu = Menu::create(screen_mode_item, resolution_item, effects_muted_item, music_muted_item,
                                     debug_grid_item, debug_physics_item, play_item, nullptr);
    UTILS_BREAK_IF(!options_menu);

    options_menu->setPosition(Point::ZERO);

    update_labels();

    addChild(options_menu, 1);

    ret = true;
  }
  while (false);

  return ret;
}

void launcher_scene::load_settings()
{
  const auto app = dynamic_cast<laser_and_bots_launcher*>(application_);

  screen_width_ = app->get_screen_width();
  screen_height_ = app->get_screen_height();
  full_screen_ = app->is_full_screen();
  debug_grid_ = app->is_debug_grid();
  debug_physics_ = app->is_debug_physics();
  effects_muted_ = app->is_effects_muted();
  music_muted_ = app->is_music_muted();
}

void launcher_scene::save_settings() const
{
  const auto app = dynamic_cast<laser_and_bots_launcher*>(application_);

  app->set_screen_width(screen_width_);
  app->set_screen_height(screen_height_);
  app->set_full_screen(full_screen_);
  app->set_debug_grid(debug_grid_);
  app->set_debug_physics(debug_physics_);
  app->set_effects_muted(effects_muted_);
  app->set_music_muted(music_muted_);
}

void launcher_scene::play(Ref* sender) const
{
  const auto app = dynamic_cast<laser_and_bots_launcher*>(application_);

  app->set_want_to_play(true);

  save_settings();

  application_->close();
}

void launcher_scene::screen_mode_click(Ref* sender)
{
  full_screen_ = !full_screen_;
  update_labels();
}

void launcher_scene::resolution_click(Ref* sender)
{
  if (!full_screen_)
  {
    if ((screen_width_ == 1920) && (screen_height_ == 1080))
    {
      screen_width_ = 1334;
      screen_height_ = 750;
    }
    else if ((screen_width_ == 1334) && (screen_height_ == 750))
    {
      screen_width_ = 1136;
      screen_height_ = 640;
    }
    else if ((screen_width_ == 1136) && (screen_height_ == 640))
    {
      screen_width_ = 1024;
      screen_height_ = 768;
    }
    else if ((screen_width_ == 1024) && (screen_height_ == 768))
    {
      screen_width_ = 960;
      screen_height_ = 640;
    }
    else if ((screen_width_ == 960) && (screen_height_ == 640))
    {
      screen_width_ = 2048;
      screen_height_ = 1536;
    }
    else
    {
      screen_width_ = 1920;
      screen_height_ = 1080;
    }
    update_labels();
  }
}


void launcher_scene::effects_muted_click(Ref* sender)
{
  effects_muted_ = !effects_muted_;
  update_labels();
}

void launcher_scene::music_muted_click(Ref* sender)
{
  music_muted_ = !music_muted_;
  update_labels();
}

void launcher_scene::debug_grid_click(Ref* sender)
{
  debug_grid_ = !debug_grid_;
  update_labels();
}

void launcher_scene::debug_physics_click(Ref* sender)
{
  debug_physics_ = !debug_physics_;
  update_labels();
}

void launcher_scene::update_labels() const
{
  if (full_screen_)
  {
    screen_mode_label_->setString("Screen Mode: Full Screen");
    resolution_label_->setVisible(false);
  }
  else
  {
    screen_mode_label_->setString("Screen Mode: Windowed");
    resolution_label_->setVisible(true);
  }

  resolution_label_->setString(string_format("Size : %d x %d", screen_width_, screen_height_));
  effects_muted_label_->setString(string_format("Sfx Sound : %s", effects_muted_ ? "false" : "true"));
  music_muted_label_->setString(string_format("Music : %s", music_muted_ ? "false" : "true"));
  debug_grid_label_->setString(string_format("Debug Grid : %s", debug_grid_ ? "true" : "false"));
  debug_physics_label_->setString(string_format("Debug Physics : %s", debug_physics_ ? "true" : "false"));
}
