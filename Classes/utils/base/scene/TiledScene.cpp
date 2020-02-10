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

#include "TiledScene.h"

TiledScene::TiledScene() : _tiledMap(nullptr) {}

TiledScene* TiledScene::create(BasicApp* application, const std::string& tmxFile)
{
  auto scene = new TiledScene();

  if (scene != nullptr)
  {
    if (scene->init(application, tmxFile))
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

Scene* TiledScene::scene(BasicApp* application, const std::string& tmxFile)
{
  auto scene = new TiledScene();

  if (scene != nullptr)
  {
    if (scene->init(application, tmxFile))
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

bool TiledScene::init(BasicApp* application, const std::string& tmxFile)
{
  auto ret = false;

  do
  {
    const auto map = experimental::TMXTiledMap::create(tmxFile);
    ret = (map != nullptr);
    UTILS_BREAK_IF(!ret);

    _tiledMap = map;

    const auto tileSize = map->getTileSize();
    const auto size = map->getMapSize();

    UTILS_BREAK_IF(!BaseClass::init(application, size, tileSize));

    ret = true;

    addChild(map);
  } while (false);

  return ret;
}

Vec2 TiledScene::getObjectCenterPosition(const ValueMap& values)
{
  const auto width = values.at("width").asFloat();
  const auto height = values.at("height").asFloat();
  const auto x = values.at("x").asFloat() + width;
  const auto y = values.at("y").asFloat() + height;
  const auto rotation = values.at("rotation").asFloat();

  const auto angle = CC_DEGREES_TO_RADIANS(-rotation);

  const auto centerX = width / 2;
  const auto centerY = height / 2;

  const auto cosRotation = cosf(angle);
  const auto sinRotation = sinf(angle);

  const auto rotatedCenterX = centerX * cosRotation - centerY * sinRotation;
  const auto rotatedCenterY = centerX * sinRotation + centerY * cosRotation;

  return {x + rotatedCenterX - width, y + rotatedCenterY};
}

Vec2 TiledScene::getObjectPosition(const ValueMap& values)
{
  const auto width = values.at("width").asFloat();
  const auto height = values.at("height").asFloat();
  const auto x = values.at("x").asFloat() + width / 2;
  const auto y = values.at("y").asFloat() + height / 2;

  return {x, y};
}
