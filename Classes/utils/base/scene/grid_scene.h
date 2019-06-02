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

#ifndef __GRID_SCENE_H__
#define __GRID_SCENE_H__

#include "basic_scene.h"

class grid_scene : public basic_scene
{
public:
  using base_class = basic_scene;

  static grid_scene* create(basic_app* application, const Size& blocks, const Size& block_size);

  static Scene* scene(basic_app* application, const Size& blocks, const Size& block_size);

  bool init(basic_app* application, const Size& blocks, const Size& block_size);

  inline Rect get_block_position(const int col, const int row) const
  {
    return Rect(col * block_size_.width, row * block_size_.height, block_size_.width, block_size_.height);
  }

  inline Vec2 get_block_center(const int col, const int row) const
  {
    auto position = this->get_block_position(col, row);

    return Vec2(position.getMidX(), position.getMidY());
  }

protected:
  bool create_debug_grid(const std::string& font_name);

  Size blocks_;
  Size block_size_;
  Size total_size_;
};

#endif // __GRID_SCENE_H__
