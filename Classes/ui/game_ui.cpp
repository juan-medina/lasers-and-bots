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
  pause_item_(nullptr)
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

    const auto play = Sprite::createWithSpriteFrameName("06_Play_1.png");
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
    const auto reload = Sprite::createWithSpriteFrameName("06_Reload_1.png");
    UTILS_BREAK_IF(reload == nullptr);

    const auto reload_click = Sprite::createWithSpriteFrameName("06_Reload_2.png");
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

    shield_label_ = Label::createWithBMFont("fonts/general_72.fnt", "100 %", TextHAlignment::CENTER);
    UTILS_BREAK_IF(shield_label_ == nullptr);

    shield_label_->setColor(Color3B(0, 255, 255));
    shield_label_->setPosition(bar_pos);

    addChild(shield_label_);

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
