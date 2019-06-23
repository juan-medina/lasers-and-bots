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

#include "pause_window.h"
#include "../laser_and_bots_app.h"
#include "../scenes/game_scene.h"
#include "../ui/TextButton.h"
#include "../ui/TextToggle.h"
#include "../utils/audio/AudioHelper.h"
#include "game_ui.h"

pause_window::pause_window()
  : audio_helper_(nullptr)
  , current_text_button_y_(0)
  , current_image_button_x_(0)
  , toggle_music_item_(nullptr)
  , toggle_sfx_item_(nullptr)
{
}

pause_window* pause_window::create(AudioHelper* audio_helper)
{
  pause_window* ret = nullptr;

  do
  {
    auto object = new pause_window();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audio_helper))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  } while (false);

  return ret;
}

bool pause_window::init(AudioHelper* audio_helper)
{
  auto ret = false;

  do
  {
    const auto& size = Director::getInstance()->getVisibleSize();

    const auto dark_all = LayerColor::create(Color4B(0, 0, 0, 127));
    UTILS_BREAK_IF(dark_all == nullptr);

    addChild(dark_all, 0);
    dark_all->setPosition(-size.width / 2, -size.height / 2);

    audio_helper_ = audio_helper;
    UTILS_BREAK_IF(!base_class::init("Pause", audio_helper, 1300.f, 1800.0f, animation_type::fade));

    audio_helper_->preLoadEffect("sounds/select.mp3");

    ret = true;
  } while (false);

  return ret;
}

void pause_window::display()
{
  base_class::display();

  const auto music_muted = audio_helper_->getMusicMuted();
  const auto effects_muted = audio_helper_->getEffectsMuted();

  toggle_music_item_->setSelectedIndex(music_muted ? 0 : 1);
  toggle_sfx_item_->setSelectedIndex(effects_muted ? 0 : 1);
}

void pause_window::hide()
{
  base_class::hide();
}

void pause_window::on_music_toggle()
{
  const auto music_muted = toggle_music_item_->getSelectedIndex() == 0;
  const auto ui = dynamic_cast<game_ui*>(getParent());
  const auto game = dynamic_cast<game_scene*>(ui->getParent());
  auto app = dynamic_cast<laser_and_bots_app*>(game->getApplication());

  app->set_music_muted(music_muted);
  audio_helper_->playEffect("sounds/select.mp3");
}

void pause_window::on_sfx_toggle()
{
  const auto sfx_muted = toggle_sfx_item_->getSelectedIndex() == 0;
  const auto ui = dynamic_cast<game_ui*>(getParent());
  const auto game = dynamic_cast<game_scene*>(ui->getParent());
  auto app = dynamic_cast<laser_and_bots_app*>(game->getApplication());

  app->set_effects_muted(sfx_muted);
  audio_helper_->playEffect("sounds/select.mp3");
}

void pause_window::on_resume()
{
  auto ui = dynamic_cast<game_ui*>(getParent());
  ui->on_pause(this);
}

void pause_window::on_reload()
{
  auto ui = dynamic_cast<game_ui*>(getParent());
  ui->on_reload(this);
}

void pause_window::on_exit()
{
  auto ui = dynamic_cast<game_ui*>(getParent());
  ui->on_close(this);
}

bool pause_window::create_menu_items()
{
  auto result = false;

  do
  {
    UTILS_BREAK_IF(add_text_button("Exit", CC_CALLBACK_0(pause_window::on_exit, this)) == nullptr);

    const auto resume_button = add_text_button("Resume", CC_CALLBACK_0(pause_window::on_resume, this));
    UTILS_BREAK_IF(resume_button == nullptr);

    const auto restart_button = add_text_button("Restart", CC_CALLBACK_0(pause_window::on_reload, this));
    UTILS_BREAK_IF(restart_button == nullptr);

    toggle_sfx_item_ =
      add_toggle_image_button("13_sound_0", CC_CALLBACK_0(pause_window::on_sfx_toggle, this));
    UTILS_BREAK_IF(toggle_sfx_item_ == nullptr);

    const auto sfx_position =
      restart_button->getPosition() +
      Vec2(-(restart_button->getContentSize().width / 2) + toggle_sfx_item_->getContentSize().width / 2,
           restart_button->getContentSize().height + 150.f);

    toggle_sfx_item_->setPosition(sfx_position);

    toggle_music_item_ =
      add_toggle_image_button("12_music_0", CC_CALLBACK_0(pause_window::on_music_toggle, this));
    UTILS_BREAK_IF(toggle_music_item_ == nullptr);

    const auto music_position =
      Vec2(restart_button->getPosition().x + (restart_button->getContentSize().width / 2) -
             toggle_music_item_->getContentSize().width / 2,
           sfx_position.y);

    toggle_music_item_->setPosition(music_position);

    set_default_menu_item(resume_button);

    result = true;
  } while (false);

  return result;
}
