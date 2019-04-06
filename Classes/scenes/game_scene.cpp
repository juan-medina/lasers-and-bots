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

#include "game_scene.h"
#include "../game/robot_object.h"
#include "../game/laser_object.h"
#include "rapidjson/document.h"

game_scene::game_scene() :
  robot_(nullptr)
{
}

game_scene* game_scene::create()
{
  game_scene* ret = nullptr;

  do
  {
    auto object = new game_scene();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init())
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  }
  while (false);

  // return the object
  return ret;
}

// on "init" you need to initialize your instance
bool game_scene::init()
{
  auto ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first
    UTILS_BREAK_IF(!base_class::init("maps/map.tmx"));

    //create bot
    UTILS_BREAK_IF(!this->add_robot());

    //init physics
    init_physics();

    // add physics to map
    UTILS_BREAK_IF(!add_physics_to_map());

    // add the lasers
    UTILS_BREAK_IF(!add_lasers_to_game());

    //get updates
    scheduleUpdate();

    const auto debug_grid = UserDefault::getInstance()->getBoolForKey("debug_grid", false);
    if (debug_grid)
    {
      UTILS_BREAK_IF(!create_debug_grid("fonts/Marker Felt.ttf"));
    }

    ret = true;
  }
  while (false);

  return ret;
}

Scene* game_scene::scene()
{
  // create the grid
  auto scene = new game_scene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init())
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

void game_scene::update(float delta)
{
  robot_->update(delta);

  update_camera();
}

void game_scene::init_physics() const
{
  const auto edge = PhysicsBody::createEdgeBox(this->total_size_, PhysicsMaterial(0.1f, 0.0f, 0.5f), 5);
  this->get_tiled_map()->addComponent(edge);

  const auto debug_physics = UserDefault::getInstance()->getBoolForKey("debug_physics", false);
  if (debug_physics)
  {
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
  }

  getPhysicsWorld()->setGravity(Vec2(0.0f, gravity));
  getPhysicsWorld()->setSubsteps(4);
}

bool game_scene::add_robot()
{
  auto result = false;

  do
  {
    robot_ = robot_object::create();
    UTILS_BREAK_IF(robot_ == nullptr);
    this->addChild(robot_);

    const auto start_row = get_tiled_map()->getProperty("startX").asInt();
    const auto start_col = get_tiled_map()->getProperty("startY").asInt();
    auto pos = this->get_block_position(start_col + 1, start_row);
    const auto block_size = get_block_size();
    const auto robot_pos = Vec2(pos.getMinX() - (block_size.width / 2), pos.getMinY() - (block_size.height / 4));
    robot_->setPosition(robot_pos);

    result = true;
  }
  while (false);

  return result;
}

bool game_scene::add_physics_to_map()
{
  auto result = false;

  do
  {
    const auto map = get_tiled_map();

    auto layer = map->getLayer("walk");
    UTILS_BREAK_IF(layer == nullptr);

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

    result = true;
  }
  while (false);

  return result;
}

string game_scene::get_shape_from_tile_gid(int gid)
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

bool game_scene::add_body_to_sprite(Sprite* sprite, const string& shape)
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

PhysicsBody* game_scene::get_body_from_shape(const string& shape, const PhysicsMaterial& material,
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

bool game_scene::add_lasers_to_game()
{
  auto result = false;

  do
  {
    const auto map = get_tiled_map();

    auto layer = map->getLayer("laser");
    UTILS_BREAK_IF(layer == nullptr);

    for (auto col = 0; col < this->blocks_.height; col++)
    {
      for (auto row = 0; row < this->blocks_.width; row++)
      {
        const auto sprite = layer->getTileAt(Vec2(row, col));
        if (sprite != nullptr)
        {
          add_laser_at_sprite(sprite);
        }
      }
    }

    result = true;
  }
  while (false);

  return result;
}

void game_scene::update_camera() const
{
  // calculate the maximum position that we could move  
  const auto min_pos = Vec2(screen_size_.width / 2, screen_size_.height / 2);
  const auto max_pos = Vec2(total_size_.width - min_pos.x, total_size_.height - min_pos.y);

  // move the camera to the clamped position
  const auto final_pos = robot_->getPosition().getClampPoint(min_pos, max_pos);
  this->getDefaultCamera()->setPosition(final_pos);
}


bool game_scene::add_laser_at_sprite(Sprite* sprite)
{
  auto ret = false;

  do
  {
    auto pos = sprite->getPosition();

    pos.x += sprite->getContentSize().width / 2;
    pos.y += sprite->getContentSize().height / 2;

    auto laser = laser_object::create();
    UTILS_BREAK_IF(laser == nullptr);
    laser->setPosition(pos);

    addChild(laser);

    sprite->setBlendFunc(BlendFunc::ADDITIVE);

    const auto blink = Sequence::create(FadeTo::create(0.5f, 20), FadeTo::create(0.5f, 255), nullptr);
    const auto repeat = RepeatForever::create(blink);
    sprite->runAction(repeat);

    ret = true;
  }
  while (false);

  return ret;
}
