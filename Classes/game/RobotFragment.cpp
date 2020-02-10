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

#include "RobotFragment.h"

#include "../utils/physics/PhysicsShapeCache.h"

RobotFragment::RobotFragment() : _smoke(nullptr), _exploding(false) {}

RobotFragment* RobotFragment::create(PhysicsShapeCache* physicsShapeCache, const int fragmentNumber)
{
  RobotFragment* ret = nullptr;

  do
  {
    auto object = new RobotFragment();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physicsShapeCache, fragmentNumber))
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

bool RobotFragment::createSmokeEmitter()
{
  auto ret = false;

  do
  {
    _smoke = ParticleFire::create();
    UTILS_BREAK_IF(_smoke == nullptr);

    _smoke->setDuration(ParticleSystem::DURATION_INFINITY);
    _smoke->setScale(2.f);
    _smoke->setBlendAdditive(true);
    _smoke->setOpacityModifyRGB(true);
    _smoke->setOpacity(127);
    _smoke->stop();
    _smoke->setAnchorPoint(Vec2(0.0f, 0.0f));

    const auto size = getContentSize();
    _smoke->setPosition(Vec2(size.width / 2, size.height / 2));

    ret = true;
  } while (false);

  return ret;
}

bool RobotFragment::init(PhysicsShapeCache* physicsShapeCache, const int fragmentNumber)
{
  auto ret = false;

  do
  {
    const auto shapeName = StringFormat("Fragment_%02d", fragmentNumber);
    const auto imageName = shapeName + ".png";

    UTILS_BREAK_IF(!BaseClass::init(physicsShapeCache, shapeName, imageName, "fragment"));

    setAnchorPoint(Vec2(0.5f, 0.5f));

    UTILS_BREAK_IF(!createSmokeEmitter());

    setVisible(false);
    addChild(_smoke);

    ret = true;
  } while (false);

  return ret;
}

void RobotFragment::explode(const Vec2& velocity)
{
  _exploding = true;
  setVisible(true);

  const auto body = getPhysicsBody();
  body->setRotationEnable(true);
  body->setDynamic(true);
  body->setVelocity(velocity);
  body->setMass(0.4f);
  body->setLinearDamping(0.5f);
  body->setAngularDamping(0.25f);

  const auto angularVelocity = random(2.5f, 4.5f) * (rand() % 2) * 2 - 1;
  body->setAngularVelocity(angularVelocity);

  _smoke->start();
}

void RobotFragment::pause()
{
  if (_smoke != nullptr)
  {
    _smoke->pause();
  }
}

void RobotFragment::update(float delta)
{
  if (_exploding)
  {
    const auto rotation = getRotation();
    _smoke->setRotation(-rotation);
  }
}
