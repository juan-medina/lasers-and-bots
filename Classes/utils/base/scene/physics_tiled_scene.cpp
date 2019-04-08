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
#include "../../physics/physics_body_cache.h"

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
  const auto edge = PhysicsBody::createEdgeBox(total_size_, PhysicsMaterial(0.1f, 0.0f, 0.5f), 5);
  get_tiled_map()->addComponent(edge);

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

bool physics_tiled_scene::add_body_to_node(Node* node, const string& shape)
{
  auto result = false;

  do
  {
    const auto mat = PhysicsMaterial(0.0f, 0.0f, 0.0f);
    PhysicsBody* body = nullptr;
    if (shape.empty())
    {
      body = PhysicsBody::createBox(node->getContentSize(), mat);
    }
    else
    {
      const auto cache = physics_body_cache::get_instance();
      body = cache->get_body(shape, mat);
    }

    UTILS_BREAK_IF(body == nullptr);

    body->setDynamic(false);
    node->setPhysicsBody(body);

    result = true;
  }
  while (false);

  return result;
}

Node* physics_tiled_scene::create_dummy_node(experimental::TMXLayer* const layer, const Vec2& tile_pos) const
{
  const auto node = Node::create();
  node->setContentSize(block_size_);
  node->setPosition(layer->getPositionAt(tile_pos));
  node->setVisible(false);
  layer->addChild(node);
  return node;
}

bool physics_tiled_scene::add_physics_to_map()
{
  auto result = false;

  do
  {
    const auto map = get_tiled_map();

    const auto shapes = map->getProperty("shapes").asString();

    auto cache = physics_body_cache::get_instance();
    cache->load(shapes, block_size_.width - 1, block_size_.height - 1);

    for (auto& child : map->getChildren())
    {
      const auto layer = dynamic_cast<experimental::TMXLayer*>(child);
      if (layer != nullptr)
      {
        auto physics = layer->getProperty("physics");
        if (physics.asBool())
        {
          for (auto col = 0; col < blocks_.height; col++)
          {
            for (auto row = 0; row < blocks_.width; row++)
            {
              const auto tile_pos = Vec2(row, col);
              const auto gid = layer->getTileGIDAt(tile_pos);
              if (gid != 0)
              {
                const auto node = create_dummy_node(layer, tile_pos);
                const auto shape = get_shape_from_tile_gid(gid);
                add_body_to_node(node, shape);
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
