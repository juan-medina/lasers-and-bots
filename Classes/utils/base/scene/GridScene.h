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

#ifndef __GRIDSCENE_H__
#define __GRIDSCENE_H__

#include "BasicScene.h"

class GridScene : public BasicScene
{
public:
  using BaseClass = BasicScene;

  static GridScene* create(BasicApp* application, const Size& blocks, const Size& blockSize);

  static Scene* scene(BasicApp* application, const Size& blocks, const Size& blockSize);

  bool init(BasicApp* application, const Size& blocks, const Size& blockSize);

  inline Rect getBlockPosition(const int col, const int row) const
  {
    return Rect(col * _blockSize.width, row * _blockSize.height, _blockSize.width, _blockSize.height);
  }

  inline Vec2 getBlockCenter(const int col, const int row) const
  {
    const auto position = this->getBlockPosition(col, row);

    return {position.getMidX(), position.getMidY()};
  }

protected:
  bool createDebugGrid(const std::string& fontName);

  Size _blocks;
  Size _blockSize;
  Size _totalSize;
};

#endif //__GRIDSCENE_H__
