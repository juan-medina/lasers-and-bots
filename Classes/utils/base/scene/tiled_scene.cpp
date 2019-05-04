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

#include "tiled_scene.h"

tiled_scene::tiled_scene() :
  tiled_map_(nullptr)
{
}

tiled_scene* tiled_scene::create(basic_app* application, const std::string& tmx_file)
{
  auto scene = new tiled_scene();

  if (scene)
  {
    if (scene->init(application, tmx_file))
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

Scene* tiled_scene::scene(basic_app* application, const std::string& tmx_file)
{
  auto scene = new tiled_scene();

  if (scene)
  {
    if (scene->init(application, tmx_file))
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

bool tiled_scene::init(basic_app* application, const std::string& tmx_file)
{
  auto ret = false;

  do
  {
    const auto map = experimental::TMXTiledMap::create(tmx_file);
    ret = (map != nullptr);
    UTILS_BREAK_IF(!ret);

    tiled_map_ = map;

    const auto tile_size = map->getTileSize();
    const auto size = map->getMapSize();

    ret = base_class::init(application, size, tile_size);
    UTILS_BREAK_IF(!ret);

    addChild(map);
  }
  while (false);

  return ret;
}
