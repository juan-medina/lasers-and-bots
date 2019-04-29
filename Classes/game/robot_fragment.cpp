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

#include "robot_fragment.h"
#include "../utils/physics/physics_shape_cache.h"

robot_fragment::robot_fragment():
  smoke_(nullptr),
  exploding_(false)
{
}

robot_fragment* robot_fragment::create(const int fragment_number)
{
  robot_fragment* ret = nullptr;

  do
  {
    auto object = new robot_fragment();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(fragment_number))
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

bool robot_fragment::create_smoke_emitter()
{
  auto ret = false;

  do
  {
    smoke_ = ParticleFire::create();
    UTILS_BREAK_IF(smoke_ == nullptr);

    smoke_->setDuration(ParticleSystem::DURATION_INFINITY);
    smoke_->setScale(2.f);
    smoke_->setBlendAdditive(true);
    smoke_->setOpacityModifyRGB(true);
    smoke_->setOpacity(127);
    smoke_->stop();
    smoke_->setAnchorPoint(Vec2(0.0f, 0.0f));

    const auto size = getContentSize();
    smoke_->setPosition(Vec2(size.width / 2, size.height / 2));

    ret = true;
  }
  while (false);

  return ret;
}

bool robot_fragment::init(const int fragment_number)
{
  auto ret = false;

  do
  {
    const auto shape_name = string_format("Fragment_%02d", fragment_number);
    const auto image_name = shape_name + ".png";

    UTILS_BREAK_IF(!base_class::init(image_name, "fragment"));

    setAnchorPoint(Vec2(0.5f, 0.5f));

    set_shape(shape_name);

    UTILS_BREAK_IF(!create_smoke_emitter());

    setVisible(false);
    addChild(smoke_);

    ret = true;
  }
  while (false);

  return ret;
}

void robot_fragment::explode(const Vec2& velocity)
{
  exploding_ = true;
  setVisible(true);

  const auto body = getPhysicsBody();
  body->setRotationEnable(true);
  body->setDynamic(true);
  body->setVelocity(velocity);
  body->setMass(0.4f);
  body->setLinearDamping(0.5f);
  body->setAngularDamping(0.25f);

  const auto angular_velocity = random(2.5f, 4.5f) * (rand() % 2) * 2 - 1;
  body->setAngularVelocity(angular_velocity);

  smoke_->start();
}

void robot_fragment::pause()
{
  if (smoke_ != nullptr)
  {
    smoke_->pause();
  }
}

void robot_fragment::update(float delta)
{
  if (exploding_)
  {
    const auto rotation = getRotation();
    smoke_->setRotation(-rotation);
  }
}
