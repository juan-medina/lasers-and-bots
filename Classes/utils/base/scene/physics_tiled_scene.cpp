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

#include "physics_tiled_scene.h"
#include "rapidjson/document.h"

physics_tiled_scene* physics_tiled_scene::create(const std::string& tmx_file, const float gravity,
                                                 const bool debug_physics)
{
  // create the game
  auto scene = new physics_tiled_scene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init(tmx_file, gravity, debug_physics))
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

Scene* physics_tiled_scene::scene(const std::string& tmx_file, const float gravity, const bool debug_physics)
{
  // create the grid
  auto scene = new physics_tiled_scene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init(tmx_file, gravity, debug_physics))
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
bool physics_tiled_scene::init(const std::string& tmx_file, const float gravity, const bool debug_physics)
{
  auto ret = false;

  do
  {
    // init with a grid
    ret = base_class::init(tmx_file);
    UTILS_BREAK_IF(!ret);

    gravity_ = gravity;

    init_physics(debug_physics);

    // add physics to map
    UTILS_BREAK_IF(!add_physics_to_map());
  }
  while (false);

  return ret;
}

void physics_tiled_scene::init_physics(const bool debug_physics) const
{
  const auto edge = PhysicsBody::createEdgeBox(this->total_size_, PhysicsMaterial(0.1f, 0.0f, 0.5f), 5);
  this->get_tiled_map()->addComponent(edge);

  if (debug_physics)
  {
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
  }

  getPhysicsWorld()->setGravity(Vec2(0.0f, gravity_));
  getPhysicsWorld()->setSubsteps(4);
}

string physics_tiled_scene::get_shape_from_tile_gid(int gid)
{
  if (gid_to_shapes_.count(gid) == 0)
  {
    string shape;
    const auto map = get_tiled_map();
    if (map->getPropertiesForGID(gid).getType() == Value::Type::MAP)
    {
      const auto properties = map->getPropertiesForGID(gid).asValueMap();
      if (properties.count("shape") == 1)
      {
        shape = properties.at("shape").asString();
      }
    }
    gid_to_shapes_[gid] = shape;
    return shape;
  }

  return gid_to_shapes_[gid];
}

PhysicsBody* physics_tiled_scene::get_body_from_shape(const string& shape, const PhysicsMaterial& material,
                                                      const float scale_x, const float scale_y)
{
  const auto body = PhysicsBody::create();
  body->setPositionOffset(Vec2(-scale_x / 2, -scale_y / 2));

  auto document = rapidjson::Document();

  const auto full_path = FileUtils::getInstance()->fullPathForFilename("shapes/shapes.json");
  const auto json = FileUtils::getInstance()->getStringFromFile(full_path);

  document.Parse(json.c_str());
  if (!document.HasParseError())
  {
    if (document.HasMember("rigidBodies"))
    {
      if (document["rigidBodies"].IsArray())
      {
        const auto bodies = document["rigidBodies"].GetArray();
        for (rapidjson::Value::ConstValueIterator bodies_iterator = bodies.Begin(); bodies_iterator != bodies.End();
             ++bodies_iterator)
        {
          if (bodies_iterator->HasMember("name"))
          {
            const auto name = (*bodies_iterator)["name"].GetString();
            if (name == shape)
            {
              if (bodies_iterator->HasMember("polygons"))
              {
                if ((*bodies_iterator)["polygons"].IsArray())
                {
                  const auto polygons = (*bodies_iterator)["polygons"].GetArray();
                  for (rapidjson::Value::ConstValueIterator polygon_iterator = polygons.Begin(); polygon_iterator !=
                       polygons.End(); ++polygon_iterator)
                  {
                    if ((*polygon_iterator).IsArray())
                    {
                      const auto vertex_def = (*polygon_iterator).GetArray();
                      const auto size = vertex_def.Size();
                      const auto vertex = new Vec2[size];
                      for (rapidjson::SizeType i = 0; i < size; i++)
                      {
                        vertex[i].x = 0.f;
                        vertex[i].y = 0.f;
                        if (vertex_def[i].HasMember("x"))
                        {
                          if (vertex_def[i]["x"].IsFloat())
                          {
                            vertex[i].x = vertex_def[i]["x"].GetFloat() * scale_x;
                          }
                        }
                        if (vertex_def[i].HasMember("y"))
                        {
                          if (vertex_def[i]["y"].IsFloat())
                          {
                            vertex[i].y = vertex_def[i]["y"].GetFloat() * scale_y;
                          }
                        }
                      }
                      const auto polygon_shape = PhysicsShapePolygon::create(vertex, size, material);

                      body->addShape(polygon_shape);
                      delete[] vertex;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return body;
}

bool physics_tiled_scene::add_body_to_sprite(Sprite* sprite, const string& shape)
{
  auto result = false;

  do
  {
    const auto mat = PhysicsMaterial(0.0f, 0.0f, 0.0f);
    PhysicsBody* body = nullptr;
    if (shape.empty())
    {
      body = PhysicsBody::createBox(sprite->getContentSize(), mat);
    }
    else
    {
      const auto size = sprite->getContentSize();
      body = get_body_from_shape(shape, mat, size.width, size.height);
    }

    UTILS_BREAK_IF(body == nullptr);

    body->setDynamic(false);
    sprite->setPhysicsBody(body);

    result = true;
  }
  while (false);

  return result;
}

bool physics_tiled_scene::add_physics_to_map()
{
  auto result = false;

  do
  {
    const auto map = get_tiled_map();

    for (auto& child : map->getChildren())
    {
      const auto layer = dynamic_cast<experimental::TMXLayer*>(child);
      if (layer!=nullptr)
      {
        auto physics = layer->getProperty("physics");
        if ((physics.getType() == Value::Type::STRING) && (physics.asBool()))
        {
          for (auto col = 0; col < this->blocks_.height; col++)
          {
            for (auto row = 0; row < this->blocks_.width; row++)
            {
              const auto tile_pos = Vec2(row, col);
              const auto sprite = layer->getTileAt(tile_pos);

              if (sprite != nullptr)
              {
                const auto gid = layer->getTileGIDAt(tile_pos);
                const auto shape = get_shape_from_tile_gid(gid);

                add_body_to_sprite(sprite, shape);
              }
            }
          }
        }
      }
    }


    result = true;
  }
  while (false);

  return result;
}
