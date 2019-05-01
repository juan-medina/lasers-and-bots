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

#include "saw_object.h"

saw_object::saw_object()
{
}

saw_object* saw_object::create(const std::string& image, const std::string& shape, const int damage,
                               const float rotation_time, const float movement, const float movement_time,
                               const float stop_time)
{
  saw_object* ret = nullptr;

  do
  {
    auto object = new saw_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(image, shape, damage, rotation_time, movement, movement_time, stop_time))
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

  return ret;
}

bool saw_object::init(const std::string& image, const std::string& shape, const int damage,
                      const float rotation_time, const float movement, const float movement_time,
                      const float stop_time)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(image, "saw", damage));

    UTILS_BREAK_IF(!set_shape(shape));

    setAnchorPoint(Vec2(0.5f, 0.5f));

    const auto move_right = MoveBy::create(movement_time, Vec2(movement, 0.f));
    UTILS_BREAK_IF(move_right == nullptr);

    const auto move_left = MoveBy::create(movement_time, Vec2(-movement, 0.f));
    UTILS_BREAK_IF(move_left == nullptr);

    const auto delay = DelayTime::create(stop_time);
    UTILS_BREAK_IF(delay == nullptr);

    const auto movement_sequence = Sequence::create(move_right, delay, move_left, delay, nullptr);
    UTILS_BREAK_IF(movement_sequence == nullptr);

    const auto repeat_movement = RepeatForever::create(movement_sequence);
    UTILS_BREAK_IF(repeat_movement == nullptr);

    const auto rotation = RotateBy::create(rotation_time, 360.f);
    UTILS_BREAK_IF(rotation == nullptr);

    const auto repeat_rotation = RepeatForever::create(rotation);
    UTILS_BREAK_IF(repeat_rotation == nullptr);

    runAction(repeat_movement);
    runAction(repeat_rotation);

    ret = true;
  }
  while (false);

  return ret;
}
