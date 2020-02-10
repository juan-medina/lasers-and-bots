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

#include "GridScene.h"

GridScene* GridScene::create(BasicApp* application, const Size& blocks, const Size& blockSize)
{
  auto scene = new GridScene();

  if (scene)
  {
    if (scene->init(application, blocks, blockSize))
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

Scene* GridScene::scene(BasicApp* application, const Size& blocks, const Size& blockSize)
{
  auto scene = new GridScene();

  if (scene)
  {
    if (scene->init(application, blocks, blockSize))
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

bool GridScene::init(BasicApp* application, const Size& blocks, const Size& blockSize)
{
  auto ret = false;

  do
  {
    ret = BaseClass::init(application);

    UTILS_BREAK_IF(!ret);

    _blocks = blocks;
    _blockSize = blockSize;

    setAnchorPoint(Point::ZERO);

    _totalSize = Size(_blockSize.width * _blocks.width, _blockSize.height * _blocks.height);

    ret = true;
  } while (false);

  return ret;
}

bool GridScene::createDebugGrid(const std::string& fontName)
{
  auto ret = false;

  do
  {
    auto draw = DrawNode::create();
    UTILS_BREAK_IF(draw == nullptr);

    auto allOk = true;

    for (auto row = 0; ((row < _blocks.height) && allOk); row++)
    {
      for (auto col = 0; ((col < _blocks.width) && allOk); col++)
      {
        auto position = getBlockPosition(col, row);

        auto from = position.origin;
        const auto to = from + position.size;

        draw->drawRect(from, to, Color4F(1.0f, 1.0f, 1.0f, 0.25f));

        auto label = Label::createWithTTF(StringFormat("%d-%d", col, row), fontName, _blockSize.width / 4);
        allOk = (label != nullptr);

        if (allOk)
        {
          label->setPosition(getBlockCenter(col, row));
          addChild(label);
        }
      }
    }

    UTILS_BREAK_IF(!allOk);

    addChild(draw);

    ret = true;
  } while (false);

  return ret;
}
