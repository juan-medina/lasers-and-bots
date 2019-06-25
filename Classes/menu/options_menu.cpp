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

#include "options_menu.h"
#include "../scenes/menu_scene.h"
#include "../ui/SliderObject.h"
#include "../ui/TextButton.h"
#include "../ui/TextToggle.h"
#include "../utils/audio/AudioHelper.h"

options_menu::options_menu()
  : desktop_application_(false)
  , back_item_(nullptr)
  , sound_toggle_(nullptr)
  , music_toggle_(nullptr)
  , sound_slider_(nullptr)
  , music_slider_(nullptr)
  , full_screen_toggle_(nullptr)
  , windowed_toggle_(nullptr)
  , debug_grid_toggle_(nullptr)
  , debug_physics_toggle_(nullptr)
{
}

options_menu* options_menu::create(AudioHelper* audio_helper, const bool is_desktop_application)
{
  options_menu* ret = nullptr;

  do
  {
    auto object = new options_menu();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audio_helper, is_desktop_application))
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

bool options_menu::init(AudioHelper* audio_helper, const bool is_desktop_application)
{
  auto ret = false;

  do
  {
    desktop_application_ = is_desktop_application;
    const auto width = 3400.f;
    auto height = 2400.f;
    if (desktop_application_)
    {
      height += 400.f;
    }
    UTILS_BREAK_IF(!base_class::init("Options", audio_helper, width, height));

    ret = true;
  } while (false);

  return ret;
}

void options_menu::display()
{
  base_class::display();

  const auto helper = getAudioHelper();

  music_toggle_->setSelectedIndex(helper->getMusicMuted() ? 0 : 1);
  sound_toggle_->setSelectedIndex(helper->getEffectsMuted() ? 0 : 1);

  music_slider_->setPercentage(helper->getMusicVolume() * 100.f);
  music_slider_->enable(!helper->getMusicMuted());

  sound_slider_->setPercentage(helper->getSoundVolume() * 100.f);
  sound_slider_->enable(!helper->getEffectsMuted());

  if (desktop_application_)
  {
    const auto menu = dynamic_cast<menu_scene*>(getParent());
    full_screen_toggle_->setSelectedIndex(menu->is_full_screen() ? 1 : 0);
    windowed_toggle_->setSelectedIndex(menu->is_full_screen() ? 0 : 1);
  }

  const auto menu = dynamic_cast<menu_scene*>(getParent());

  debug_physics_toggle_->setSelectedIndex(menu->is_debug_physics() ? 1 : 0);
  debug_grid_toggle_->setSelectedIndex(menu->is_debug_grid() ? 1 : 0);

  update_labels();
}

bool options_menu::createMenuItems()
{
  auto result = false;
  do
  {
    static auto const labels_starts = 800.0f;
    back_item_ = addTextButton("Back", CC_CALLBACK_0(options_menu::on_back, this));
    UTILS_BREAK_IF(back_item_ == nullptr);

    debug_grid_toggle_ = addToggleTextButton("Enabled", CC_CALLBACK_0(options_menu::on_debug_grid, this));
    UTILS_BREAK_IF(debug_grid_toggle_ == nullptr);
    debug_grid_toggle_->setPosition(Vec2(-400, debug_grid_toggle_->getPosition().y));
    UTILS_BREAK_IF(addRowLabel("Debug Grid", debug_grid_toggle_, labels_starts) == nullptr);

    debug_physics_toggle_ =
      addToggleTextButton("Enabled", CC_CALLBACK_0(options_menu::on_debug_physics, this));
    UTILS_BREAK_IF(debug_physics_toggle_ == nullptr);
    debug_physics_toggle_->setPosition(
      Vec2(debug_grid_toggle_->getPosition().x, debug_physics_toggle_->getPosition().y));
    UTILS_BREAK_IF(addRowLabel("Debug Physics", debug_physics_toggle_, labels_starts) == nullptr);

    if (desktop_application_)
    {
      full_screen_toggle_ =
        addToggleTextButton("Full Screen", CC_CALLBACK_0(options_menu::on_full_screen, this));
      UTILS_BREAK_IF(full_screen_toggle_ == nullptr);

      full_screen_toggle_->setPosition(
        Vec2(debug_grid_toggle_->getPosition().x, full_screen_toggle_->getPosition().y));

      UTILS_BREAK_IF(addRowLabel("Video Mode", full_screen_toggle_, labels_starts) == nullptr);

      windowed_toggle_ =
        addToggleTextButton("Windowed", CC_CALLBACK_0(options_menu::on_windowed, this), true);
      UTILS_BREAK_IF(windowed_toggle_ == nullptr);

      windowed_toggle_->setPosition(full_screen_toggle_->getPosition() +
                                    Vec2(full_screen_toggle_->getContentSize().width + 125, 0.f));
    }

    sound_toggle_ = addToggleTextButton("Enabled", CC_CALLBACK_0(options_menu::on_sound, this));
    UTILS_BREAK_IF(sound_toggle_ == nullptr);

    sound_toggle_->setPosition(Vec2(debug_grid_toggle_->getPosition().x, sound_toggle_->getPosition().y));
    UTILS_BREAK_IF(addRowLabel("Sound", sound_toggle_, labels_starts) == nullptr);

    sound_slider_ = addSlider(sound_toggle_, CC_CALLBACK_1(options_menu::on_sound_slider_change, this));
    UTILS_BREAK_IF(sound_slider_ == nullptr);

    music_toggle_ = addToggleTextButton("Enabled", CC_CALLBACK_0(options_menu::on_music, this));
    UTILS_BREAK_IF(music_toggle_ == nullptr);

    music_toggle_->setPosition(sound_toggle_->getPosition().x, music_toggle_->getPosition().y);
    UTILS_BREAK_IF(addRowLabel("Music", music_toggle_, labels_starts) == nullptr);

    music_slider_ = addSlider(music_toggle_, CC_CALLBACK_1(options_menu::on_music_slider_change, this));
    UTILS_BREAK_IF(music_slider_ == nullptr);

    setDefaultMenuItem(back_item_);

    result = true;
  } while (false);
  return result;
}

void options_menu::on_back()
{
  getAudioHelper()->playEffect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_main_menu();
}

void options_menu::on_music()
{
  getAudioHelper()->playEffect("sounds/select.mp3");

  const auto menu = dynamic_cast<menu_scene*>(getParent());
  const auto disable = music_toggle_->getSelectedIndex() == 0;
  music_slider_->enable(!disable);
  menu->change_music(disable);
  update_labels();
}

void options_menu::on_sound()
{
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  const auto disable = sound_toggle_->getSelectedIndex() == 0;
  sound_slider_->enable(!disable);
  menu->change_sound(disable);

  getAudioHelper()->playEffect("sounds/select.mp3");
  update_labels();
}

void options_menu::on_music_slider_change(const float percentage)
{
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->change_music_volume(percentage / 100);
}

void options_menu::on_sound_slider_change(const float percentage)
{
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->change_sound_volume(percentage / 100);
}

void options_menu::on_full_screen()
{
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  if (menu->is_full_screen())
  {
    full_screen_toggle_->setSelectedIndex(1);
    return;
  }
  getAudioHelper()->playEffect("sounds/select.mp3");

  const auto full_screen = full_screen_toggle_->getSelectedIndex() == 1;
  windowed_toggle_->setSelectedIndex(full_screen ? 0 : 1);
  menu->change_application_video_mode(full_screen);
}

void options_menu::on_windowed()
{
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  if (!menu->is_full_screen())
  {
    windowed_toggle_->setSelectedIndex(1);
    return;
  }

  getAudioHelper()->playEffect("sounds/select.mp3");

  const auto full_screen = windowed_toggle_->getSelectedIndex() == 0;
  full_screen_toggle_->setSelectedIndex(full_screen ? 1 : 0);
  menu->change_application_video_mode(full_screen);
}

void options_menu::on_debug_grid()
{
  getAudioHelper()->playEffect("sounds/select.mp3");

  const auto menu = dynamic_cast<menu_scene*>(getParent());
  const auto debug = debug_grid_toggle_->getSelectedIndex() == 1;
  menu->set_debug_grid(debug);

  update_labels();
}

void options_menu::on_debug_physics()
{
  getAudioHelper()->playEffect("sounds/select.mp3");

  const auto menu = dynamic_cast<menu_scene*>(getParent());
  const auto debug = debug_physics_toggle_->getSelectedIndex() == 1;
  menu->set_debug_physics(debug);

  update_labels();
}

void options_menu::update_labels()
{
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  const auto helper = getAudioHelper();

  sound_toggle_->setText(helper->getEffectsMuted() ? "Disabled" : "Enabled");
  music_toggle_->setText(helper->getMusicMuted() ? "Disabled" : "Enabled");
  debug_physics_toggle_->setText(menu->is_debug_physics() ? "Enabled" : "Disabled");
  debug_grid_toggle_->setText(menu->is_debug_grid() ? "Enabled" : "Disabled");
}
