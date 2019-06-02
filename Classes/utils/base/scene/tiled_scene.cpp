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

#include "tiled_scene.h"

tiled_scene::tiled_scene() : tiled_map_(nullptr) {}

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

    UTILS_BREAK_IF(!base_class::init(application, size, tile_size));

    UTILS_BREAK_IF(!handle_transparent_layers());

    ret = true;

    addChild(map);
  } while (false);

  return ret;
}

bool tiled_scene::handle_transparent_layers() const
{
  auto result = false;

  do
  {
    const auto map = get_tiled_map();

    for (auto& child : map->getChildren())
    {
      const auto layer = dynamic_cast<experimental::TMXLayer*>(child);
      if (layer != nullptr)
      {
        const auto opacity = layer->getOpacity();
        if (opacity != 255)
        {
          for (auto col = 0; col < blocks_.height; col++)
          {
            for (auto row = 0; row < blocks_.width; row++)
            {
              const auto tile_pos = Vec2(row, col);
              const auto gid = layer->getTileGIDAt(tile_pos);
              if (gid != 0)
              {
                layer->getTileAt(tile_pos)->setOpacity(opacity);
              }
            }
          }
        }
      }
    }

    result = true;
  } while (false);

  return result;
}

Vec2 tiled_scene::get_object_center_position(const ValueMap& values)
{
  const auto width = values.at("width").asFloat();
  const auto height = values.at("height").asFloat();
  const auto x = values.at("x").asFloat() + width;
  const auto y = values.at("y").asFloat() + height;
  const auto rotation = values.at("rotation").asFloat();

  const auto angle = CC_DEGREES_TO_RADIANS(-rotation);

  const auto center_x = width / 2;
  const auto center_y = height / 2;

  const auto cos_rotation = cosf(angle);
  const auto sin_rotation = sinf(angle);

  const auto rotated_center_x = center_x * cos_rotation - center_y * sin_rotation;
  const auto rotated_center_y = center_x * sin_rotation + center_y * cos_rotation;

  return {x + rotated_center_x - width, y + rotated_center_y};
}

Vec2 tiled_scene::get_object_position(const ValueMap& values)
{
  const auto width = values.at("width").asFloat();
  const auto height = values.at("height").asFloat();
  const auto x = values.at("x").asFloat() + width / 2;
  const auto y = values.at("y").asFloat() + height / 2;

  return {x, y};
}
