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

#include "grid_scene.h"

grid_scene* grid_scene::create(const Size& blocks, const Size& block_size)
{
  // create the grid
  auto scene = new grid_scene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init(blocks, block_size))
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }
  }

  // return the scene
  return scene;
}

Scene* grid_scene::scene(const Size& blocks, const Size& block_size)
{
  // create the grid
  auto scene = new grid_scene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init(blocks, block_size))
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }
  }

  // return the scene
  return scene;
}

// on "init" you need to initialize your instance
bool grid_scene::init(const Size& blocks, const Size& block_size)
{
  auto ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first

    ret = base_class::init();

    UTILS_BREAK_IF(!ret);

    // init vars
    blocks_ = blocks;
    block_size_ = block_size;

    // set the anchor
    setAnchorPoint(Point::ZERO);

    // calculate total size
    total_size_ = Size(block_size_.width * blocks_.width, block_size_.height * blocks_.height);

    ret = true;
  }
  while (false);

  return ret;
}

void grid_scene::onEnter()
{
  // call base_class
  base_class::onEnter();
}

void grid_scene::onExit()
{
  // call base_class
  base_class::onExit();

  // we don't need anything else
  removeAllChildrenWithCleanup(true);
}

bool grid_scene::create_debug_grid(const std::string& font_name)
{
  auto ret = false;

  do
  {
    // create a draw node
    auto draw = DrawNode::create();
    UTILS_BREAK_IF(draw == nullptr);

    // everything ok, so far
    auto all_ok = true;

    // for each row and col and if we still ok
    for (auto row = 0; ((row < blocks_.height) && all_ok); row++)
    {
      for (auto col = 0; ((col < blocks_.width) && all_ok); col++)
      {
        // get the block position and calculate quad
        auto position = get_block_position(col, row);

        auto from = position.origin;
        const auto to = from + position.size;

        Point vertex[4];
        vertex[0] = from;
        vertex[1] = Vec2(from.x, to.y);
        vertex[2] = to;
        vertex[3] = Vec2(to.x, from.y);

        // draw the quad
        draw->drawPolygon(&vertex[0], 4, Color4F(0.0f, 0.0f, 0.0f, 0.25f), 2.0f, Color4F(1.0f, 1.0f, 1.0f, 0.25f));

        // create the label for this quad
        auto label = Label::createWithTTF(string_format("%d-%d", col, row), font_name, block_size_.width / 4);
        all_ok = (label != nullptr);

        // if all its ok set position and add the label
        if (all_ok)
        {
          label->setPosition(get_block_center(col, row));
          addChild(label);
        }
      }
    }

    UTILS_BREAK_IF(!all_ok);

    // add the debug grid
    addChild(draw);

    ret = true;
  }
  while (false);

  return ret;
}
