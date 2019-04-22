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

#include "game_ui.h"
#include "../utils/audio/audio_helper.h"
#include "../scenes/game_scene.h"
#include "virtual_joy_stick.h"


game_ui::game_ui():
  virtual_joy_stick_(nullptr),
  shield_bar_(nullptr),
  shield_label_(nullptr),
  pause_item_(nullptr),
  time_label_(nullptr)
{
}

game_ui::~game_ui()
{
}

game_ui* game_ui::create()
{
  game_ui* ret = nullptr;

  do
  {
    auto object = new game_ui();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init())
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

  // return the object
  return ret;
}

// on "init" you need to initialize your instance
bool game_ui::init()
{
  auto ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first
    UTILS_BREAK_IF(!base_class::init());

    audio_helper::pre_load_effect("sounds/select.ogg");
    audio_helper::pre_load_effect("sounds/star.ogg");

    const auto& size = Director::getInstance()->getVisibleSize();

    //////////////////////////////
    // cache
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui.plist");

    //////////////////////////////
    // close
    const auto close = Sprite::createWithSpriteFrameName("01_Exit_1.png");
    UTILS_BREAK_IF(close==nullptr);

    const auto close_click = Sprite::createWithSpriteFrameName("01_Exit_2.png");
    UTILS_BREAK_IF(close_click == nullptr);

    const auto close_item = MenuItemSprite::create(close, close_click, CC_CALLBACK_1(game_ui::on_close, this));
    UTILS_BREAK_IF(close_item == nullptr);

    close_item->setPosition(size.width / 2 - close->getContentSize().width,
                            size.height / 2 - close->getContentSize().height);

    //////////////////////////////
    // pause
    const auto pause = Sprite::createWithSpriteFrameName("05_Pause_1.png");
    UTILS_BREAK_IF(pause == nullptr);

    const auto pause_click = Sprite::createWithSpriteFrameName("05_Pause_2.png");
    UTILS_BREAK_IF(pause_click == nullptr);

    const auto pause_disable = Sprite::createWithSpriteFrameName("05_Pause_4.png");
    UTILS_BREAK_IF(pause_click == nullptr);

    const auto pause_item = MenuItemSprite::create(pause, pause_click, pause_disable);
    UTILS_BREAK_IF(pause_item == nullptr);

    const auto play = Sprite::createWithSpriteFrameName("06_Play_3.png");
    UTILS_BREAK_IF(play == nullptr);

    const auto play_click = Sprite::createWithSpriteFrameName("06_Play_2.png");
    UTILS_BREAK_IF(play_click == nullptr);

    const auto play_disable = Sprite::createWithSpriteFrameName("06_Play_4.png");
    UTILS_BREAK_IF(play_disable == nullptr);


    const auto play_item = MenuItemSprite::create(play, play_click, play_disable);
    UTILS_BREAK_IF(play_item == nullptr);

    pause_item_ = MenuItemToggle::createWithCallback(CC_CALLBACK_1(game_ui::on_pause, this),
                                                     pause_item, play_item, nullptr);
    UTILS_BREAK_IF(pause_item_ == nullptr);


    pause_item_->setPosition(close_item->getPosition() - Vec2(close->getContentSize().width + 10.f, 0.f));

    //////////////////////////////
    // reload
    const auto reload = Sprite::createWithSpriteFrameName("07_Reload_1.png");
    UTILS_BREAK_IF(reload == nullptr);

    const auto reload_click = Sprite::createWithSpriteFrameName("07_Reload_2.png");
    UTILS_BREAK_IF(reload_click == nullptr);

    const auto reload_item = MenuItemSprite::create(reload, reload_click, CC_CALLBACK_1(game_ui::on_reload, this));
    UTILS_BREAK_IF(reload_item == nullptr);

    reload_item->setPosition(pause_item_->getPosition() - Vec2(pause->getContentSize().width + 10.f, 0.f));

    //////////////////////////////
    // menu
    const auto menu = Menu::create(close_item, pause_item_, reload_item, nullptr);
    UTILS_BREAK_IF(menu == nullptr);

    addChild(menu);

    //////////////////////////////
    // joystick
    virtual_joy_stick_ = virtual_joy_stick::create(size.height - 500.f);
    addChild(virtual_joy_stick_);

    //////////////////////////////
    // head
    const auto head_pos = Vec2(50.f, size.height - 50);

    auto head = Sprite::createWithSpriteFrameName("03_head.png");
    UTILS_BREAK_IF(head == nullptr);

    head->setAnchorPoint(Vec2(0, 1));
    head->setPosition(head_pos);
    addChild(head);

    //////////////////////////////
    // shield bar
    const auto bar_sprite = Sprite::createWithSpriteFrameName("04_bar.png");
    UTILS_BREAK_IF(bar_sprite == nullptr);

    const auto bar_pos = head_pos + Vec2(head->getContentSize().width + (bar_sprite->getContentSize().width / 2.f),
                                         -head->getContentSize().height / 2.f);
    bar_sprite->setPosition(bar_pos);

    addChild(bar_sprite);

    const auto bar_full_sprite = Sprite::createWithSpriteFrameName("05_bar_full.png");
    UTILS_BREAK_IF(bar_full_sprite == nullptr);
    bar_full_sprite->setOpacity(120);
    bar_full_sprite->setBlendFunc(BlendFunc::ADDITIVE);

    shield_bar_ = ProgressTimer::create(bar_full_sprite);
    UTILS_BREAK_IF(shield_bar_ == nullptr);

    shield_bar_->setType(ProgressTimer::Type::BAR);
    shield_bar_->setMidpoint(Vec2(0, 0));
    shield_bar_->setBarChangeRate(Vec2(1, 0));
    shield_bar_->setPercentage(100.f);
    shield_bar_->setPosition(bar_pos);

    shield_bar_->setOpacity(200);

    addChild(shield_bar_);

    shield_label_ = Label::createWithTTF("100%", "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(shield_label_ == nullptr);

    shield_label_->setTextColor(Color4B(0, 127, 127, 255));
    shield_label_->setPosition(bar_pos);

    addChild(shield_label_);

    //////////////////////////////
    // time label

    // create the text for the label
    time_label_ = Label::createWithTTF("00:00.00", "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(time_label_ == nullptr);

    time_label_->setTextColor(Color4B(0, 255, 255, 255));

    // position the label
    time_label_->setPosition(Vec2(size.width / 2, size.height - close_item->getContentSize().height));

    addChild(time_label_);

    ret = true;
  }
  while (false);

  return ret;
}

void game_ui::on_pause(Ref* sender)
{
  audio_helper::get_instance()->play_effect("sounds/select.ogg");
  const auto scene = dynamic_cast<game_scene*>(Director::getInstance()->getRunningScene());
  scene->toggle_pause();
}

void game_ui::on_close(Ref* sender)
{
  audio_helper::get_instance()->play_effect("sounds/select.ogg");
  const auto scene = dynamic_cast<game_scene*>(Director::getInstance()->getRunningScene());
  scene->pause();

  //Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();
}

void game_ui::on_reload(Ref* sender)
{
  audio_helper::get_instance()->play_effect("sounds/select.ogg");
  const auto scene = dynamic_cast<game_scene*>(Director::getInstance()->getRunningScene());
  scene->reload();
}

void game_ui::update_time(const float time) const
{
  float whole;
  const auto fractional = std::modf(time, &whole);

  const auto minutes = static_cast<int>(whole / 60.f);
  const auto seconds = static_cast<int>(time - (minutes * 60));
  const auto milliseconds = static_cast<int>(fractional * 100);
  time_label_->setString(string_format("%02d:%02d%c%02d", minutes, seconds, '.', milliseconds));
}

string game_ui::time_message(const unsigned int time)
{
  const auto minutes = static_cast<int>(time / 60.f);
  const auto seconds = static_cast<int>(time - (minutes * 60));
  return string_format("%02d:%02d%", minutes, seconds);
}

void game_ui::display_message(const std::string& message, const bool extended /*= false*/,
                              const unsigned short int stars /*= 0*/, const unsigned int limit_seconds /*= 0*/)
{
  do
  {
    const auto& size = Director::getInstance()->getVisibleSize();

    //////////////////////////////
    // background

    const auto horizontal_segment = size.width / 2;
    const auto vertical_segment = size.height / (extended ? 2 : 4);

    const auto dark_all = LayerColor::create(Color4B(0, 0, 0, 0));
    UTILS_BREAK_IF(dark_all == nullptr);

    addChild(dark_all);
    dark_all->runAction(FadeTo::create(0.5f, 127));

    auto area = Rect(horizontal_segment, vertical_segment, 100, 100);

    const auto background = LayerColor::create(Color4B(0, 255, 255, 0), horizontal_segment, vertical_segment);
    UTILS_BREAK_IF(background == nullptr);

    background->setPosition((size.width - horizontal_segment) / 2, (size.height - vertical_segment) / 2);

    addChild(background);
    background->runAction(FadeTo::create(0.5f, 127));

    //////////////////////////////
    // border

    const auto draw = DrawNode::create(4.f);
    draw->drawRect(Vec2(0.f, 0.f), Vec2(horizontal_segment, vertical_segment), Color4F(0, 1.f, 1.f, 0.5f));
    background->addChild(draw);

    //////////////////////////////
    // label

    const auto label = Label::createWithTTF(message, "fonts/tahoma.ttf", 200);
    UTILS_BREAK_IF(label == nullptr);

    label->setTextColor(Color4B(0, 255, 255, 255));

    // position the label
    label->setPosition(horizontal_segment / 2, vertical_segment - label->getContentSize().height);

    background->addChild(label);

    //////////////////////////////
    // button

    const auto continue_sprite = Sprite::createWithSpriteFrameName("08_Text_1.png");
    UTILS_BREAK_IF(continue_sprite == nullptr);

    const auto continue_sprite_click = Sprite::createWithSpriteFrameName("08_Text_2.png");
    UTILS_BREAK_IF(continue_sprite_click == nullptr);

    const auto continue_item = MenuItemSprite::create(continue_sprite, continue_sprite_click,
                                                      CC_CALLBACK_1(game_ui::on_reload, this));
    UTILS_BREAK_IF(continue_item == nullptr);

    continue_item->setPosition(-horizontal_segment / 2,
                               (-size.height / 2) + (continue_sprite->getContentSize().height));

    const auto label_button = Label::createWithTTF("Continue", "fonts/tahoma.ttf", 60);
    UTILS_BREAK_IF(label_button == nullptr);

    label_button->setPosition(continue_sprite->getContentSize().width / 2,
                              continue_sprite->getContentSize().height / 2);
    label_button->setTextColor(Color4B(0, 64, 64, 255));

    continue_item->addChild(label_button);
    continue_item->setScale(2.f);

    continue_item->setOpacity(0);
    continue_item->runAction(FadeTo::create(0.5f, 255));

    //////////////////////////////
    // menu
    const auto menu = Menu::create(continue_item, nullptr);
    UTILS_BREAK_IF(menu == nullptr);

    background->addChild(menu);

    if (!extended)
    {
      break;
    }

    //////////////////////////////
    // time label

    const auto label_time = Label::createWithTTF("Time :", "fonts/tahoma.ttf", 150);
    UTILS_BREAK_IF(label_time == nullptr);

    label_time->setAnchorPoint(Vec2(0.f, 0.f));
    label_time->setHorizontalAlignment(TextHAlignment::LEFT);
    label_time->setTextColor(Color4B(255, 255, 255, 255));

    // position the label
    label_time->setPosition(50, label->getPosition().y - 600);

    background->addChild(label_time);

    const auto label_time_value = Label::createWithTTF(time_label_->getString(), "fonts/tahoma.ttf", 150);
    UTILS_BREAK_IF(label_time_value == nullptr);

    label_time_value->setAnchorPoint(Vec2(0.f, 0.f));
    label_time_value->setHorizontalAlignment(TextHAlignment::LEFT);
    label_time_value->setTextColor(Color4B(255, 255, 255, 255));

    // position the label
    label_time_value->setPosition(650, label_time->getPosition().y);

    background->addChild(label_time_value);

    //////////////////////////////
    // shield label

    const auto label_shield = Label::createWithTTF("Shield :", "fonts/tahoma.ttf", 150);
    UTILS_BREAK_IF(label_shield == nullptr);

    label_shield->setAnchorPoint(Vec2(0.f, 0.f));
    label_shield->setHorizontalAlignment(TextHAlignment::LEFT);
    label_shield->setTextColor(Color4B(255, 255, 255, 255));

    // position the label
    label_shield->setPosition(50, label->getPosition().y - 900);

    background->addChild(label_shield);

    const auto shield_value = string_format("%d %%", static_cast<int>(shield_bar_->getPercentage()));

    const auto label_shield_value = Label::createWithTTF(shield_value, "fonts/tahoma.ttf", 150);
    UTILS_BREAK_IF(label_shield_value == nullptr);

    label_shield_value->setAnchorPoint(Vec2(0.f, 0.f));
    label_shield_value->setHorizontalAlignment(TextHAlignment::LEFT);
    label_shield_value->setTextColor(Color4B(255, 255, 255, 255));

    // position the label
    label_shield_value->setPosition(650, label_shield->getPosition().y);

    background->addChild(label_shield_value);

    //////////////////////////////
    // stars

    const auto first_start_pos = Vec2(horizontal_segment / 6, 750.f);

    for (unsigned short int start_counter = 0; start_counter < 3; ++start_counter)
    {
      const auto is_gold = start_counter + 1 <= stars;

      const auto star_gray = Sprite::createWithSpriteFrameName("09_star_02.png");

      const auto star_pos = first_start_pos + Vec2((horizontal_segment/3) * start_counter, 0.f);
      star_gray->setPosition(star_pos);
      background->addChild(star_gray);

      auto star_tex = string("complete level");
      if (start_counter == 1)
      {
        star_tex = "and under " + time_message(limit_seconds);
      }
      else if (start_counter == 2)
      {
        star_tex = "and with 100% shield";
      }
      const auto label_star = Label::createWithTTF(star_tex, "fonts/tahoma.ttf", 70);
      UTILS_BREAK_IF(label_star == nullptr);

      const auto label_pos = Vec2(star_gray->getContentSize().width / 2,
                                  -(star_gray->getContentSize().height / 2) + 40.f);
      label_star->setHorizontalAlignment(TextHAlignment::CENTER);
      label_star->setPosition(label_pos);
      label_star->setTextColor(Color4B(255, 255, 255, 255));
      star_gray->addChild(label_star);

      if (is_gold)
      {
        const auto star_gold = Sprite::createWithSpriteFrameName("09_star_01.png");

        star_gold->setPosition(star_pos);
        star_gold->setOpacity(0);

        const auto play_sound = CallFunc::create(CC_CALLBACK_0(game_ui::star_sound, this));
        const auto delay = DelayTime::create(0.5f + (1.f * start_counter));
        const auto fade_in = FadeIn::create(1.f);
        const auto appear = Sequence::create(delay, fade_in, nullptr);
        star_gold->runAction(appear);

        const auto scale_up = ScaleTo::create(0.5f, 1.5f, 1.5f);
        const auto scale_down = ScaleTo::create(0.5f, 1.0f, 1.0f);
        const auto scale = Sequence::create(delay->clone(), scale_up, scale_down, play_sound, nullptr);
        star_gold->runAction(scale);

        background->addChild(star_gold);
      }
    }
  }
  while (false);
}

void game_ui::star_sound()
{
  audio_helper::get_instance()->play_effect("sounds/star.ogg");
}
