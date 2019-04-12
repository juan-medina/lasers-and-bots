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
#include "utils/audio/audio_helper.h"

game_ui::game_ui() :
  listener_(nullptr),
  joy_stick_(nullptr),
  thumb_(nullptr)
{
}

game_ui::~game_ui()
{
  // if we have a listener
  if (listener_ != nullptr)
  {
    // remove the listener and release it
    _eventDispatcher->removeEventListener(listener_);
    listener_->release();
    listener_ = nullptr;
  }
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

    audio_helper::pre_load_music("sounds/select.wav");

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
    const auto scale = 1.0f;
    joy_stick_ = Sprite::createWithSpriteFrameName("02_joystick_background.png");
    UTILS_BREAK_IF(joy_stick_ == nullptr);

    joy_stick_->setOpacity(127);
    joy_stick_->setPosition(size.width / 2, size.height / 2);
    joy_stick_->setVisible(false);
    joy_stick_->setScale(scale);

    addChild(joy_stick_, 0);

    thumb_ = Sprite::createWithSpriteFrameName("02_joystick_thumb.png");
    UTILS_BREAK_IF(thumb_ == nullptr);

    thumb_->setOpacity(127);
    thumb_->setPosition(size.width / 2, size.height / 2);
    thumb_->setVisible(false);
    thumb_->setScale(scale);

    addChild(thumb_, 1);

    register_touch();

    ret = true;
  }
  while (false);

  return ret;
}

void game_ui::on_close(Ref* sender)
{
  //Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();

  audio_helper::get_instance()->play_effect("sounds/select.wav");
}

Vec2 game_ui::get_location_in_node_space(Touch* touch) const
{
  const auto point_one = Director::getInstance()->convertToUI(touch->getLocationInView());
  const auto location = convertToNodeSpace(point_one) + getPosition();
  return location;
}

bool game_ui::on_touch_began(Touch* touch, Event* unused_event) const
{
  const auto location = get_location_in_node_space(touch);

  joy_stick_->setVisible(true);
  joy_stick_->setPosition(location);
  thumb_->setVisible(true);
  thumb_->setPosition(location);

  return true;
}

void game_ui::on_touch_moved(Touch* touch, Event* unused_event)
{
  const auto location = get_location_in_node_space(touch);
  update_velocity(location);
}

void game_ui::on_touch_ended(Touch* touch, Event* unused_event)
{
  joy_stick_->setVisible(false);
  thumb_->setVisible(false);
  velocity_ = Vec2::ZERO;
}

void game_ui::on_touch_cancel(Touch* touch, Event* unused_event)
{
  on_touch_ended(touch, unused_event);
}

void game_ui::register_touch()
{
  // Register Touch Event
  listener_ = EventListenerTouchOneByOne::create();
  listener_->retain();
  listener_->setSwallowTouches(false);

  listener_->onTouchBegan = CC_CALLBACK_2(game_ui::on_touch_began, this);
  listener_->onTouchEnded = CC_CALLBACK_2(game_ui::on_touch_ended, this);
  listener_->onTouchMoved = CC_CALLBACK_2(game_ui::on_touch_moved, this);
  listener_->onTouchCancelled = CC_CALLBACK_2(game_ui::on_touch_cancel, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener_, this);
}

void game_ui::update_velocity(Point point)
{
  const auto first_tap_location = joy_stick_->getPosition();

  // calculate Angle and length
  auto distance_x = point.x - first_tap_location.x;
  auto distance_y = point.y - first_tap_location.y;

  const auto distance = sqrtf(distance_x * distance_x + distance_y * distance_y);
  const auto angle = atan2f(distance_y, distance_x); // in radians

  const auto joy_stick_radius = joy_stick_->getContentSize().width * joy_stick_->getScale();

  if (distance > joy_stick_radius)
  {
    distance_x = cosf(angle) * joy_stick_radius;
    distance_y = sinf(angle) * joy_stick_radius;
  }

  velocity_ = Vec2(distance_x / joy_stick_radius, distance_y / joy_stick_radius);

  const auto thumb_radius = thumb_->getContentSize().width * thumb_->getScale();

  if (distance > thumb_radius)
  {
    point.x = first_tap_location.x + cosf(angle) * thumb_radius;
    point.y = first_tap_location.y + sinf(angle) * thumb_radius;
  }

  thumb_->setPosition(point);
}
