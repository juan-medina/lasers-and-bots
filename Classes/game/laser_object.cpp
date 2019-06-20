/****************************************************************************
 Copyright (c) 2019 Juan Medina

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
#include "../scenes/game_scene.h"
#include "../utils/audio/audio_helper.h"
#include "../utils/base/nodes/custom_draw_node.h"
#include "robot_object.h"

int laser_object::loop_sound_ = -1;

laser_object::laser_object()
  : angle_(0.f)
  , initial_angle_(0)
  , final_angle_(0)
  , direction_(0)
  , draw_(nullptr)
  , physics_world_(nullptr)
  , spark_(nullptr)
  , speed_factor_(1.f)
  , audio_helper_(nullptr)
{
}

laser_object* laser_object::create(audio_helper* audio_helper, const float initial_angle,
                                   const float rotation_angle, const float speed_factor, const int damage)
{
  laser_object* ret = nullptr;

  do
  {
    auto object = new laser_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audio_helper, initial_angle, rotation_angle, speed_factor, damage))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  } while (false);

  return ret;
}

bool laser_object::init(audio_helper* audio_helper, const float initial_angle, const float rotation_angle,
                        const float speed_factor, const int damage)
{
  auto ret = false;

  do
  {
    audio_helper_ = audio_helper;
    initial_angle_ = CC_DEGREES_TO_RADIANS(90 - initial_angle);
    direction_ = -(rotation_angle / fabs(rotation_angle));
    final_angle_ = CC_DEGREES_TO_RADIANS((90 - initial_angle) - rotation_angle);
    angle_ = initial_angle_;

    speed_factor_ = speed_factor;

    UTILS_BREAK_IF(!base_class::init("laser", damage));

    draw_ = custom_draw_node::create();
    UTILS_BREAK_IF(draw_ == nullptr);

    addChild(draw_);

    const auto blink = Sequence::create(FadeTo::create(0.5f, 64), FadeTo::create(0.5f, 185.f), nullptr);
    const auto repeat_blink = RepeatForever::create(blink);
    draw_->runAction(repeat_blink);

    scheduleUpdate();

    audio_helper_->pre_load_effect("sounds/laser.mp3");

    ret = true;
  } while (false);

  return ret;
}

void laser_object::update(const float delta)
{
  // get the physicsWorld
  if (physics_world_ == nullptr)
  {
    physics_world_ = Director::getInstance()->getRunningScene()->getPhysicsWorld();
    return;
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

  // save the touched shape
  PhysicsShape* touch_shape = nullptr;

  // lambda that check all bodies that the laser touch and save the one closer
  // to the origin
  const PhysicsRayCastCallbackFunc intersect_closer_body_func =
    [origin_in_point_in_world, &final_point_in_world](PhysicsWorld& /*world*/, const PhysicsRayCastInfo& info,
                                                      void* out) -> bool {
    const auto origin = Point(origin_in_point_in_world);
    const auto new_point = Point(info.contact);
    const auto saved_point = Point(final_point_in_world);
    if (origin.distance(new_point) < origin.distance(saved_point))
    {
      final_point_in_world = info.contact;

      // save which shape we touch
      const auto dest_shape = static_cast<PhysicsShape**>(out);
      *dest_shape = info.shape;
    }
    // we need to check all bodies that touch so return true to continue
    return true;
  };

  // do the ray cast
  physics_world_->rayCast(intersect_closer_body_func, origin_in_point_in_world, destination_point_in_world,
                          &touch_shape);

  if (touch_shape != nullptr)
  {
    if (touch_shape->getCategoryBitmask() == static_cast<unsigned short>(game_scene::categories::robot))
    {
      const auto robot = dynamic_cast<robot_object*>(touch_shape->getBody()->getNode());
      robot->damage_shield(get_damage());
    }
  }

  // convert the point into node space and draw it
  const auto final_point = convertToNodeSpace(final_point_in_world);

  auto t = (final_point - origin_point).getNormalized();
  auto n = t.getPerp();

  static const auto radius = 12.f;

  n.scale(radius);
  t.scale(radius);

  const auto v0 = final_point - (n + t);
  const auto v1 = final_point + (n - t);
  const auto v2 = origin_point - (n + t);
  const auto v3 = origin_point + (n - t);  

  static const auto red = Color4F::RED;
  static const auto dark_red = Color4F(0.8, 0.f, 0.f, 1.f);
  Color4F colors[4] = {red, red, dark_red, dark_red};

  Vec2 vertex_1[4] = {origin_point, final_point, v0, v2};
  draw_->draw_color_quad(&vertex_1[0], &colors[0]);

  Vec2 vertex_2[4] = {origin_point, final_point, v1, v3};
  draw_->draw_color_quad(&vertex_2[0], &colors[0]);

  // if we have actually hit something draw a dot and create an emitter
  if ((final_point_in_world.x != destination_point_in_world.x) &
      (final_point_in_world.y != destination_point_in_world.y))
  {
    draw_->drawDot(final_point, 12, Color4F::RED);
    update_spark(final_point);
  }

  // rotate the laser angle
  angle_ += (5.f * static_cast<float>(M_PI) / 180.0f) * (delta * speed_factor_) * direction_;

  // if we need to swap direction
  auto swap_direction = false;

  if (direction_ < 0)
  {
    if (angle_ < final_angle_)
    {
      swap_direction = true;
    }
  }
  else
  {
    if (angle_ > final_angle_)
    {
      swap_direction = true;
    }
  }

  if (swap_direction)
  {
    angle_ = final_angle_;
    final_angle_ = initial_angle_;
    initial_angle_ = angle_;
    direction_ = -direction_;
  }
}

void laser_object::pause()
{
  base_class::pause();

  if (spark_ != nullptr)
  {
    spark_->pause();
  }

  draw_->pause();
  if (loop_sound_ != -1)
  {
    audio_helper_->stop_sound(loop_sound_);
    loop_sound_ = -1;
  }
}

void laser_object::resume()
{
  base_class::resume();

  if (spark_ != nullptr)
  {
    spark_->resume();
  }
  if (loop_sound_ == -1)
  {
    loop_sound_ = audio_helper_->play_effect("sounds/laser.mp3", true, 0.7f);
  }

  draw_->resume();
}

void laser_object::update_spark(const Vec2& point)
{
  if (spark_ == nullptr)
  {
    spark_ = ParticleFire::create();
    spark_->setDuration(ParticleSystem::DURATION_INFINITY);
    spark_->setBlendAdditive(true);
    spark_->setColor(Color3B::RED);
    spark_->setOpacityModifyRGB(true);
    spark_->setOpacity(127);
    spark_->setTotalParticles(5000);
    spark_->setEmissionRate(200.f);
    spark_->setGravity(Vec2(0.f, -1000.f));
    spark_->pause();
    addChild(spark_);

    if (loop_sound_ == -1)
    {
      loop_sound_ = audio_helper_->play_effect("sounds/laser.mp3", true, 0.7f);
    }
  }
  spark_->setPosition(point);
}
