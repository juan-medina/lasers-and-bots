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

#include "play_menu.h"
#include "../laser_and_bots_app.h"
#include "../misc/LevelManager.h"
#include "../scenes/menu_scene.h"
#include "../ui/GameUI.h"
#include "../ui/TextButton.h"
#include "../ui/TextToggle.h"
#include "../utils/audio/AudioHelper.h"

play_menu::play_menu()
  : back_item_(nullptr)
  , play_item_(nullptr)
  , level_name_label_(nullptr)
  , level_time_limit_label_(nullptr)
  , level_time_record_label_(nullptr)
  , level_3_stars_time_record_label_(nullptr)
  , level_stars_label_(nullptr)
  , selected_level_(1)
{
}

play_menu* play_menu::create(AudioHelper* audio_helper, const unsigned short int selected_level)
{
  play_menu* ret = nullptr;

  do
  {
    auto object = new play_menu();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audio_helper, selected_level))
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

bool play_menu::init(AudioHelper* audio_helper, const unsigned short int selected_level)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init("Level Select", audio_helper, 3600.f, 1800.f));

    selected_level_ = selected_level;

    ret = true;
  } while (false);

  return ret;
}

void play_menu::display()
{
  base_class::display();

  const auto levels = get_level_manager();
  const auto max_level = levels->getNumLevels();

  for (const auto pair : level_buttons_)
  {
    const auto id = pair.first;
    const auto item = pair.second;

    item->setVisible(id <= max_level);

    const auto stars = levels->getLevelStars(id);
    item->setEnabled(levels->isLevelEnabled(id));

    for (auto star_counter = 0; star_counter < 3; ++star_counter)
    {
      const auto star = item->getChildByTag(star_tag + star_counter);
      const auto is_gold = (star_counter + 1) <= stars;

      star->setColor(is_gold ? Color3B(0, 255, 255) : Color3B(127, 127, 127));
    }
  }
  select_level(selected_level_);
  selectMenuItem(level_buttons_.at(selected_level_));
}

bool play_menu::createMenuItems()
{
  auto result = false;
  do
  {
    back_item_ = addTextButton("Back", CC_CALLBACK_0(play_menu::on_back, this));
    UTILS_BREAK_IF(back_item_ == nullptr);

    play_item_ = addTextButton("Play", CC_CALLBACK_0(play_menu::on_play, this));
    UTILS_BREAK_IF(play_item_ == nullptr);

    play_item_->setPosition(-550 + play_item_->getContentSize().width / 2, play_item_->getPosition().y);

    const auto margin = Vec2(300.f, 370.f);
    const auto first_button_pos =
      Vec2(-(getContentSize().width / 2) + margin.x, getContentSize().height - margin.y);

    const auto first_label_pos = Vec2(-550.f, (getContentSize().height / 2) - margin.y);
    auto label_pos = first_label_pos;
    static const auto separation = 900.f;

    level_name_label_ = add_labels("Level", "One long level name", label_pos, separation);
    UTILS_BREAK_IF(level_name_label_ == nullptr);
    label_pos.y -= 200;

    level_time_limit_label_ = add_labels("Time Limit", "00:00", label_pos, separation);
    UTILS_BREAK_IF(level_time_limit_label_ == nullptr);
    label_pos.y -= 200;

    level_stars_label_ = add_labels("Stars", "", label_pos, separation);
    UTILS_BREAK_IF(level_stars_label_ == nullptr);
    label_pos.y -= 200;

    auto label_star_pos =
      level_stars_label_->getPosition() + Vec2(level_stars_label_->getContentSize().width + 60.f, 0.f);

    for (auto star_counter = 0; star_counter < 3; ++star_counter)
    {
      auto label_star = Sprite::createWithSpriteFrameName("09_star_01.png");
      UTILS_BREAK_IF(label_star == nullptr);

      label_star->setScale(0.50f);
      label_star->setPosition(label_star_pos);

      addChild(label_star);
      label_star_pos.x += (label_star->getContentSize().width * label_star->getScale()) + 30.f;
      label_star->setTag(star_tag + star_counter);
    }

    level_time_record_label_ = add_labels("Time Record", "00:00", label_pos, separation);
    UTILS_BREAK_IF(level_time_record_label_ == nullptr);
    label_pos.y -= 200;

    level_3_stars_time_record_label_ = add_labels("3 Stars Record", "00:00", label_pos, separation);
    UTILS_BREAK_IF(level_3_stars_time_record_label_ == nullptr);

    auto button_pos = first_button_pos;
    auto col = 1;
    for (unsigned short int button_count = 1; button_count <= 12; ++button_count)
    {
      auto text = StringFormat("%02d", button_count);
      auto button =
        addSmallToggleTextButton(text, CC_CALLBACK_1(play_menu::on_level_select, this, button_count));
      UTILS_BREAK_IF(button == nullptr);

      button->setPosition(button_pos);

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
      level_buttons_.insert(std::make_pair(button_count, button));

      button_pos.x += button->getContentSize().width + 50.0f;
      col++;
      if (col > 3)
      {
        col = 1;
        button_pos.x = first_button_pos.x;
        button_pos.y -= (button->getContentSize().height) + 50.0f;
      }
    }

    setDefaultMenuItem(back_item_);

    result = true;
  } while (false);
  return result;
}

Label* play_menu::add_labels(const std::string& label_text, const std::string& text, const Vec2& pos,
                             const float separation)
{
  Label* result = nullptr;
  do
  {
    auto label = Label::createWithTTF(label_text, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label == nullptr);

    label->setPosition(pos - Vec2(-label->getContentSize().width / 2, -label->getContentSize().height / 2));
    label->setTextColor(Color4B(255, 255, 255, 255));
    label->enableOutline(Color4B(0, 0, 0, 255), 5);
    addChild(label);

    auto value = Label::createWithTTF(text, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label == nullptr);

    value->setAnchorPoint(Vec2(0.f, 0.5f));
    value->setHorizontalAlignment(TextHAlignment::LEFT);
    value->setPosition(pos.x + separation, label->getPosition().y);
    value->setTextColor(Color4B(0, 255, 255, 255));
    value->enableOutline(Color4B(0, 0, 0, 255), 5);
    addChild(value);

    result = value;
  } while (false);

  return result;
}

void play_menu::on_back()
{
  getAudioHelper()->playEffect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_main_menu();
}

void play_menu::on_level_select(Ref*, const unsigned short int level)
{
  getAudioHelper()->playEffect("sounds/select.mp3");

  if (level == selected_level_)
  {
    level_buttons_.at(level)->setSelectedIndex(1);
  }
  else
  {
    select_level(level);
  }
  selectMenuItem(play_item_);
}

void play_menu::on_play()
{
  getAudioHelper()->playEffect("sounds/select.mp3");

  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->go_to_game(selected_level_);
}

void play_menu::select_level(const unsigned short int level)
{
  const auto levels = get_level_manager();
  const auto level_name = levels->getLevelName(level);
  const auto level_time_limit = levels->getLevelTimeLimit(level);
  const auto level_time_limit_string = GameUI::timeMessage(level_time_limit);
  const auto level_time_record = levels->getLevelTimeRecord(level);
  const auto level_time_record_string = GameUI::timeMessage(level_time_record);
  const auto level_3_stars_record = levels->getLevel3StarsRecord(level);
  const auto level_3_stars_record_string = GameUI::timeMessage(level_3_stars_record);
  const auto stars = levels->getLevelStars(level);

  level_name_label_->setString(level_name);
  level_time_limit_label_->setString(level_time_limit_string);

  if (level_time_record != LevelManager::NO_TIME_RECORD)
  {
    level_time_record_label_->setString(level_time_record_string);
  }
  else
  {
    level_time_record_label_->setString("n/a");
  }

  if (level_3_stars_record != LevelManager::NO_TIME_RECORD)
  {
    level_3_stars_time_record_label_->setString(level_3_stars_record_string);
  }
  else
  {
    level_3_stars_time_record_label_->setString("n/a");
  }

  for (const auto pair : level_buttons_)
  {
    const auto id = pair.first;
    const auto item = pair.second;
    item->setSelectedIndex(id == level ? 1 : 0);
  }

  for (auto star_counter = 0; star_counter < 3; ++star_counter)
  {
    const auto star = getChildByTag(star_tag + star_counter);
    const auto is_gold = (star_counter + 1) <= stars;

    star->setColor(is_gold ? Color3B(0, 255, 255) : Color3B::WHITE);
  }

  selected_level_ = level;
}

LevelManager* play_menu::get_level_manager()
{
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  const auto app = dynamic_cast<laser_and_bots_app*>(menu->getApplication());
  return app->get_level_manager();
}
