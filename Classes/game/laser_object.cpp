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

#include "laser_object.h"

laser_object::laser_object() :
  angle_(0.f),
  draw_(nullptr),
  physics_world_(nullptr),
  spark_(nullptr)
{
}

laser_object* laser_object::create(const float initial_angle)
{
  laser_object* ret = nullptr;

  do
  {
    auto object = new laser_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(initial_angle))
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
bool laser_object::init(const float initial_angle)
{
  auto ret = false;

  do
  {
    angle_ = CC_DEGREES_TO_RADIANS(90-initial_angle);

    //////////////////////////////
    // 1. super init first
    UTILS_BREAK_IF(!base_class::init());

    draw_ = DrawNode::create();
    UTILS_BREAK_IF(draw_ == nullptr);

    addChild(draw_);

    const auto blink = Sequence::create(FadeTo::create(0.5f, 64), FadeTo::create(0.5f, 185.f), nullptr);
    const auto repeat_blink = RepeatForever::create(blink);
    draw_->runAction(repeat_blink);


    spark_ = ParticleFire::create();
    spark_->setDuration(ParticleSystem::DURATION_INFINITY);
    spark_->setBlendAdditive(true);
    spark_->setColor(Color3B::RED);
    spark_->setOpacityModifyRGB(true);
    spark_->setOpacity(127);
    spark_->setTotalParticles(5000);
    spark_->setPosition(this->getPosition());
    spark_->setEmissionRate(200.f);
    spark_->setGravity(Vec2(0.f, -1000.f));
    addChild(spark_);

    scheduleUpdate();

    ret = true;
  }
  while (false);

  return ret;
}

void laser_object::update(const float delta)
{
  // get the physicsWorld
  if (physics_world_ == nullptr)
  {
    physics_world_ = Director::getInstance()->getRunningScene()->getPhysicsWorld();
  }

  // clear our laser
  draw_->clear();

  // we star where the laser is
  const auto origin_point = Vec2::ZERO;
  auto origin_in_point_in_world = convertToWorldSpace(origin_point);

  // calculate distance base on the angle
  const auto distance = Vec2(max_laser_length * cosf(angle_), max_laser_length * sinf(angle_));

  // where the laser will reach
  const auto destination_point_in_world = origin_in_point_in_world + distance;

  // final point of the laser
  auto final_point_in_world = destination_point_in_world;

  // lambda that check all bodies that the laser touch and save the one closer to the origin
  const PhysicsRayCastCallbackFunc intersect_closer_body_func = [origin_in_point_in_world, &final_point_in_world]
  (PhysicsWorld& /*world*/, const PhysicsRayCastInfo& info, void* /*data*/)-> bool
  {
    auto origin = Point(origin_in_point_in_world);
    const auto new_point = Point(info.contact);
    const auto saved_point = Point(final_point_in_world);
    if (origin.distance(new_point) < origin.distance(saved_point))
    {
      final_point_in_world = info.contact;
    }
    // we need to check all bodies that touch so return true to continue
    return true;
  };

  // do the ray cast
  physics_world_->rayCast(intersect_closer_body_func, origin_in_point_in_world, destination_point_in_world, nullptr);

  // convert the point into node space and draw it
  const auto final_point = convertToNodeSpace(final_point_in_world);
  draw_->drawSegment(origin_point, final_point, 8, Color4F::RED);

  // if we have actually hit something draw a dot and create an emitter
  if ((final_point_in_world.x != destination_point_in_world.x) &
    (final_point_in_world.y != destination_point_in_world.y))
  {
    draw_->drawDot(final_point, 12, Color4F::RED);
    update_spark(final_point);
  }

  // rotate the laser angle
  angle_ += (5.f * static_cast<float>(M_PI) / 180.0f) * delta;
}

void laser_object::update_spark(const Vec2& point) const
{
  spark_->setPosition(point);
}
