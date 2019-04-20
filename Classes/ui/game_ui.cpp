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
#include "virtual_joy_stick.h"

game_ui::game_ui():
  virtual_joy_stick_(nullptr),
  shield_bar_(nullptr)
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

    // cache
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui.plist");

    const auto close = Sprite::createWithSpriteFrameName("01_Exit_1.png");
    UTILS_BREAK_IF(close==nullptr);

    const auto close_click = Sprite::createWithSpriteFrameName("01_Exit_2.png");
    UTILS_BREAK_IF(close_click == nullptr);

    const auto close_item = MenuItemSprite::create(close, close_click, CC_CALLBACK_1(game_ui::on_close, this));
    UTILS_BREAK_IF(close_item == nullptr);

    close_item->setPosition(size.width / 2 - close->getContentSize().width,
                            size.height / 2 - close->getContentSize().height);

    const auto menu = Menu::create(close_item, nullptr);
    UTILS_BREAK_IF(menu == nullptr);

    addChild(menu);

    // joystick
    virtual_joy_stick_ = virtual_joy_stick::create(size.height - 500.f);
    addChild(virtual_joy_stick_);

    const auto head_pos = Vec2(50.f, size.height - 50);

    auto head = Sprite::createWithSpriteFrameName("03_head.png");
    UTILS_BREAK_IF(head == nullptr);

    head->setAnchorPoint(Vec2(0, 1));
    head->setPosition(head_pos);
    addChild(head);

    const auto bar_sprite = Sprite::createWithSpriteFrameName("04_bar.png");
    UTILS_BREAK_IF(bar_sprite == nullptr);
    bar_sprite->setBlendFunc(BlendFunc::ADDITIVE);
    bar_sprite->setOpacity(190);

    shield_bar_ = ProgressTimer::create(bar_sprite);
    UTILS_BREAK_IF(shield_bar_ == nullptr);

    shield_bar_->setType(ProgressTimer::Type::BAR);
    shield_bar_->setMidpoint(Vec2(0, 0));
    shield_bar_->setBarChangeRate(Vec2(1, 0));
    shield_bar_->setPercentage(100.f);
    shield_bar_->setAnchorPoint(Vec2(0, 1));
    shield_bar_->setPosition(head_pos);

    addChild(shield_bar_);

    ret = true;
  }
  while (false);

  return ret;
}

void game_ui::on_close(Ref* sender)
{
  //Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();

  audio_helper::get_instance()->play_effect("sounds/select.ogg");
}
