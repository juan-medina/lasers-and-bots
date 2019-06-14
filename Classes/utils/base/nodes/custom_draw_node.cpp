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

#include "custom_draw_node.h"

custom_draw_node *custom_draw_node::create()
{
  custom_draw_node *node = nullptr;

  do
  {
    node = new custom_draw_node();
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

bool custom_draw_node::init()
{
  auto ret = false;

  do
  {
    ret = base_class::init();

    UTILS_BREAK_IF(!ret);

    ret = true;
  } while (false);

  return ret;
}

void custom_draw_node::draw_color_quad(const Vec2 *vertex, const Color4F *fill_colors)
{
  auto vertex_count = 6;
  ensureCapacity(vertex_count);

  V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);

  static Vec2 vec2_zero(0.0f, 0.0f);

  V2F_C4B_T2F_Triangle triangle_1 = {
    {vertex[0], Color4B(fill_colors[0]), *(Tex2F *)&vec2_zero},
    {vertex[1], Color4B(fill_colors[1]), *(Tex2F *)&vec2_zero},
    {vertex[3], Color4B(fill_colors[3]), *(Tex2F *)&vec2_zero},
  };

  *triangles++ = triangle_1;

  V2F_C4B_T2F_Triangle triangle_2 = {
    {vertex[1], Color4B(fill_colors[1]), *(Tex2F *)&vec2_zero},
    {vertex[2], Color4B(fill_colors[2]), *(Tex2F *)&vec2_zero},
    {vertex[3], Color4B(fill_colors[3]), *(Tex2F *)&vec2_zero},
  };

  *triangles = triangle_2;

  _bufferCount += vertex_count;

  _dirty = true;
}