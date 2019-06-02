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

#include "box_object.h"

box_object* box_object::create(physics_shape_cache* physics_shape_cache, const std::string& image,
                               const std::string& shape)
{
  box_object* ret = nullptr;

  do
  {
    auto object = new box_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physics_shape_cache, image, shape))
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

bool box_object::init(physics_shape_cache* physics_shape_cache, const std::string& image,
                      const std::string& shape)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(physics_shape_cache, shape, image, "box"));

    setAnchorPoint(Vec2(0.5f, 0.5f));

    ret = true;
  } while (false);

  return ret;
}
