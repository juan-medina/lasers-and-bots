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

#include "robot_object.h"
#include "../ui/virtual_joy_stick.h"
#include "../utils/audio/audio_helper.h"

const Vec2 robot_object::normal_movement = Vec2(1000.0f, 2600.f);
const int robot_object::blink_on_damage_action_tag = 0xF0F0F;

robot_object::robot_object() :
  to_left_(false),
  to_right_(false),
  jumping_(false),
  current_state_(e_idle),
  virtual_joy_stick_(nullptr),
  walk_sound_(-1),
  current_shield_(0),
  max_shield_(0)
{
}

robot_object* robot_object::create(virtual_joy_stick* virtual_joy_stick, const int max_shield)
{
  robot_object* ret = nullptr;

  do
  {
    auto object = new robot_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(virtual_joy_stick, max_shield))
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
bool robot_object::init(virtual_joy_stick* virtual_joy_stick, const int max_shield)
{
  auto ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first

    // cache
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot_0.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot_1.plist");

    UTILS_BREAK_IF(!base_class::init("Idle_01.png", "robot"));

    setAnchorPoint(Vec2(0.5f, 0.0f));

    UTILS_BREAK_IF(!set_shape("Robot"));

    auto body = getPhysicsBody();
    body->setMass(1.0f);
    body->setMoment(PHYSICS_INFINITY);

    UTILS_BREAK_IF(!create_anim("Idle_%02d.png", 10, 0.05f, "idle"));
    UTILS_BREAK_IF(!create_anim("Run_%02d.png", 8, 0.15f, "run"));
    UTILS_BREAK_IF(!create_anim("Jump_%02d.png", 10, 0.15f, "jump", 1));

    audio_helper::pre_load_effect("sounds/metal_footsteps.mp3");
    audio_helper::pre_load_effect("sounds/jump.mp3");
    audio_helper::pre_load_effect("sounds/land.mp3");

    change_anim("idle");

    virtual_joy_stick_ = virtual_joy_stick;

    current_shield_ = max_shield;
    max_shield_ = max_shield;

    ret = true;
  }
  while (false);

  return ret;
}

void robot_object::update(float delta)
{
  move_to_left(virtual_joy_stick_->left());
  move_to_right(virtual_joy_stick_->right());
  jump(virtual_joy_stick_->up() || virtual_joy_stick_->button_a());

  change_state(decide_state());
}

void robot_object::on_land_on_block()
{
  if (jumping_)
  {
    audio_helper::get_instance()->play_effect("sounds/land.mp3", false, 0.35f);
  }
  jumping_ = false;
}

float robot_object::get_shield_percentage() const
{
  const auto ratio = static_cast<float>(current_shield_) / max_shield_;
  const auto amount = ratio * 100.f;
  return amount;
}


robot_object::state robot_object::decide_state() const
{
  auto wanted_state = e_idle;

  if (jumping_)
  {
    wanted_state = e_jumping;
  }
  else if (to_left_ || to_right_)
  {
    wanted_state = e_running;
  }

  return wanted_state;
}

void robot_object::change_state(const state wanted_state)
{
  if (wanted_state != current_state_)
  {
    current_state_ = wanted_state;
    switch (current_state_)
    {
    case e_jumping:
      change_anim("jump");
      walk_sound(false);
      break;
    case e_idle:
      change_anim("idle");
      walk_sound(false);
      break;
    case e_running:
      change_anim("run");
      walk_sound(true);
      break;
    default:
      break;
    }
  }
}

void robot_object::move_robot() const
{
  const auto move_x = ((to_left_ ? 0.0f : 1.0f) - (to_right_ ? 0.0f : 1.0f)) * normal_movement.x;
  const auto move_y = getPhysicsBody()->getVelocity().y;
  getPhysicsBody()->setVelocity(Vec2(move_x, move_y));
}

void robot_object::damage_shield(const int amount)
{
  current_shield_ -= amount;
  if (current_shield_ < 0)
  {
    current_shield_ = 0;
  }

  if (getActionByTag(blink_on_damage_action_tag) == nullptr)
  {
    const auto ink_to_damaged = TintTo::create(0.1f, Color3B::RED);
    const auto ink_to_normal = TintTo::create(0.1f, Color3B::WHITE);
    const auto sequence = Sequence::create(ink_to_damaged, ink_to_normal, nullptr);
    const auto repeat = Repeat::create(sequence, 5);
    repeat->setTag(blink_on_damage_action_tag);
    runAction(repeat);
  }
}

void robot_object::pause()
{
  base_class::pause();
  if (walk_sound_ != -1)
  {
    audio_helper::get_instance()->stop_sound(walk_sound_);
    walk_sound_ = -1;
  }
}

void robot_object::resume()
{
  base_class::resume();
}

void robot_object::move_to_left(const bool to_left)
{
  if (to_left)
  {
    setFlippedX(true);
    getPhysicsBody()->setPositionOffset(Vec2(40.0f, 0.0f));
  }

  to_left_ = to_left;
  move_robot();
}

void robot_object::move_to_right(const bool to_right)
{
  if (to_right)
  {
    setFlippedX(false);
    getPhysicsBody()->setPositionOffset(Vec2::ZERO);
  }

  to_right_ = to_right;
  move_robot();
}

void robot_object::jump(const bool to_jump)
{
  static auto jump_pressed = false;
  if (to_jump)
  {
    if (!jump_pressed)
    {
      if (!jumping_)
      {
        audio_helper::get_instance()->play_effect("sounds/jump.mp3", false, 0.35f);
        getPhysicsBody()->applyImpulse(Vec2(0.0f, normal_movement.y));
        jumping_ = true;
      }
      jump_pressed = true;
    }
  }
  else
  {
    jump_pressed = false;
  }
}

void robot_object::walk_sound(const bool active)
{
  if (active)
  {
    if (walk_sound_ == -1)
    {
      walk_sound_ = audio_helper::get_instance()->play_effect("sounds/metal_footsteps.mp3", true);
    }
    else
    {
      audio_helper::get_instance()->resume_sound(walk_sound_);
    }
  }
  else
  {
    if (walk_sound_ != -1)
    {
      audio_helper::get_instance()->pause_sound(walk_sound_);
    }
  }
}
