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

#include "main_menu.h"
#include "../utils/audio/audio_helper.h"
#include "../scenes/menu_scene.h"
#include "../ui/text_button.h"

main_menu::main_menu():
  back_item_(nullptr),
  play_item_(nullptr)
{
}

main_menu* main_menu::create(audio_helper* audio_helper)
{
  main_menu* ret = nullptr;

  do
  {
    auto object = new main_menu();
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

bool main_menu::init(audio_helper* audio_helper)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init("Main Menu", audio_helper, 1300.f, 2200.f));

    ret = true;
  }
  while (false);

  return ret;
}

void main_menu::display()
{
  const auto need_select_play = get_selected_menu_item() == nullptr;
  base_class::display();

  if (need_select_play)
  {
    select_menu_item(play_item_);
  }
}


bool main_menu::create_menu_items()
{
  auto result = false;
  do
  {
    back_item_ = add_text_button("Exit", CC_CALLBACK_0(main_menu::on_exit, this));
    UTILS_BREAK_IF(back_item_ == nullptr);

    UTILS_BREAK_IF(add_text_button("Credits", CC_CALLBACK_0(main_menu::on_credits, this)) == nullptr);
    UTILS_BREAK_IF(add_text_button("About", CC_CALLBACK_0(main_menu::on_about, this)) == nullptr);
    UTILS_BREAK_IF(add_text_button("Options", CC_CALLBACK_0(main_menu::on_options, this)) == nullptr);

    play_item_ = add_text_button("PLAY!", CC_CALLBACK_0(main_menu::on_play, this));
    UTILS_BREAK_IF(play_item_ == nullptr);

    set_default_menu_item(back_item_);

    result = true;
  }
  while (false);
  return result;
}


void main_menu::on_options()
{
  get_audio_helper()->play_effect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_options_menu();
}

void main_menu::on_play()
{
  get_audio_helper()->play_effect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_play_menu();
}

void main_menu::on_exit()
{
  get_audio_helper()->play_effect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->exit_app();
}

void main_menu::on_credits()
{
  get_audio_helper()->play_effect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_credits_menu();
}

void main_menu::on_about()
{
  get_audio_helper()->play_effect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_about_menu();
}
