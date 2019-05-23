#include "level_completed.h"
#include "../utils/audio/audio_helper.h"
#include "resizable_window.h"
#include "../misc/level_manager.h"
#include "game_ui.h"

level_completed::level_completed():
  audio_helper_(nullptr),
  continue_item_(nullptr),
  sub_label_(nullptr),
  level_manager_(nullptr)
{
}

level_completed* level_completed::create(audio_helper* audio_helper, level_manager* level_manager)
{
  level_completed* ret = nullptr;

  do
  {
    auto object = new level_completed();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audio_helper, level_manager))
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

bool level_completed::init(audio_helper* audio_helper, level_manager* level_manager)
{
  auto ret = false;

  do
  {
    audio_helper_ = audio_helper;
    level_manager_ = level_manager;

    const auto& size = Director::getInstance()->getVisibleSize();

    const auto dark_all = LayerColor::create(Color4B(0, 0, 0, 127));
    UTILS_BREAK_IF(dark_all == nullptr);

    addChild(dark_all, 0);
    dark_all->setPosition(-size.width / 2, -size.height / 2);

    UTILS_BREAK_IF(!base_class::init("Level Completed", 1800.f, 1300.f));

    audio_helper_->pre_load_effect("sounds/star.mp3");

    const auto horizontal_segment = getContentSize().width;
    const auto vertical_segment = getContentSize().height;

    setPosition(size.width / 2, size.height / 2);

    sub_label_ = Label::createWithTTF("", "fonts/tahoma.ttf", 100);
    UTILS_BREAK_IF(sub_label_ == nullptr);

    sub_label_->setTextColor(Color4B(255, 255, 255, 255));
    sub_label_->enableOutline(Color4B(0, 0, 0, 255), 5);

    sub_label_->setPosition(0.f, (getContentSize().height / 2) - 250);

    addChild(sub_label_, 100);

    const auto continue_sprite = Sprite::createWithSpriteFrameName("08_Text_1.png");
    UTILS_BREAK_IF(continue_sprite == nullptr);

    const auto continue_sprite_click = Sprite::createWithSpriteFrameName("08_Text_2.png");
    UTILS_BREAK_IF(continue_sprite_click == nullptr);

    continue_item_ = MenuItemSprite::create(continue_sprite, continue_sprite_click);
    UTILS_BREAK_IF(continue_item_ == nullptr);

    continue_item_->setPosition(0, 0);

    const auto label_button = Label::createWithTTF("Continue", "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label_button == nullptr);

    label_button->setPosition(continue_sprite->getContentSize().width / 2,
                              continue_sprite->getContentSize().height / 2 + 30);
    label_button->setTextColor(Color4B(255, 255, 255, 255));
    label_button->enableOutline(Color4B(0, 0, 0, 255), 5);

    continue_item_->addChild(label_button, 100);

    const auto menu = Menu::create(continue_item_, nullptr);
    UTILS_BREAK_IF(menu == nullptr);

    menu->setPosition(0.f, -getContentSize().height / 2);

    addChild(menu, 100);

    const auto first_start_pos = Vec2(-horizontal_segment / 3, 50.f);

    for (unsigned short int start_counter = 0; start_counter < 3; ++start_counter)
    {
      const auto star_gray = Sprite::createWithSpriteFrameName("09_star_01.png");
      UTILS_BREAK_IF(star_gray == nullptr);

      gray_stars_.push_back(star_gray);
      star_gray->setCascadeOpacityEnabled(true);
      star_gray->setOpacity(0);

      const auto star_pos = first_start_pos + Vec2((horizontal_segment / 3) * start_counter, 0.f);
      star_gray->setPosition(star_pos);
      addChild(star_gray, 100);

      auto star_tex = string("level\ncompleted");
      if (start_counter == 1)
      {
        star_tex = "under\n 00:00.00";
      }
      else if (start_counter == 2)
      {
        star_tex = "with 100%\nshield";
      }

      const auto label_star = Label::createWithTTF(star_tex, "fonts/tahoma.ttf", 70);
      UTILS_BREAK_IF(label_star == nullptr);
      label_stars_.push_back(label_star);

      const auto label_pos = Vec2(star_gray->getContentSize().width / 2,
                                  -(star_gray->getContentSize().height / 2) + 40.f);
      label_star->setHorizontalAlignment(TextHAlignment::CENTER);
      label_star->setPosition(label_pos);
      label_star->setTextColor(Color4B(255, 255, 255, 255));
      label_star->enableOutline(Color4B(0, 0, 0, 255), 5);

      star_gray->addChild(label_star, 100);

      const auto star_gold = Sprite::createWithSpriteFrameName("09_star_01.png");
      UTILS_BREAK_IF(star_gold == nullptr);

      gold_stars_.push_back(star_gold);

      star_gold->setPosition(star_pos);
      star_gold->setOpacity(0);

      addChild(star_gold, 100);
    }

    setOpacity(0);
    setVisible(false);

    ret = true;
  }
  while (false);
  return ret;
}

void level_completed::display(const unsigned short int level, const float time, const unsigned short int stars,
                              const ccMenuCallback& callback)
{
  const auto level_name = level_manager_->get_level_name(level);
  const auto time_limit = level_manager_->get_level_time_limit(level);

  sub_label_->setString(level_name);
  continue_item_->setCallback(callback);
  setVisible(true);

  const auto fade_in_message = FadeTo::create(0.5f, 190);
  runAction(fade_in_message);

  if (stars > 0)
  {
    for (unsigned short int start_counter = 0; start_counter < 3; ++start_counter)
    {
      const auto is_gold = start_counter + 1 <= stars;

      gray_stars_.at(start_counter)->setOpacity(255);

      auto star_tex = string("level\ncompleted");
      if (start_counter == 1)
      {
        star_tex = "under\n " + game_ui::time_message(time_limit);
      }

      label_stars_.at(start_counter)->setString(star_tex);

      if (is_gold)
      {
        const auto star_gold = gold_stars_.at(start_counter);

        star_gold->setColor(Color3B(0, 255, 255));

        const auto play_sound = CallFunc::create(CC_CALLBACK_0(level_completed::star_sound, this));
        const auto delay = DelayTime::create(0.5f + (static_cast<float>(start_counter) * 1.f));
        const auto fade_in = FadeIn::create(1.f);
        const auto appear = Sequence::create(delay, fade_in, nullptr);
        star_gold->runAction(appear);

        const auto scale_up = ScaleTo::create(0.5f, 1.5f, 1.5f);
        const auto scale_down = ScaleTo::create(0.5f, 1.0f, 1.0f);
        const auto scale = Sequence::create(delay->clone(), scale_up, scale_down, play_sound, nullptr);
        star_gold->runAction(scale);
      }
    }
  }
}

void level_completed::star_sound() const
{
  audio_helper_->play_effect("sounds/star.mp3");
}
