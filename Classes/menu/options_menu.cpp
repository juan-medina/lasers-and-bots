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

#include "options_menu.h"
#include "../utils/audio/audio_helper.h"
#include "../scenes/menu_scene.h"
#include "../ui/slider_object.h"

options_menu::options_menu():
  sound_toggle_(nullptr),
  music_toggle_(nullptr),
  sound_slider_(nullptr),
  music_slider_(nullptr)
{
}

options_menu* options_menu::create(audio_helper* audio_helper)
{
  options_menu* ret = nullptr;

  do
  {
    auto object = new options_menu();
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
  }
  while (false);

  return ret;
}

bool options_menu::init(audio_helper* audio_helper)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init("Options", audio_helper, 2600.f, 1400.f));

    ret = true;
  }
  while (false);

  return ret;
}

void options_menu::display()
{
  base_class::display();

  const auto helper = get_audio_helper();

  music_toggle_->setSelectedIndex(helper->get_music_muted() ? 0 : 1);
  sound_toggle_->setSelectedIndex(helper->get_effects_muted() ? 0 : 1);

  music_slider_->set_percentage(helper->get_music_volume() * 100.f);
  music_slider_->enable(!helper->get_music_muted());

  sound_slider_->set_percentage(helper->get_sound_volume() * 100.f);
  sound_slider_->enable(!helper->get_effects_muted());
}

bool options_menu::create_menu_items()
{
  auto result = false;
  do
  {
    UTILS_BREAK_IF(!add_text_button("Back", CC_CALLBACK_0(options_menu::on_back, this)));

    sound_toggle_ = add_toggle_text_button("Sound", CC_CALLBACK_0(options_menu::on_sound, this));
    UTILS_BREAK_IF(sound_toggle_ == nullptr);

    sound_toggle_->setPosition(sound_toggle_->getPosition() - Vec2(getContentSize().width / 4, 0.f));

    sound_slider_ = add_slider(sound_toggle_, CC_CALLBACK_1(options_menu::on_sound_slider_change, this));
    UTILS_BREAK_IF(sound_slider_ == nullptr);

    music_toggle_ = add_toggle_text_button("Music", CC_CALLBACK_0(options_menu::on_music, this));
    UTILS_BREAK_IF(music_toggle_ == nullptr);

    music_toggle_->setPosition(sound_toggle_->getPosition().x, music_toggle_->getPosition().y);

    music_slider_ = add_slider(music_toggle_, CC_CALLBACK_1(options_menu::on_music_slider_change, this));
    UTILS_BREAK_IF(music_slider_ == nullptr);

    result = true;
  }
  while (false);
  return result;
}

void options_menu::on_back()
{
  get_audio_helper()->play_effect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_main_menu();
}

void options_menu::on_music()
{
  get_audio_helper()->play_effect("sounds/select.mp3");

  const auto menu = dynamic_cast<menu_scene*>(getParent());
  const auto disable = music_toggle_->getSelectedIndex() == 0;
  music_slider_->enable(!disable);
  menu->change_music(disable);
}

void options_menu::on_sound()
{
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  const auto disable = sound_toggle_->getSelectedIndex() == 0;
  sound_slider_->enable(!disable);
  menu->change_sound(disable);

  get_audio_helper()->play_effect("sounds/select.mp3");
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
