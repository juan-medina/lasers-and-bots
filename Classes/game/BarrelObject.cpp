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

#include "BarrelObject.h"

BarrelObject* BarrelObject::create(PhysicsShapeCache* physicsShapeCache, const int barrelNum,
                                   const std::string& image, const std::string& shape)
{
  BarrelObject* ret = nullptr;

  do
  {
    auto object = new BarrelObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physicsShapeCache, barrelNum, image, shape))
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

bool BarrelObject::init(PhysicsShapeCache* physicsShapeCache, const int barrelNum, const std::string& image,
                        const std::string& shape)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init(physicsShapeCache, shape, image, "barrel"));

    setAnchorPoint(Vec2(0.5f, 0.5f));

    // making look like random but is not, base on how may barrels we added

    const auto gap = barrelNum * 10.f;
    auto stepTime = 0.f;
    auto step = 0.f;
    auto amount = 0.f;

    amount = 50.f;
    step = amount + gap;
    stepTime = step / amount;
    const auto moveDownStep1 = MoveBy::create(stepTime, Vec3(0.0f, step, 0.0f));
    UTILS_BREAK_IF(moveDownStep1 == nullptr);

    amount = 40.f;
    step = -(amount + gap);
    stepTime = step / -amount;
    const auto moveUpStep1 = MoveBy::create(stepTime, Vec3(0.0f, step, 0.0f));
    UTILS_BREAK_IF(moveUpStep1 == nullptr);

    amount = 30.f;
    step = amount + gap;
    stepTime = step / amount;
    const auto moveDownStep2 = MoveBy::create(stepTime, Vec3(0.0f, step, 0.0f));
    UTILS_BREAK_IF(moveDownStep2 == nullptr);

    amount = 40.f;
    step = -(amount + gap);
    stepTime = step / -amount;
    const auto moveUpStep2 = MoveBy::create(stepTime, Vec3(0.0f, step, 0.0f));
    UTILS_BREAK_IF(moveUpStep2 == nullptr);

    const auto movement = Sequence::create(moveDownStep1, moveUpStep1, moveDownStep2, moveUpStep2, nullptr);
    UTILS_BREAK_IF(movement == nullptr);

    const auto repeatMovement = RepeatForever::create(movement);
    UTILS_BREAK_IF(repeatMovement == nullptr);

    runAction(repeatMovement);

    ret = true;
  } while (false);

  return ret;
}
