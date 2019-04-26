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

#include "virtual_joy_stick.h"
#include "game_ui.h"

virtual_joy_stick::virtual_joy_stick():
  joy_stick_(nullptr),
  thumb_(nullptr),
  joy_stick_radius_(0.f),
  thumb_radius_(0.f),
  key_left_(false),
  key_right_(false),
  key_up_(false),
  key_down_(false),
  blind_y_(0.f),
  button_a_keyboard_(false),
  button_b_keyboard_(false)
{
}

virtual_joy_stick* virtual_joy_stick::create(float blind_y)
{
  virtual_joy_stick* ret = nullptr;

  do
  {
    auto object = new virtual_joy_stick();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(blind_y))
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

bool virtual_joy_stick::init(float blind_y)
{
  auto ret = false;

  do
  {
    blind_y_ = blind_y;

    //////////////////////////////
    // 1. super init first
    UTILS_BREAK_IF(!base_class::init());

    // joystick
    const auto scale = 1.0f;
    joy_stick_ = Sprite::createWithSpriteFrameName("02_joystick_background.png");
    UTILS_BREAK_IF(joy_stick_ == nullptr);

    joy_stick_->setOpacity(127);
    joy_stick_->setPosition(0.f, 0.f);
    joy_stick_->setVisible(false);
    joy_stick_->setScale(scale);
    joy_stick_radius_ = joy_stick_->getContentSize().width * joy_stick_->getScale();

    addChild(joy_stick_, 0);

    thumb_ = Sprite::createWithSpriteFrameName("02_joystick_thumb.png");
    UTILS_BREAK_IF(thumb_ == nullptr);

    thumb_->setOpacity(127);
    thumb_->setPosition(0.f, 0.f);
    thumb_->setVisible(false);
    thumb_->setScale(scale);
    thumb_radius_ = thumb_->getContentSize().width * thumb_->getScale();

    addChild(thumb_, 1);

    UTILS_BREAK_IF(!create_touch_listener());

    UTILS_BREAK_IF(!create_keyboard_listener());

    ret = true;
  }
  while (false);

  return ret;
}

Vec2 virtual_joy_stick::get_location_in_node_space(Touch* touch)
{
  const auto point_one = Director::getInstance()->convertToUI(touch->getLocationInView());
  const auto ui = dynamic_cast<game_ui*>(getParent());
  const auto location = convertToNodeSpace(point_one) + ui->getPosition();
  return location;
}

bool virtual_joy_stick::on_touch_began(Touch* touch, Event* unused_event)
{
  const auto location = get_location_in_node_space(touch);

  if (location.y > blind_y_)
  {
    return false;
  }

  joy_stick_->setVisible(true);
  joy_stick_->setPosition(location);
  thumb_->setVisible(true);
  thumb_->setPosition(location);

  return true;
}

void virtual_joy_stick::on_touch_moved(Touch* touch, Event* unused_event)
{
  const auto location = get_location_in_node_space(touch);
  update_velocity(location);
}

void virtual_joy_stick::on_touch_ended(Touch* touch, Event* unused_event)
{
  joy_stick_->setVisible(false);
  thumb_->setVisible(false);
  velocity_ = Vec2::ZERO;
}

void virtual_joy_stick::on_touch_cancel(Touch* touch, Event* unused_event)
{
  on_touch_ended(touch, unused_event);
}

bool virtual_joy_stick::create_touch_listener()
{
  auto ret = false;

  do
  {
    // Register Touch Event
    const auto listener = EventListenerTouchOneByOne::create();
    UTILS_BREAK_IF(listener == nullptr);

    listener->setSwallowTouches(false);

    listener->onTouchBegan = CC_CALLBACK_2(virtual_joy_stick::on_touch_began, this);
    listener->onTouchEnded = CC_CALLBACK_2(virtual_joy_stick::on_touch_ended, this);
    listener->onTouchMoved = CC_CALLBACK_2(virtual_joy_stick::on_touch_moved, this);
    listener->onTouchCancelled = CC_CALLBACK_2(virtual_joy_stick::on_touch_cancel, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    ret = true;
  }
  while (false);

  return ret;
}

void virtual_joy_stick::update_velocity(Point point)
{
  const auto first_tap_location = joy_stick_->getPosition();

  // calculate Angle and length
  auto distance_x = point.x - first_tap_location.x;
  auto distance_y = point.y - first_tap_location.y;

  const auto distance = sqrtf(distance_x * distance_x + distance_y * distance_y);
  const auto angle = atan2f(distance_y, distance_x); // in radians


  if (distance > joy_stick_radius_)
  {
    distance_x = cosf(angle) * joy_stick_radius_;
    distance_y = sinf(angle) * joy_stick_radius_;
  }

  if (distance > thumb_radius_)
  {
    point.x = first_tap_location.x + cosf(angle) * thumb_radius_;
    point.y = first_tap_location.y + sinf(angle) * thumb_radius_;
  }

  thumb_->setPosition(point);

  distance_x = point.x - first_tap_location.x;
  distance_y = point.y - first_tap_location.y;

  velocity_ = Vec2((point.x - first_tap_location.x) / thumb_radius_, (point.y - first_tap_location.y) / thumb_radius_);
}

bool virtual_joy_stick::create_keyboard_listener()
{
  auto result = false;

  do
  {
    auto listener = EventListenerKeyboard::create();
    UTILS_BREAK_IF(listener == nullptr);

    listener->onKeyPressed = CC_CALLBACK_2(virtual_joy_stick::on_key_pressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(virtual_joy_stick::on_key_released, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    result = true;
  }
  while (false);

  return result;
}

void virtual_joy_stick::on_key_pressed(const EventKeyboard::KeyCode key_code, Event*)
{
  switch (key_code)
  {
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
  case EventKeyboard::KeyCode::KEY_W:
    key_up_ = true;
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    key_left_ = true;;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    key_right_ = true;
    break;
  case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
  case EventKeyboard::KeyCode::KEY_S:
    key_down_ = true;
    break;
  case EventKeyboard::KeyCode::KEY_SPACE:
    button_a_keyboard_ = true;
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_CTRL:
  case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
    button_b_keyboard_ = true;
    break;
  default:
    break;
  }
}

void virtual_joy_stick::on_key_released(const EventKeyboard::KeyCode key_code, Event*)
{
  switch (key_code)
  {
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
  case EventKeyboard::KeyCode::KEY_W:
    key_up_ = false;
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    key_left_ = false;;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    key_right_ = false;
    break;
  case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
  case EventKeyboard::KeyCode::KEY_S:
    key_down_ = false;
    break;
  case EventKeyboard::KeyCode::KEY_SPACE:
    button_a_keyboard_ = false;
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_CTRL:
  case EventKeyboard::KeyCode::KEY_RIGHT_CTRL:
    button_b_keyboard_ = false;
    break;
  default:
    break;
  }
}
