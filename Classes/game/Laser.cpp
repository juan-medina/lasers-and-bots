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

#include "Laser.h"

Laser::Laser() :
  _angle(0.0f),
  _draw(nullptr),
  _physicsWorld(nullptr)
{
}

Laser::~Laser()
{
}

Laser* Laser::create()
{
  Laser* ret = nullptr;

  do
  {
    auto object = new Laser();
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
  } while (0);

  // return the object
  return ret;
}

// on "init" you need to initialize your instance
bool Laser::init()
{
  bool ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first
    UTILS_BREAK_IF(!parent::init());

    _draw = DrawNode::create();
    UTILS_BREAK_IF(_draw == nullptr);

    _draw->setOpacity(180);

    addChild(_draw);

    scheduleUpdate();

    ret = true;

  } while (0);

  return ret;
}

void Laser::update(float delta)
{
  // get the physicsWorld
  if (_physicsWorld == nullptr)
  {
    _physicsWorld = Director::getInstance()->getRunningScene()->getPhysicsWorld();
  }

  // clear our laser
  _draw->clear();

  // we star where the laser is
  const auto originPoint = Vec2::ZERO;
  auto originInPointInWorld = convertToWorldSpace(originPoint);

  // calculate distance base on the angle
  auto distance = Vec2(_maxLaserLength * cosf(_angle), _maxLaserLength * sinf(_angle));

  // where the laser will reach
  auto destinationPointInWorld = originInPointInWorld + distance;

  // final point of the laser
  auto finalPointInWorld = destinationPointInWorld;

  // lambda that check all bodys that the laser touch and save the one closer to the origin
  PhysicsRayCastCallbackFunc intersectCloserBodyFunc = [originInPointInWorld, &finalPointInWorld]
  (PhysicsWorld& /*world*/, const PhysicsRayCastInfo& info, void* /*data*/)->bool
  {
    auto origin = Point(originInPointInWorld);
    auto newPoint = Point(info.contact);
    auto savedPoint = Point(finalPointInWorld);
    if (origin.distance(newPoint) < origin.distance(savedPoint))
    {
      finalPointInWorld = info.contact;
    }
    // we need to check all bodies that touch so return true to continue
    return true;
  };

  // do the ray cast
  _physicsWorld->rayCast(intersectCloserBodyFunc, originInPointInWorld, destinationPointInWorld, nullptr);
  
  // conver the point into node space and draw it
  auto finalPoint = convertToNodeSpace(finalPointInWorld);
  _draw->drawSegment(originPoint, finalPoint, 2, Color4F::RED);

  // if we have actually hit something draw a dot and create an emiter
  if ((finalPointInWorld.x != destinationPointInWorld.x) & (finalPointInWorld.y != destinationPointInWorld.y))
  {
    _draw->drawDot(finalPoint, 10, Color4F::RED);
    createEmitter(finalPoint);
  }

  // rotate the laser angle
  _angle += 0.25f * (float)M_PI / 180.0f;

}

void Laser::createEmitter(const Vec2 point)
{
  auto emitter = ParticleFire::create();
  emitter->setDuration(1);
  emitter->setBlendFunc(BlendFunc::ADDITIVE);
  emitter->setColor(Color3B::RED);
  emitter->setOpacityModifyRGB(true);
  emitter->setOpacity(127);
  emitter->setPosition(point);
  emitter->setEmissionRate(10);
  emitter->setGravity(_physicsWorld->getGravity());
  emitter->setAutoRemoveOnFinish(true);
  addChild(emitter);
}
