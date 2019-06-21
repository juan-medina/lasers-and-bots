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

#include "grid_scene.h"

grid_scene* grid_scene::create(basic_app* application, const Size& blocks, const Size& block_size)
{
  auto scene = new grid_scene();

  if (scene)
  {
    if (scene->init(application, blocks, block_size))
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }
  }

  return scene;
}

Scene* grid_scene::scene(basic_app* application, const Size& blocks, const Size& block_size)
{
  auto scene = new grid_scene();

  if (scene)
  {
    if (scene->init(application, blocks, block_size))
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }
  }

  return scene;
}

bool grid_scene::init(basic_app* application, const Size& blocks, const Size& block_size)
{
  auto ret = false;

  do
  {
    ret = base_class::init(application);

    UTILS_BREAK_IF(!ret);

    blocks_ = blocks;
    block_size_ = block_size;

    setAnchorPoint(Point::ZERO);

    total_size_ = Size(block_size_.width * blocks_.width, block_size_.height * blocks_.height);

    ret = true;
  } while (false);

  return ret;
}

bool grid_scene::create_debug_grid(const std::string& font_name)
{
  auto ret = false;

  do
  {
    auto draw = DrawNode::create();
    UTILS_BREAK_IF(draw == nullptr);

    auto all_ok = true;

    for (auto row = 0; ((row < blocks_.height) && all_ok); row++)
    {
      for (auto col = 0; ((col < blocks_.width) && all_ok); col++)
      {
        auto position = get_block_position(col, row);

        auto from = position.origin;
        const auto to = from + position.size;

        draw->drawRect(from, to, Color4F(1.0f, 1.0f, 1.0f, 0.25f));

        auto label = Label::createWithTTF(StringFormat("%d-%d", col, row), font_name, block_size_.width / 4);
        all_ok = (label != nullptr);

        if (all_ok)
        {
          label->setPosition(get_block_center(col, row));
          addChild(label);
        }
      }
    }

    UTILS_BREAK_IF(!all_ok);

    addChild(draw);

    ret = true;
  } while (false);

  return ret;
}
