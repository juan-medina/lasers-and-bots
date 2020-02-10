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

#include "LaserObject.h"

#include "../utils/audio/AudioHelper.h"
#include "../utils/base/nodes/CustomDrawNode.h"
#include "RobotObject.h"
#include "scenes/GameScene.h"

int LaserObject::LOOP_SOUND = -1;

LaserObject::LaserObject()
  : _angle(0.f)
  , _initialAngle(0)
  , _finalAngle(0)
  , _direction(0)
  , _draw(nullptr)
  , _physicsWorld(nullptr)
  , _spark(nullptr)
  , _speedFactor(1.f)
  , _audioHelper(nullptr)
{
}

LaserObject* LaserObject::create(AudioHelper* audioHelper, const float initialAngle,
                                 const float rotationAngle, const float speedFactor, const int damage)
{
  LaserObject* ret = nullptr;

  do
  {
    auto object = new LaserObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audioHelper, initialAngle, rotationAngle, speedFactor, damage))
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

bool LaserObject::init(AudioHelper* audioHelper, const float initialAngle, const float rotationAngle,
                       const float speedFactor, const int damage)
{
  auto ret = false;

  do
  {
    _audioHelper = audioHelper;
    _initialAngle = CC_DEGREES_TO_RADIANS(90 - initialAngle);
    _direction = -(rotationAngle / fabs(rotationAngle));
    _finalAngle = CC_DEGREES_TO_RADIANS((90 - initialAngle) - rotationAngle);
    _angle = _initialAngle;

    _speedFactor = speedFactor;

    UTILS_BREAK_IF(!BaseClass::init("laser", damage));

    _draw = CustomDrawNode::create();
    UTILS_BREAK_IF(_draw == nullptr);

    addChild(_draw);

    const auto blink = Sequence::create(FadeTo::create(0.5f, 64), FadeTo::create(0.5f, 185.f), nullptr);
    const auto repeatBlink = RepeatForever::create(blink);
    _draw->runAction(repeatBlink);

    scheduleUpdate();

    _audioHelper->preLoadEffect("sounds/laser.mp3");

    ret = true;
  } while (false);

  return ret;
}

void LaserObject::update(const float delta)
{
  // get the physicsWorld
  if (_physicsWorld == nullptr)
  {
    _physicsWorld = Director::getInstance()->getRunningScene()->getPhysicsWorld();
    return;
  }

  // clear our laser
  _draw->clear();

  // we star where the laser is
  const auto originPoint = Vec2::ZERO;
  auto originInPointInWorld = convertToWorldSpace(originPoint);

  // calculate distance base on the angle
  const auto distance = Vec2(MAX_LASER_LENGTH * cosf(_angle), MAX_LASER_LENGTH * sinf(_angle));

  // where the laser will reach
  const auto destinationPointInWorld = originInPointInWorld + distance;

  // final point of the laser
  auto finalPointInWorld = destinationPointInWorld;

  // save the touched shape
  PhysicsShape* touchShape = nullptr;

  // lambda that check all bodies that the laser touch and save the one closer
  // to the origin
  const PhysicsRayCastCallbackFunc intersectCloserBodyFunc =
    [originInPointInWorld, &finalPointInWorld](PhysicsWorld& /*world*/, const PhysicsRayCastInfo& info,
                                               void* out) -> bool {
    const auto origin = Point(originInPointInWorld);
    const auto newPoint = Point(info.contact);
    const auto savedPoint = Point(finalPointInWorld);
    if (origin.distance(newPoint) < origin.distance(savedPoint))
    {
      finalPointInWorld = info.contact;

      // save which shape we touch
      const auto destShape = static_cast<PhysicsShape**>(out);
      *destShape = info.shape;
    }
    // we need to check all bodies that touch so return true to continue
    return true;
  };

  // do the ray cast
  _physicsWorld->rayCast(intersectCloserBodyFunc, originInPointInWorld, destinationPointInWorld, &touchShape);

  if (touchShape != nullptr)
  {
    if (touchShape->getCategoryBitmask() == static_cast<unsigned short>(GameScene::Categories::robot))
    {
      const auto robot = dynamic_cast<RobotObject*>(touchShape->getBody()->getNode());
      robot->damageShield(getDamage());
    }
  }

  // convert the point into node space and draw it
  const auto finalPoint = convertToNodeSpace(finalPointInWorld);

  auto t = (finalPoint - originPoint).getNormalized();
  auto n = t.getPerp();

  static const auto RADIUS = 12.f;

  n.scale(RADIUS);
  t.scale(RADIUS);

  const auto v0 = finalPoint - (n + t);
  const auto v1 = finalPoint + (n - t);
  const auto v2 = originPoint - (n + t);
  const auto v3 = originPoint + (n - t);

  static const auto RED = Color4F::RED;
  static const auto DARK_RED = Color4F(0.8f, 0.f, 0.f, 1.f);
  Color4F colors[4] = {RED, RED, DARK_RED, DARK_RED};

  Vec2 vertex1[4] = {originPoint, finalPoint, v0, v2};
  _draw->drawColorQuad(&vertex1[0], &colors[0]);

  Vec2 vertex2[4] = {originPoint, finalPoint, v1, v3};
  _draw->drawColorQuad(&vertex2[0], &colors[0]);

  // if we have actually hit something draw a dot and create an emitter
  if ((finalPointInWorld.x != destinationPointInWorld.x) & (finalPointInWorld.y != destinationPointInWorld.y))
  {
    _draw->drawDot(finalPoint, 12, Color4F::RED);
    updateSpark(finalPoint);
  }

  // rotate the laser angle
  _angle += (5.f * static_cast<float>(M_PI) / 180.0f) * (delta * _speedFactor) * _direction;

  // if we need to swap direction
  auto swapDirection = false;

  if (_direction < 0)
  {
    if (_angle < _finalAngle)
    {
      swapDirection = true;
    }
  }
  else
  {
    if (_angle > _finalAngle)
    {
      swapDirection = true;
    }
  }

  if (swapDirection)
  {
    _angle = _finalAngle;
    _finalAngle = _initialAngle;
    _initialAngle = _angle;
    _direction = -_direction;
  }
}

void LaserObject::pause()
{
  BaseClass::pause();

  if (_spark != nullptr)
  {
    _spark->pause();
  }

  _draw->pause();
  if (LOOP_SOUND != -1)
  {
    _audioHelper->stopSound(LOOP_SOUND);
    LOOP_SOUND = -1;
  }
}

void LaserObject::resume()
{
  BaseClass::resume();

  if (_spark != nullptr)
  {
    _spark->resume();
  }
  if (LOOP_SOUND == -1)
  {
    LOOP_SOUND = _audioHelper->playEffect("sounds/laser.mp3", true, 0.7f);
  }

  _draw->resume();
}

void LaserObject::updateSpark(const Vec2& point)
{
  if (_spark == nullptr)
  {
    _spark = ParticleFire::create();
    _spark->setDuration(ParticleSystem::DURATION_INFINITY);
    _spark->setBlendAdditive(true);
    _spark->setColor(Color3B::RED);
    _spark->setOpacityModifyRGB(true);
    _spark->setOpacity(127);
    _spark->setTotalParticles(5000);
    _spark->setEmissionRate(200.f);
    _spark->setGravity(Vec2(0.f, -1000.f));
    _spark->pause();
    addChild(_spark);

    if (LOOP_SOUND == -1)
    {
      LOOP_SOUND = _audioHelper->playEffect("sounds/laser.mp3", true, 0.7f);
    }
  }
  _spark->setPosition(point);
}
