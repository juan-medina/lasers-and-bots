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

#include "SawObject.h"

SawObject::SawObject() {}

SawObject* SawObject::create(PhysicsShapeCache* physicsShapeCache, const std::string& image,
                             const std::string& shape, const int damage, const float rotationTime,
                             const float movement, const float movementTime, const float stopTime)
{
  SawObject* ret = nullptr;

  do
  {
    auto object = new SawObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physicsShapeCache, image, shape, damage, rotationTime, movement, movementTime, stopTime))
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

bool SawObject::init(PhysicsShapeCache* physicsShapeCache, const std::string& image, const std::string& shape,
                     const int damage, const float rotationTime, const float movement,
                     const float movementTime, const float stopTime)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init(physicsShapeCache, shape, image, "saw", damage));

    setAnchorPoint(Vec2(0.5f, 0.5f));

    const auto moveRight = MoveBy::create(movementTime, Vec2(movement, 0.f));
    UTILS_BREAK_IF(moveRight == nullptr);

    const auto moveLeft = MoveBy::create(movementTime, Vec2(-movement, 0.f));
    UTILS_BREAK_IF(moveLeft == nullptr);

    const auto delay = DelayTime::create(stopTime);
    UTILS_BREAK_IF(delay == nullptr);

    const auto movementSequence = Sequence::create(moveRight, delay, moveLeft, delay, nullptr);
    UTILS_BREAK_IF(movementSequence == nullptr);

    const auto repeatMovement = RepeatForever::create(movementSequence);
    UTILS_BREAK_IF(repeatMovement == nullptr);

    const auto rotation = RotateBy::create(rotationTime, 360.f);
    UTILS_BREAK_IF(rotation == nullptr);

    const auto repeatRotation = RepeatForever::create(rotation);
    UTILS_BREAK_IF(repeatRotation == nullptr);

    runAction(repeatMovement);
    runAction(repeatRotation);

    ret = true;
  } while (false);

  return ret;
}
