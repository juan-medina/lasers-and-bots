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

#include "physics_tiled_scene.h"
#include "physics_shape_cache.h"

physics_tiled_scene::physics_tiled_scene():
  physics_shape_cache_(nullptr)
{
}

physics_tiled_scene::~physics_tiled_scene()
{
  physics_shape_cache_->remove_all_shapes();
  delete physics_shape_cache_;
  physics_shape_cache_ = nullptr;
}

physics_tiled_scene* physics_tiled_scene::create(basic_app* application, const std::string& tmx_file,
                                                 const float gravity,
                                                 const bool debug_physics)
{
  auto scene = new physics_tiled_scene();

  if (scene)
  {
    if (scene->init(application, tmx_file, gravity, debug_physics))
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

Scene* physics_tiled_scene::scene(basic_app* application, const std::string& tmx_file, const float gravity,
                                  const bool debug_physics)
{
  auto scene = new physics_tiled_scene();

  if (scene)
  {
    if (scene->init(application, tmx_file, gravity, debug_physics))
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

bool physics_tiled_scene::init(basic_app* application, const std::string& tmx_file, const float gravity,
                               const bool debug_physics)
{
  auto ret = false;

  do
  {
    ret = base_class::init(application, tmx_file);
    UTILS_BREAK_IF(!ret);

    gravity_ = gravity;

    init_physics(debug_physics);

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

string physics_tiled_scene::get_shape_from_tile_gid(const int gid)
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

Node* physics_tiled_scene::provide_physics_node(const int gid)
{
  const auto node = Node::create();
  const auto shape = get_shape_from_tile_gid(gid);
  if (!shape.empty())
  {
    add_body_to_node(node, shape);
  }
  return node;
}

bool physics_tiled_scene::add_body_to_node(Node* node, const string& shape) const
{
  auto result = false;

  do
  {
    PhysicsBody* body = nullptr;
    if (!shape.empty())
    {
      body = physics_shape_cache_->create_body_with_name(shape);
    }

    if (body != nullptr)
    {
      node->setPhysicsBody(body);
    }

    result = true;
  }
  while (false);

  return result;
}

Node* physics_tiled_scene::create_dummy_node(experimental::TMXLayer* const layer, const Vec2& tile_pos,
                                             const int gid)
{
  const auto node = provide_physics_node(gid);
  node->setAnchorPoint(Vec2(0, 0));
  node->setContentSize(block_size_);
  node->setPosition(layer->getPositionAt(tile_pos) + (block_size_ / 2));
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

    physics_shape_cache_ = new physics_shape_cache();
    UTILS_BREAK_IF(physics_shape_cache_ == nullptr);

    UTILS_BREAK_IF(!physics_shape_cache_->add_shapes_with_file(shapes));

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
                create_dummy_node(layer, tile_pos, gid);
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
