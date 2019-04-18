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

const Vec2 robot_object::normal_movement = Vec2(1000.0f, 2600.0f);

robot_object::robot_object() :
  to_left_(false),
  to_right_(false),
  current_state_(e_idle),
  virtual_joy_stick_(nullptr)
{
}

robot_object* robot_object::create(virtual_joy_stick* virtual_joy_stick)
{
  robot_object* ret = nullptr;

  do
  {
    auto object = new robot_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(virtual_joy_stick))
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
bool robot_object::init(virtual_joy_stick* virtual_joy_stick)
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

    auto bot_size = getContentSize();

    bot_size.width *= 0.4f;
    bot_size.height *= 0.85f;

    auto body = PhysicsBody::createBox(bot_size, PhysicsMaterial(0.1f, 0.5f, 0.5f));
    UTILS_BREAK_IF(body == nullptr);

    body->setDynamic(true);
    body->setRotationEnable(false);
    body->setMass(1.0);
    body->setMoment(PHYSICS_INFINITY);
    setPhysicsBody(body);

    UTILS_BREAK_IF(!create_anim("Idle_%02d.png", 10, 0.05f, "idle"));
    UTILS_BREAK_IF(!create_anim("Run_%02d.png", 8, 0.15f, "run"));
    UTILS_BREAK_IF(!create_anim("Jump_%02d.png", 10, 0.15f, "jump", 1));

    change_anim("idle");

    virtual_joy_stick_ = virtual_joy_stick;
    ret = true;
  }
  while (false);

  return ret;
}

void robot_object::update(float delta)
{
  move_to_left(virtual_joy_stick_->get_left());
  move_to_right(virtual_joy_stick_->get_right());

  if (virtual_joy_stick_->get_up())
  {
    static auto next_jump = -1.f;
    if(next_jump<=0)
    {
      jump();
      next_jump = 0.15;
    }
    else
    {
      next_jump -= delta;
    }
    
  }

  const auto move_x = ((to_left_ ? 0.0f : 1.0f) - (to_right_ ? 0.0f : 1.0f)) * normal_movement.x;
  const auto move_y = getPhysicsBody()->getVelocity().y;

  getPhysicsBody()->setVelocity(Vec2(move_x, move_y));

  change_state(decide_state());
}

robot_object::state robot_object::decide_state() const
{
  const auto velocity = getPhysicsBody()->getVelocityAtLocalPoint(Vec2::ZERO);

  auto wanted_state = current_state_;

  if (fuzzy_equals(velocity.y, 0.0f, 0.1f) && fuzzy_equals(velocity.x, 0.0f, 0.f))
  {
    wanted_state = e_idle;
  }
  else
  {
    if (!fuzzy_equals(fabs(velocity.y), 0.0f))
    {
      wanted_state = e_jumping;
    }
    else
    {
      wanted_state = e_running;
    }
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
      break;
    case e_idle:
      change_anim("idle");
      break;
    case e_running:
      change_anim("run");
      break;
    default:
      break;
    }
  }
}

void robot_object::move_to_left(const bool to_left)
{
  if (to_left)
  {
    setFlippedX(true);
  }

  to_left_ = to_left;
}

void robot_object::move_to_right(const bool to_right)
{
  if (to_right)
  {
    setFlippedX(false);
  }

  to_right_ = to_right;
}

void robot_object::jump()
{
  if (current_state_ != e_jumping)
  {
      getPhysicsBody()->applyImpulse(Vec2(0.0f, normal_movement.y));    
  }
}
