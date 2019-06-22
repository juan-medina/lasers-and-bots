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

#include "CustomDrawNode.h"

CustomDrawNode *CustomDrawNode::create()
{
  CustomDrawNode *node = nullptr;

  do
  {
    node = new CustomDrawNode();
    UTILS_BREAK_IF(node == nullptr);

    if (node->init())
    {
      node->autorelease();
    }
    else
    {
      delete node;
      node = nullptr;
    }

  } while (false);

  return node;
}

bool CustomDrawNode::init()
{
  auto ret = false;

  do
  {
    ret = BaseClass::init();

    UTILS_BREAK_IF(!ret);

    ret = true;
  } while (false);

  return ret;
}

void CustomDrawNode::drawColorQuad(const Vec2 *vertex, const Color4F *fillColors)
{
  static const auto VERTEX_COUNT = 6;
  ensureCapacity(VERTEX_COUNT);

  auto triangles = reinterpret_cast<V2F_C4B_T2F_Triangle *>(_buffer + _bufferCount);

  static const Tex2F TEXT_2_F_ZERO(0.0f, 0.0f);

  const V2F_C4B_T2F_Triangle triangle1 = {
    {vertex[0], Color4B(fillColors[0]), TEXT_2_F_ZERO},
    {vertex[1], Color4B(fillColors[1]), TEXT_2_F_ZERO},
    {vertex[3], Color4B(fillColors[3]), TEXT_2_F_ZERO},
  };

  *triangles++ = triangle1;

  const V2F_C4B_T2F_Triangle triangle2 = {
    {vertex[1], Color4B(fillColors[1]), TEXT_2_F_ZERO},
    {vertex[2], Color4B(fillColors[2]), TEXT_2_F_ZERO},
    {vertex[3], Color4B(fillColors[3]), TEXT_2_F_ZERO},
  };

  *triangles = triangle2;

  _bufferCount += VERTEX_COUNT;

  _dirty = true;
}
