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
  time_label_(nullptr),
  sub_time_label_(nullptr),
  countdown_label_(nullptr),
  time_limit_(0),
  continue_callback_(nullptr)
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

    audio_helper::pre_load_effect("sounds/select.mp3");
    audio_helper::pre_load_effect("sounds/star.mp3");

    const auto& size = Director::getInstance()->getVisibleSize();

    //////////////////////////////
    // cache
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-0.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-1.plist");

    //////////////////////////////
    // close
    const auto close = Sprite::createWithSpriteFrameName("01_Exit_1.png");
    UTILS_BREAK_IF(close==nullptr);

    const auto close_click = Sprite::createWithSpriteFrameName("01_Exit_2.png");
    UTILS_BREAK_IF(close_click == nullptr);

    const auto close_item = MenuItemSprite::create(close, close_click, CC_CALLBACK_1(game_ui::on_close, this));
    UTILS_BREAK_IF(close_item == nullptr);

    close_item->setPosition(size.width / 2 - close->getContentSize().width / 2,
                            size.height / 2 - close->getContentSize().height / 2);

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

    const auto gap = Vec2(close->getContentSize().width * 1.25f, 0.f);
    pause_item_->setPosition(close_item->getPosition() - gap);

    //////////////////////////////
    // reload
    const auto reload = Sprite::createWithSpriteFrameName("07_Reload_1.png");
    UTILS_BREAK_IF(reload == nullptr);

    const auto reload_click = Sprite::createWithSpriteFrameName("07_Reload_2.png");
    UTILS_BREAK_IF(reload_click == nullptr);

    const auto reload_item = MenuItemSprite::create(reload, reload_click, CC_CALLBACK_1(game_ui::on_reload, this));
    UTILS_BREAK_IF(reload_item == nullptr);

    reload_item->setPosition(pause_item_->getPosition() - gap);

    //////////////////////////////
    // menu
    const auto menu = Menu::create(close_item, pause_item_, reload_item, nullptr);
    UTILS_BREAK_IF(menu == nullptr);

    addChild(menu, 100);

    //////////////////////////////
    // joystick
    virtual_joy_stick_ = virtual_joy_stick::create();
    addChild(virtual_joy_stick_, 100);

    //////////////////////////////
    // head
    const auto head_pos = Vec2(50.f, size.height - 50);

    auto head = Sprite::createWithSpriteFrameName("03_head.png");
    UTILS_BREAK_IF(head == nullptr);

    head->setAnchorPoint(Vec2(0, 1));
    head->setPosition(head_pos);
    addChild(head, 100);

    //////////////////////////////
    // shield bar
    const auto bar_sprite = Sprite::createWithSpriteFrameName("04_bar.png");
    UTILS_BREAK_IF(bar_sprite == nullptr);

    const auto bar_pos = head_pos + Vec2(head->getContentSize().width + (bar_sprite->getContentSize().width / 2.f),
                                         -head->getContentSize().height / 2.f);
    bar_sprite->setPosition(bar_pos);

    addChild(bar_sprite, 100);

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

    addChild(shield_bar_, 100);

    shield_label_ = Label::createWithTTF("100%", "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(shield_label_ == nullptr);

    shield_label_->setTextColor(Color4B(255, 255, 255, 255));
    shield_label_->setPosition(bar_pos);

    shield_label_->enableOutline(Color4B(0, 0, 0, 255), 5);

    addChild(shield_label_, 100);

    //////////////////////////////
    // time label

    // create the text for the label
    time_label_ = Label::createWithTTF("00:00.00", "fonts/tahoma.ttf", 180);
    UTILS_BREAK_IF(time_label_ == nullptr);

    time_label_->setTextColor(Color4B(0, 255, 255, 255));

    time_label_->enableGlow(Color4B(0, 127, 127, 127));
    time_label_->enableShadow(Color4B(255, 255, 255, 127), Size(5, -5));
    time_label_->enableOutline(Color4B(0, 0, 0, 255), 5);

    // position the label
    time_label_->setPosition(Vec2(size.width / 2, size.height - close_item->getContentSize().height / 2));

    addChild(time_label_, 100);

    //////////////////////////////
    // sub time label

    // create the text for the label
    sub_time_label_ = Label::createWithTTF("00:00.00", "fonts/tahoma.ttf", 90);
    UTILS_BREAK_IF(sub_time_label_ == nullptr);

    sub_time_label_->setTextColor(Color4B(0, 0, 255, 255));

    sub_time_label_->enableShadow(Color4B(0, 0, 0, 127), Size(5, -5));
    sub_time_label_->enableOutline(Color4B(0, 0, 0, 255), 5);

    // position the label
    sub_time_label_->setPosition(Vec2(time_label_->getPosition().x + time_label_->getContentSize().width,
                                      time_label_->getPosition().y));

    addChild(sub_time_label_, 100);

    //////////////////////////////
    // count down label

    // create the text for the label
    countdown_label_ = Label::createWithTTF("GO!", "fonts/tahoma.ttf", 500);
    UTILS_BREAK_IF(countdown_label_ == nullptr);

    countdown_label_->setTextColor(Color4B(0, 255, 255, 255));
    countdown_label_->enableGlow(Color4B(0, 127, 127, 127));
    countdown_label_->enableShadow(Color4B(255, 255, 255, 127), Size(10, -10));
    countdown_label_->enableOutline(Color4B(0, 0, 0, 255), 5);
    countdown_label_->setPosition(Vec2(size.width / 2, size.height / 2));
    countdown_label_->setVisible(false);
    addChild(countdown_label_, 100);

    // ui keyboard
    create_keyboard_listener();

    ret = true;
  }
  while (false);

  return ret;
}

void game_ui::on_pause(Ref* sender)
{
  audio_helper::get_instance()->play_effect("sounds/select.mp3");
  const auto scene = dynamic_cast<game_scene*>(Director::getInstance()->getRunningScene());
  scene->toggle_pause();
}

void game_ui::on_close(Ref* sender)
{
  audio_helper::get_instance()->play_effect("sounds/select.mp3");
  const auto scene = dynamic_cast<game_scene*>(Director::getInstance()->getRunningScene());
  scene->pause();

  //Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();
}

void game_ui::on_reload(Ref* sender)
{
  audio_helper::get_instance()->play_effect("sounds/select.mp3");
  const auto scene = dynamic_cast<game_scene*>(Director::getInstance()->getRunningScene());
  scene->reload();
}

void game_ui::disable_buttons(const bool disabled) const
{
  pause_item_->setEnabled(!disabled);
  virtual_joy_stick_->disabled(disabled);
}

void game_ui::update_time(const float time, const unsigned int time_limit)
{
  time_limit_ = time_limit;
  const auto time_left = time_limit - time;
  if (time_left < 0.f)
  {
    sub_time_label_->setTextColor(Color4B(255, 0, 0, 255));
  }
  sub_time_label_->setString(time_message(fabs(time_left)));
  time_label_->setString(time_message(time));
}

string game_ui::time_message(const float time)
{
  float whole;
  const auto fractional = std::modf(time, &whole);

  const auto minutes = static_cast<int>(whole / 60.f);
  const auto seconds = static_cast<int>(time - (minutes * 60));
  const auto milliseconds = static_cast<int>(fractional * 100);
  return string_format("%02d:%02d%c%02d", minutes, seconds, '.', milliseconds);
}

void game_ui::display_message(const std::string& message, const std::string& sub_message,
                              const ccMenuCallback& callback, const short int stars /*= -1*/)
{
  do
  {
    const auto& size = Director::getInstance()->getVisibleSize();

    //////////////////////////////
    // background

    const auto dark_all = LayerColor::create(Color4B(0, 0, 0, 0));
    UTILS_BREAK_IF(dark_all == nullptr);

    addChild(dark_all, 0);
    dark_all->runAction(FadeTo::create(0.5f, 127));

    const auto background = Sprite::createWithSpriteFrameName("10_message.png");
    UTILS_BREAK_IF(background == nullptr);

    const auto horizontal_segment = background->getContentSize().width;
    const auto vertical_segment = background->getContentSize().height;

    background->setCascadeOpacityEnabled(true);
    background->setOpacity(0);
    background->setPosition(size.width / 2, size.height / 2);
    background->setColor(Color3B(0, 255, 255));

    addChild(background, 100);
    background->runAction(FadeTo::create(0.5f, 190));

    const auto header = Sprite::createWithSpriteFrameName("11_message_header.png");
    UTILS_BREAK_IF(header == nullptr);

    header->setPosition(horizontal_segment / 2, vertical_segment);
    header->setColor(Color3B(0, 127, 127));

    background->addChild(header, 100);

    //////////////////////////////
    // label

    const auto label = Label::createWithTTF(message, "fonts/tahoma.ttf", 150);
    UTILS_BREAK_IF(label == nullptr);

    label->setTextColor(Color4B(0, 255, 255, 255));
    label->enableOutline(Color4B(0, 0, 0, 255), 5);
    label->enableShadow(Color4B(255, 255, 255, 127), Size(5, -5));

    // position the label
    label->setPosition(header->getContentSize().width / 2, (header->getContentSize().height / 2) + 100);

    header->addChild(label, 100);

    //////////////////////////////
    // sub label

    const auto sub_label = Label::createWithTTF(sub_message, "fonts/tahoma.ttf", 100);
    UTILS_BREAK_IF(sub_label == nullptr);

    sub_label->setTextColor(Color4B(255, 255, 255, 255));
    sub_label->enableOutline(Color4B(0, 0, 0, 255), 5);

    // position the label
    sub_label->setPosition(background->getContentSize().width / 2, (background->getContentSize().height) - 250);

    background->addChild(sub_label, 100);

    //////////////////////////////
    // button

    const auto continue_sprite = Sprite::createWithSpriteFrameName("08_Text_1.png");
    UTILS_BREAK_IF(continue_sprite == nullptr);

    const auto continue_sprite_click = Sprite::createWithSpriteFrameName("08_Text_2.png");
    UTILS_BREAK_IF(continue_sprite_click == nullptr);

    continue_callback_ = callback;
    const auto continue_item = MenuItemSprite::create(continue_sprite, continue_sprite_click,
                                                      CC_CALLBACK_0(game_ui::on_continue, this));

    UTILS_BREAK_IF(continue_item == nullptr);

    continue_item->setPosition(horizontal_segment / 2, 0);

    const auto label_button = Label::createWithTTF("Continue", "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label_button == nullptr);

    label_button->setPosition(continue_sprite->getContentSize().width / 2,
                              continue_sprite->getContentSize().height / 2 + 30);
    label_button->setTextColor(Color4B(255, 255, 255, 255));
    label_button->enableOutline(Color4B(0, 0, 0, 255), 5);

    continue_item->addChild(label_button, 100);

    //////////////////////////////
    // menu
    const auto menu = Menu::create(continue_item, nullptr);
    UTILS_BREAK_IF(menu == nullptr);

    menu->setPosition(0, 0);

    background->addChild(menu, 100);

    if (stars < 0)
    {
      break;
    }

    //////////////////////////////
    // stars

    const auto first_start_pos = Vec2(horizontal_segment / 6, 750.f);

    for (unsigned short int start_counter = 0; start_counter < 3; ++start_counter)
    {
      const auto is_gold = start_counter + 1 <= stars;

      const auto star_gray = Sprite::createWithSpriteFrameName("09_star_01.png");

      const auto star_pos = first_start_pos + Vec2((horizontal_segment / 3) * start_counter, 0.f);
      star_gray->setPosition(star_pos);
      background->addChild(star_gray, 100);

      auto star_tex = string("level\ncompleted");
      if (start_counter == 1)
      {
        star_tex = "under\n" + time_message(time_limit_);
      }
      else if (start_counter == 2)
      {
        star_tex = "with 100%\nshield";
      }
      const auto label_star = Label::createWithTTF(star_tex, "fonts/tahoma.ttf", 70);
      UTILS_BREAK_IF(label_star == nullptr);

      const auto label_pos = Vec2(star_gray->getContentSize().width / 2,
                                  -(star_gray->getContentSize().height / 2) + 40.f);
      label_star->setHorizontalAlignment(TextHAlignment::CENTER);
      label_star->setPosition(label_pos);
      label_star->setTextColor(Color4B(255, 255, 255, 255));
      label_star->enableOutline(Color4B(0, 0, 0, 255), 5);

      star_gray->addChild(label_star, 100);

      if (is_gold)
      {
        const auto star_gold = Sprite::createWithSpriteFrameName("09_star_01.png");

        star_gold->setPosition(star_pos);
        star_gold->setOpacity(0);

        star_gold->setColor(Color3B(0, 255, 255));

        const auto play_sound = CallFunc::create(CC_CALLBACK_0(game_ui::star_sound, this));
        const auto delay = DelayTime::create(0.5f + (1.f * start_counter));
        const auto fade_in = FadeIn::create(1.f);
        const auto appear = Sequence::create(delay, fade_in, nullptr);
        star_gold->runAction(appear);

        const auto scale_up = ScaleTo::create(0.5f, 1.5f, 1.5f);
        const auto scale_down = ScaleTo::create(0.5f, 1.0f, 1.0f);
        const auto scale = Sequence::create(delay->clone(), scale_up, scale_down, play_sound, nullptr);
        star_gold->runAction(scale);

        background->addChild(star_gold, 100);
      }
    }
  }
  while (false);
}

void game_ui::update_countdown(const int value) const
{
  const auto text = string_format("%d", value);
  if (value >= 0)
  {
    countdown_label_->setString(text);
    countdown_label_->setVisible(true);
  }
  else
  {
    countdown_label_->setString("GO!");

    const auto delay = DelayTime::create(1.0f);
    const auto fade_out = FadeOut::create(0.5);
    const auto remove = RemoveSelf::create(true);
    const auto vanish = Sequence::create(delay, fade_out, remove, nullptr);
    countdown_label_->runAction(vanish);
  }

  const auto scale_up = ScaleTo::create(0.25f, 1.5f, 1.5f);
  const auto scale_down = ScaleTo::create(0.25f, 1.0f, 1.0f);
  const auto scale = Sequence::create(scale_up, scale_down, nullptr);
  countdown_label_->runAction(scale);
}

void game_ui::star_sound()
{
  audio_helper::get_instance()->play_effect("sounds/star.mp3");
}

void game_ui::on_continue()
{
  if (continue_callback_ != nullptr)
  {
    audio_helper::get_instance()->play_effect("sounds/select.mp3");
    continue_callback_(this);
    continue_callback_ = nullptr;
  }
}

bool game_ui::create_keyboard_listener()
{
  auto result = false;

  do
  {
    auto listener = EventListenerKeyboard::create();
    UTILS_BREAK_IF(listener == nullptr);

    listener->onKeyPressed = CC_CALLBACK_2(game_ui::on_key_pressed, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    result = true;
  }
  while (false);

  return result;
}

void game_ui::on_key_pressed(EventKeyboard::KeyCode key_code, Event* event)
{
  switch (key_code)
  {
  case EventKeyboard::KeyCode::KEY_ESCAPE:
    if (pause_item_->isEnabled())
    {
      pause_item_->setSelectedIndex(pause_item_->getSelectedIndex() == 0 ? 1 : 0);
      on_pause(this);
    }
    break;
  case EventKeyboard::KeyCode::KEY_ENTER:
    if (continue_callback_ != nullptr)
    {
      on_continue();
    }
    break;
  case EventKeyboard::KeyCode::KEY_F5:
    on_reload(this);
    break;
  default:
    break;
  }
}
