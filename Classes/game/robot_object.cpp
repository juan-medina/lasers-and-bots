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
#include "../utils/physics/physics_shape_cache.h"
#include "Particle3D/PU/CCPUColorAffector.h"

const Vec2 robot_object::normal_movement = Vec2(1000.0f, 400.0f);

robot_object::robot_object() :
  to_left_(false),
  to_right_(false),
  jumping_(false),
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

    const auto cache = physics_shape_cache::get_instance();
    auto body = cache->create_body_with_name("Robot");
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
  jump(virtual_joy_stick_->get_up());

  change_state(decide_state());
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

void robot_object::move_robot() const
{
  const auto move_x = ((to_left_ ? 0.0f : 1.0f) - (to_right_ ? 0.0f : 1.0f)) * normal_movement.x;
  const auto move_y = getPhysicsBody()->getVelocity().y;
  getPhysicsBody()->setVelocity(Vec2(move_x, move_y));
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
  if (to_jump)
  {
    if (!jumping_)
    {
      getPhysicsBody()->applyImpulse(Vec2(0.0f, normal_movement.y));
      jumping_ = true;
    }
  }

  const auto center_point = Vec2(getContentSize().width / 2.f, getContentSize().height / 2);
  const auto left_point = Vec2((getContentSize().width / 2.f) - 50, getContentSize().height / 2);
  const auto right_point = Vec2((getContentSize().width / 2.f) + 50, getContentSize().height / 2);

  if ((on_top_of_block(center_point)) || ((on_top_of_block(left_point))) || (on_top_of_block(right_point)))
  {
    jumping_ = false;
  }
}

bool robot_object::on_top_of_block(const Vec2& origin_point) const
{
  auto result = false;
  const auto world = Director::getInstance()->getRunningScene()->getPhysicsWorld();

  if (world != nullptr)
  {
    if (jumping_)
    {
      // we star where the robot is      
      auto origin_in_point_in_world = convertToWorldSpace(origin_point);

      // we look down
      const auto distance = Vec2(0.f, -1000.f);

      // where  will reach
      const auto destination_point_in_world = origin_in_point_in_world + distance;

      // final point 
      auto final_point_in_world = destination_point_in_world;

      // we want only to find things that we could have collision
      auto robot_mask = getPhysicsBody()->getCollisionBitmask();

      // lambda that check all bodies that save the one closer to the origin
      const PhysicsRayCastCallbackFunc intersect_closer_body_func = [origin_in_point_in_world, &final_point_in_world,
          robot_mask]
      (PhysicsWorld& /*world*/, const PhysicsRayCastInfo& info, void* /*data*/)-> bool
      {
        if (info.shape->getCategoryBitmask() & robot_mask)
        {
          auto origin = Point(origin_in_point_in_world);
          const auto new_point = Point(info.contact);
          const auto saved_point = Point(final_point_in_world);
          if (origin.distance(new_point) < origin.distance(saved_point))
          {
            final_point_in_world = info.contact;
          }
        }
        // we need to check all bodies that touch so return true to continue
        return true;
      };

      // do the ray cast
      world->rayCast(intersect_closer_body_func, origin_in_point_in_world, destination_point_in_world, nullptr);

      // if we hit any point
      const auto final_point = convertToNodeSpace(final_point_in_world);

      const auto distance_to_anything = origin_point.distance(final_point);
      if (distance_to_anything < (getContentSize().height / 2.f))
      {
        result = true;
      }
    }
  }
  return result;
}
