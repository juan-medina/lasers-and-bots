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

#ifndef __GRID_SCENE_H__
#define __GRID_SCENE_H__

#include "basic_scene.h"

class grid_scene : public basic_scene
{
public:

  // base_class
  using base_class = basic_scene;

  // create with size
  static grid_scene* create(basic_app* application, const Size& blocks, const Size& block_size);

  // create the scene
  static Scene* scene(basic_app* application, const Size& blocks, const Size& block_size);

  // init this class
  bool init(basic_app* application, const Size& blocks, const Size& block_size);

  // get the position of a giving block in a col/row
  Rect get_block_position(const int col, const int row) const
  {
    return Rect(col * block_size_.width, row * block_size_.height, block_size_.width, block_size_.height);
  }

  // get the block center for a giving block in a col/row
  Vec2 get_block_center(const int col, const int row) const
  {
    auto position = this->get_block_position(col, row);

    return Vec2(position.getMidX(), position.getMidY());
  }

  // get a block in a giving location
  Vec2 get_block_at_location(const Point location) const
  {
    // calculate row and col
    const auto col = static_cast<int>(location.x / block_size_.width);
    const auto row = static_cast<int>(location.y / block_size_.height);

    return Vec2(col, row);
  }


  // enter scene
  void onEnter() override;

  // exit scene
  void onExit() override;


  Size get_blocks() const noexcept
  {
    return blocks_;
  };

  Size get_block_size() const noexcept
  {
    return block_size_;
  };

  Size get_total_size() const noexcept
  {
    return total_size_;
  };

protected:

  // create a debug grid with a label text in the center
  bool create_debug_grid(const std::string& font_name);

  // number of blocks
  Size blocks_;

  // size of a block
  Size block_size_;

  // total size of the area
  Size total_size_;
};

#endif // __GRID_SCENE_H__
