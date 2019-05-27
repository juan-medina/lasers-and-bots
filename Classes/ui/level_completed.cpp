#include "level_completed.h"
#include "../utils/audio/audio_helper.h"
#include "../misc/level_manager.h"
#include "game_ui.h"
#include "text_button.h"

level_completed::level_completed():
  audio_helper_(nullptr),
  continue_item_(nullptr),
  level_name_label_(nullptr),
  level_manager_(nullptr),
  level_total_time_label_(nullptr),
  level_time_limit_label_(nullptr),
  level_time_record_label_(nullptr),
  level_3_stars_record_label_(nullptr)
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

    UTILS_BREAK_IF(!base_class::init("Level Completed", audio_helper, 1800.f, 2100.f, animation_type::fade));

    audio_helper_->pre_load_effect("sounds/star.mp3");

    const auto horizontal_segment = getContentSize().width;
    const auto vertical_segment = getContentSize().height;

    level_name_label_ = Label::createWithTTF("", "fonts/tahoma.ttf", 100);
    UTILS_BREAK_IF(level_name_label_ == nullptr);

    level_name_label_->setTextColor(Color4B(255, 255, 255, 255));
    level_name_label_->enableOutline(Color4B(0, 0, 0, 255), 5);

    level_name_label_->setPosition(0.f, (getContentSize().height / 2) - 250);

    addChild(level_name_label_, 100);

    const auto first_start_pos = Vec2(-horizontal_segment / 3, level_name_label_->getPosition().y - 350.f);

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

      addChild(star_gold, 110);
    }

    const auto first_star = gray_stars_.at(0);
    const auto first_label_pos = first_star->getPosition()
      - Vec2(first_star->getContentSize().width / 2, first_star->getContentSize().height * 1.9);
    auto label_pos = first_label_pos;

    static const auto separation = 950.f;

    level_total_time_label_ = add_labels("Total Time", "00:00.00", label_pos, separation);
    UTILS_BREAK_IF(level_total_time_label_ == nullptr);
    label_pos.y -= 200;

    level_time_limit_label_ = add_labels("Time Limit", "00:00.00", label_pos, separation);
    UTILS_BREAK_IF(level_time_limit_label_ == nullptr);
    label_pos.y -= 200;

    level_time_record_label_ = add_labels("Time Record", "00:00.00", label_pos, separation);
    UTILS_BREAK_IF(level_time_limit_label_ == nullptr);
    label_pos.y -= 200;

    level_3_stars_record_label_ = add_labels("3 Stars Record", "00:00.00", label_pos, separation);
    UTILS_BREAK_IF(level_time_limit_label_ == nullptr);

    ret = true;
  }
  while (false);
  return ret;
}

void level_completed::display(const unsigned short int level, const float time, const unsigned short int stars,
                              const completed_result completion, const ccMenuCallback& callback)
{
  base_class::display();

  const auto level_name = level_manager_->get_level_name(level);
  const auto level_time_limit = level_manager_->get_level_time_limit(level);
  const auto level_time_record = level_manager_->get_level_time_record(level);
  const auto level_3_stars_record = level_manager_->get_level_3_stars_record(level);

  level_total_time_label_->setString(game_ui::time_message(time));
  level_time_limit_label_->setString(game_ui::time_message(level_time_limit));

  if (level_time_record != level_manager::no_time_record)
  {
    level_time_record_label_->setString(game_ui::time_message(level_time_record));
    if ((completion == completed_result::new_level_record) || (completion == completed_result::
      new_level_record_and_3_stars_record))
    {
      animate_label(level_time_record_label_);
    }
  }
  else
  {
    level_time_record_label_->setString("n/a");
  }

  if (level_3_stars_record != level_manager::no_time_record)
  {
    level_3_stars_record_label_->setString(game_ui::time_message(level_3_stars_record));
    if ((completion == completed_result::new_level_3_stars_record) || (completion == completed_result::
      new_level_record_and_3_stars_record))
    {
      animate_label(level_3_stars_record_label_);
    }
  }
  else
  {
    level_3_stars_record_label_->setString("n/a");
  }

  level_name_label_->setString(level_name);
  continue_item_->setCallback(callback);
  setVisible(true);

  if (stars > 0)
  {
    for (unsigned short int start_counter = 0; start_counter < 3; ++start_counter)
    {
      const auto is_gold = start_counter + 1 <= stars;

      gray_stars_.at(start_counter)->setOpacity(255);

      if (start_counter == 1)
      {
        const auto star_tex = "under\n " + game_ui::time_message(level_time_limit);
        label_stars_.at(start_counter)->setString(star_tex);
      }

      if (is_gold)
      {
        const auto star_gold = gold_stars_.at(start_counter);

        star_gold->setColor(Color3B(0, 255, 255));

        const auto delay = DelayTime::create(0.5f + (static_cast<float>(start_counter) * 1.f));
        const auto play_sound = CallFunc::create(CC_CALLBACK_0(level_completed::star_sound, this));

        const auto fade_in = FadeIn::create(1.f);
        const auto appear = Sequence::create(delay, fade_in, nullptr);
        star_gold->runAction(appear);

        const auto scale_up = ScaleTo::create(0.5f, 1.5f, 1.5f);
        const auto scale_down = ScaleTo::create(0.5f, 1.0f, 1.0f);
        const auto scale = Sequence::create(delay->clone(), scale_up, scale_down, play_sound->clone(), nullptr);
        star_gold->runAction(scale);
      }
    }
  }
}

void level_completed::hide()
{
  base_class::hide();
}

void level_completed::star_sound() const
{
  audio_helper_->play_effect("sounds/star.mp3");
}

void level_completed::animate_label(Label* label) const
{
  const auto ink_down = TintTo::create(0.5f, 180, 180, 180);
  const auto ink_up = TintTo::create(0.5f, 255, 255, 255);
  const auto sequence = Sequence::create(ink_down, ink_up, nullptr);
  const auto repeat = RepeatForever::create(sequence);
  label->runAction(repeat);
}

bool level_completed::create_menu_items()
{
  auto result = false;
  do
  {
    continue_item_ = add_text_button("Continue", nullptr);
    UTILS_BREAK_IF(continue_item_ == nullptr);

    set_default_menu_item(continue_item_);

    result = true;
  }
  while (false);

  return result;
}

Label* level_completed::add_labels(const std::string& label_text, const std::string& text, const Vec2& pos,
                                   const float separation)
{
  Label* result = nullptr;
  do
  {
    auto label = Label::createWithTTF(label_text, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label == nullptr);

    label->setPosition(pos - Vec2(-label->getContentSize().width / 2,
                                  -label->getContentSize().height / 2));
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
  }
  while (false);

  return result;
}
