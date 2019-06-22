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

#ifndef __TILED_SCENE_H__
#define __TILED_SCENE_H__

#include "GridScene.h"

class TiledScene : public GridScene
{
public:
  using BaseClass = GridScene;

  TiledScene();

  static TiledScene* create(BasicApp*, const std::string& tmxFile);

  static Scene* scene(BasicApp*, const std::string& tmxFile);

  bool init(BasicApp*, const std::string& tmxFile);

  virtual experimental::TMXTiledMap* getTiledMap() const noexcept { return _tiledMap; };

protected:
  static Vec2 getObjectCenterPosition(const ValueMap& values);
  static Vec2 getObjectPosition(const ValueMap& values);

private:
  experimental::TMXTiledMap* _tiledMap;
};

#endif // __TILED_SCENE_H__
