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
#include "../scenes/game_scene.h"
#include "../utils/audio/audio_helper.h"
#include "virtual_joy_stick.h"
#include "message_window.h"
#include "level_completed.h"
#include "pause_window.h"
#include "../misc/level_manager.h"

game_ui::game_ui():
  virtual_joy_stick_(nullptr),
  shield_bar_(nullptr),
  shield_label_(nullptr),
  pause_item_(nullptr),
  time_label_(nullptr),
  sub_time_label_(nullptr),
  countdown_label_(nullptr),
  level_name_label_(nullptr),
  time_limit_(0),
  continue_callback_(nullptr),
  audio_helper_(nullptr),
  message_window_(nullptr),
  level_completed_(nullptr),
  pause_window_(nullptr),
  level_manager_(nullptr), level_(0)
{
}

game_ui* game_ui::create(audio_helper* audio_helper, level_manager* level_manager, const unsigned short int level)
{
  game_ui* ret = nullptr;

  do
  {
    auto object = new game_ui();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audio_helper, level_manager, level))
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

bool game_ui::init(audio_helper* audio_helper, level_manager* level_manager, const unsigned short int level)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init());

    const auto& size = Director::getInstance()->getVisibleSize();

    audio_helper_ = audio_helper;
    level_manager_ = level_manager;
    level_ = level;

    audio_helper_->pre_load_effect("sounds/select.mp3");

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-0.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-1.plist");

    const auto close = Sprite::createWithSpriteFrameName("01_Exit_1.png");
    UTILS_BREAK_IF(close==nullptr);

    const auto close_click = Sprite::createWithSpriteFrameName("01_Exit_2.png");
    UTILS_BREAK_IF(close_click == nullptr);

    const auto close_item = MenuItemSprite::create(close, close_click, CC_CALLBACK_1(game_ui::on_close, this));
    UTILS_BREAK_IF(close_item == nullptr);

    close_item->setPosition(size.width / 2 - close->getContentSize().width / 2,
                            size.height / 2 - close->getContentSize().height / 2);

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

    const auto reload = Sprite::createWithSpriteFrameName("07_Reload_1.png");
    UTILS_BREAK_IF(reload == nullptr);

    const auto reload_click = Sprite::createWithSpriteFrameName("07_Reload_2.png");
    UTILS_BREAK_IF(reload_click == nullptr);

    const auto reload_item = MenuItemSprite::create(reload, reload_click, CC_CALLBACK_1(game_ui::on_reload, this));
    UTILS_BREAK_IF(reload_item == nullptr);

    reload_item->setPosition(pause_item_->getPosition() - gap);

    const auto menu = Menu::create(close_item, pause_item_, reload_item, nullptr);
    UTILS_BREAK_IF(menu == nullptr);

    addChild(menu, 100);

    virtual_joy_stick_ = virtual_joy_stick::create();
    addChild(virtual_joy_stick_, 100);

    const auto head_pos = Vec2(50.f, size.height - 50);

    auto head = Sprite::createWithSpriteFrameName("03_head.png");
    UTILS_BREAK_IF(head == nullptr);

    head->setAnchorPoint(Vec2(0, 1));
    head->setPosition(head_pos);
    addChild(head, 100);

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

    time_label_ = Label::createWithTTF("00:00.00", "fonts/tahoma.ttf", 180);
    UTILS_BREAK_IF(time_label_ == nullptr);

    time_label_->setTextColor(Color4B(0, 255, 255, 255));

    time_label_->enableGlow(Color4B(0, 127, 127, 127));
    time_label_->enableShadow(Color4B(255, 255, 255, 127), Size(5, -5));
    time_label_->enableOutline(Color4B(0, 0, 0, 255), 5);

    time_label_->setPosition(Vec2(size.width / 2, size.height - close_item->getContentSize().height / 2));

    addChild(time_label_, 100);

    sub_time_label_ = Label::createWithTTF("00:00.00", "fonts/tahoma.ttf", 90);
    UTILS_BREAK_IF(sub_time_label_ == nullptr);

    sub_time_label_->setTextColor(Color4B(0, 0, 255, 255));

    sub_time_label_->enableShadow(Color4B(0, 0, 0, 127), Size(5, -5));
    sub_time_label_->enableOutline(Color4B(0, 0, 0, 255), 5);

    sub_time_label_->setPosition(Vec2(time_label_->getPosition().x + time_label_->getContentSize().width,
                                      time_label_->getPosition().y));

    addChild(sub_time_label_, 100);

    countdown_label_ = Label::createWithTTF("GO!", "fonts/tahoma.ttf", 500);
    UTILS_BREAK_IF(countdown_label_ == nullptr);

    countdown_label_->setTextColor(Color4B(0, 255, 255, 255));
    countdown_label_->enableGlow(Color4B(0, 127, 127, 127));
    countdown_label_->enableShadow(Color4B(255, 255, 255, 127), Size(10, -10));
    countdown_label_->enableOutline(Color4B(0, 0, 0, 255), 5);
    countdown_label_->setPosition(Vec2(size.width / 2, size.height / 2));
    countdown_label_->setVisible(false);
    addChild(countdown_label_, 100);

    level_name_label_ = Label::createWithTTF(level_manager_->get_level_name(level_), "fonts/tahoma.ttf", 500);
    UTILS_BREAK_IF(level_name_label_ == nullptr);

    level_name_label_->setTextColor(Color4B(0, 255, 255, 255));
    level_name_label_->enableGlow(Color4B(0, 127, 127, 127));
    level_name_label_->enableShadow(Color4B(255, 255, 255, 127), Size(10, -10));
    level_name_label_->enableOutline(Color4B(0, 0, 0, 255), 5);
    level_name_label_->setPosition(Vec2(size.width / 2, size.height - (size.height / 3)));
    level_name_label_->setVisible(false);
    addChild(level_name_label_, 100);

    message_window_ = message_window::create(audio_helper_);
    UTILS_BREAK_IF(message_window_ == nullptr);

    addChild(message_window_);

    level_completed_ = level_completed::create(audio_helper_, level_manager_);
    UTILS_BREAK_IF(level_completed_ == nullptr);

    addChild(level_completed_);

    pause_window_ = pause_window::create(audio_helper_);
    UTILS_BREAK_IF(pause_window_ == nullptr);

    addChild(pause_window_);

#if (GAME_PLATFORM == DESKTOP_GAME)
    UTILS_BREAK_IF(!create_keyboard_listener());
#endif

    UTILS_BREAK_IF(!create_controller_listener());

    ret = true;
  }
  while (false);

  return ret;
}

void game_ui::on_pause(Ref* sender)
{
  audio_helper_->play_effect("sounds/select.mp3");
  const auto scene = dynamic_cast<game_scene*>(Director::getInstance()->getRunningScene());
  scene->toggle_pause();

  if (scene->is_paused())
  {
    pause_item_->setSelectedIndex(1);
    pause_window_->display();
  }
  else
  {
    pause_item_->setSelectedIndex(0);
    pause_window_->hide();
  }
}

void game_ui::on_close(Ref* sender)
{
  audio_helper_->play_effect("sounds/select.mp3");
  const auto scene = dynamic_cast<game_scene*>(Director::getInstance()->getRunningScene());
  scene->close();
}

void game_ui::on_reload(Ref* sender)
{
  audio_helper_->play_effect("sounds/select.mp3");
  const auto scene = dynamic_cast<game_scene*>(Director::getInstance()->getRunningScene());
  scene->reload();
}

void game_ui::change_pause_button() const
{
  pause_item_->setSelectedIndex(1);
}

void game_ui::disable_buttons(const bool disabled) const
{
  pause_item_->setEnabled(!disabled);
  pause_item_->setSelectedIndex(0);
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
                              const ccMenuCallback& callback)
{
  continue_callback_ = callback;
  message_window_->display(message, sub_message, time_message(time_limit_), CC_CALLBACK_0(game_ui::on_continue, this));
}

void game_ui::display_level_completed(const unsigned short int level, const float time, const unsigned short int stars,
                                      const completed_result completion, const ccMenuCallback& callback)
{
  continue_callback_ = callback;
  level_completed_->display(level, time, stars, completion, CC_CALLBACK_0(game_ui::on_continue, this));
}

void game_ui::update_countdown(const int value) const
{
  const auto text = string_format("%d", value);
  if (value >= 0)
  {
    countdown_label_->setString(text);
    countdown_label_->setVisible(true);
    if (value == 3)
    {
      level_name_label_->setVisible(true);
    }
  }
  else
  {
    countdown_label_->setString("GO!");

    const auto delay = DelayTime::create(1.0f);
    const auto fade_out = FadeOut::create(0.5);
    const auto remove = RemoveSelf::create(true);
    const auto vanish = Sequence::create(delay, fade_out, remove, nullptr);
    countdown_label_->runAction(vanish);
    level_name_label_->runAction(vanish->clone());
  }

  const auto scale_up = ScaleTo::create(0.25f, 1.5f, 1.5f);
  const auto scale_down = ScaleTo::create(0.25f, 1.0f, 1.0f);
  const auto scale = Sequence::create(scale_up, scale_down, nullptr);
  countdown_label_->runAction(scale);
}

void game_ui::display_pause_window() const
{
  pause_window_->display();
}


void game_ui::on_continue()
{
  if (continue_callback_ != nullptr)
  {
    audio_helper_->play_effect("sounds/select.mp3");
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
  case EventKeyboard::KeyCode::KEY_KP_ENTER:
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

void game_ui::on_controller_key_down(Controller* controller, int key_code, Event* event)
{
  switch (key_code)
  {
  case virtual_joy_stick::controller_start:
    on_key_pressed(EventKeyboard::KeyCode::KEY_ESCAPE, nullptr);
    break;
  case virtual_joy_stick::controller_button_a:
    on_key_pressed(EventKeyboard::KeyCode::KEY_ENTER, nullptr);
    break;
  case virtual_joy_stick::controller_back:
    on_key_pressed(EventKeyboard::KeyCode::KEY_F5, nullptr);
    break;
  default:
    break;
  }
}

bool game_ui::create_controller_listener()
{
  auto ret = false;

  do
  {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    const auto listener = EventListenerController::create();

    listener->onKeyDown = CC_CALLBACK_3(game_ui::on_controller_key_down, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    Controller::startDiscoveryController();
#endif
    ret = true;
  }
  while (false);

  return ret;
}
