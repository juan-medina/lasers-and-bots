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

#include "play_menu.h"
#include "../utils/audio/audio_helper.h"
#include "../scenes/menu_scene.h"
#include "../laser_and_bots_app.h"
#include "../misc/level_manager.h"

play_menu* play_menu::create(audio_helper* audio_helper)
{
  play_menu* ret = nullptr;

  do
  {
    auto object = new play_menu();
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

bool play_menu::init(audio_helper* audio_helper)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init("Level Select", audio_helper, 1800.f, 1300.f));

    ret = true;
  }
  while (false);

  return ret;
}

void play_menu::display()
{
  base_class::display();

  const auto levels = get_level_manager();
  const auto max_level = levels->get_num_levels();

  for (const auto pair : level_buttons_)
  {
    const auto id = pair.first;
    const auto item = pair.second;

    item->setVisible(id <= max_level);

    const auto stars = levels->get_stars(id);
    item->setEnabled(levels->is_level_enabled(id));

    for (auto star_counter = 0; star_counter < 3; ++star_counter)
    {
      const auto star = item->getChildByTag(star_tag + star_counter);
      const auto is_gold = (star_counter + 1) <= stars;

      star->setColor(is_gold ? Color3B(0, 255, 255) : Color3B::WHITE);
    }
  }
}

bool play_menu::create_menu_items()
{
  auto result = false;
  do
  {
    UTILS_BREAK_IF(add_text_button("Back", CC_CALLBACK_0(play_menu::on_back, this)) == nullptr);

    for (auto button_count = 1; button_count <= 10; ++button_count)
    {
      auto text = string_format("%02d", button_count);
      auto button = add_image_button("02_joystick_empty", text, CC_CALLBACK_1(play_menu::on_play, this, button_count));
      UTILS_BREAK_IF(button == nullptr);

      auto star_pos = Vec2(60.f, 90.f);

      for (auto star_counter = 0; star_counter < 3; ++star_counter)
      {
        auto star = Sprite::createWithSpriteFrameName("09_star_01.png");
        UTILS_BREAK_IF(star == nullptr);

        star->setScale(0.20f);
        star->setPosition(star_pos);

        button->addChild(star);
        star_pos.x += (star->getContentSize().width * star->getScale());
        star->setTag(star_tag + star_counter);
      }

      button->setVisible(false);

      level_buttons_.insert(std::pair<int, MenuItem*>(button_count, button));
    }

    result = true;
  }
  while (false);
  return result;
}

void play_menu::on_back()
{
  get_audio_helper()->play_effect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_main_menu();
}

void play_menu::on_play(Ref*, const int level)
{
  get_audio_helper()->play_effect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->go_to_game(level);
}

level_manager* play_menu::get_level_manager()
{
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  const auto app = dynamic_cast<laser_and_bots_app*>(menu->get_application());
  return app->get_level_manager();
}
