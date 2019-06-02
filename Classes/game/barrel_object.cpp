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

#include "barrel_object.h"

barrel_object* barrel_object::create(physics_shape_cache* physics_shape_cache, const int barrel_num,
                                     const std::string& image, const std::string& shape)
{
  barrel_object* ret = nullptr;

  do
  {
    auto object = new barrel_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physics_shape_cache, barrel_num, image, shape))
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

bool barrel_object::init(physics_shape_cache* physics_shape_cache, const int barrel_num,
                         const std::string& image, const std::string& shape)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(physics_shape_cache, shape, image, "barrel"));

    setAnchorPoint(Vec2(0.5f, 0.5f));

    // making look like random but is not, base on how may barrels we added

    const auto gap = barrel_num * 10.f;
    auto step_time = 0.f;
    auto step = 0.f;
    auto amount = 0.f;

    amount = 50.f;
    step = amount + gap;
    step_time = step / amount;
    const auto move_down_step_1 = MoveBy::create(step_time, Vec3(0.0f, step, 0.0f));
    UTILS_BREAK_IF(move_down_step_1 == nullptr);

    amount = 40.f;
    step = -(amount + gap);
    step_time = step / -amount;
    const auto move_up_step_1 = MoveBy::create(step_time, Vec3(0.0f, step, 0.0f));
    UTILS_BREAK_IF(move_up_step_1 == nullptr);

    amount = 30.f;
    step = amount + gap;
    step_time = step / amount;
    const auto move_down_step_2 = MoveBy::create(step_time, Vec3(0.0f, step, 0.0f));
    UTILS_BREAK_IF(move_down_step_2 == nullptr);

    amount = 40.f;
    step = -(amount + gap);
    step_time = step / -amount;
    const auto move_up_step_2 = MoveBy::create(step_time, Vec3(0.0f, step, 0.0f));
    UTILS_BREAK_IF(move_up_step_2 == nullptr);

    const auto movement =
      Sequence::create(move_down_step_1, move_up_step_1, move_down_step_2, move_up_step_2, nullptr);
    UTILS_BREAK_IF(movement == nullptr);

    const auto repeat_movement = RepeatForever::create(movement);
    UTILS_BREAK_IF(repeat_movement == nullptr);

    runAction(repeat_movement);

    ret = true;
  } while (false);

  return ret;
}
