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
#include "../utils/physics/physics_body_cache.h"
#include "../ui/game_ui.h"

game_scene::game_scene() :
  robot_(nullptr),
  game_ui_(nullptr)
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
    const auto debug_physics = UserDefault::getInstance()->getBoolForKey("debug_physics", false);

    UTILS_BREAK_IF(!base_class::init("maps/map.tmx", gravity, debug_physics));

    //load objects
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("objects/objects.plist");

    // game ui
    game_ui_ = game_ui::create();
    UTILS_BREAK_IF(game_ui_ == nullptr);

    game_ui_->setAnchorPoint(Vec2(0.f, 0.f));

    addChild(game_ui_);

    // add the lasers
    UTILS_BREAK_IF(!add_objects_to_game());

    // clear the cache
    physics_body_cache::get_instance()->clear();

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

Vec2 game_scene::get_object_center_position(const ValueMap& values)
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

  return Vec2(x + rotated_center_x - width, y + rotated_center_y);
}

Vec2 game_scene::get_object_position(const ValueMap& values)
{
  const auto width = values.at("width").asFloat();
  const auto height = values.at("height").asFloat();
  const auto x = values.at("x").asFloat() + width / 2;
  const auto y = values.at("y").asFloat() + height / 2;

  return Vec2(x, y);
}

bool game_scene::add_objects_to_game()
{
  auto result = false;

  do
  {
    const auto map = get_tiled_map();

    auto layer_back = map->getLayer("walk");
    UTILS_BREAK_IF(layer_back == nullptr);

    auto layer_walk_back = map->getLayer("walk_back");
    UTILS_BREAK_IF(layer_walk_back == nullptr);

    const auto objects = map->getObjectGroup("objects");
    for (const auto& object : objects->getObjects())
    {
      const auto& values = object.asValueMap();
      if (values.at("type").asString() == "laser")
      {
        const auto rotation = values.at("rotation").asFloat();
        const auto position = get_object_center_position(values);

        auto laser = laser_object::create(rotation);
        UTILS_BREAK_IF(laser == nullptr);

        laser->setPosition(position);
        layer_back->addChild(laser);
      }
      else if (values.at("type").asString() == "robot")
      {
        robot_ = robot_object::create(game_ui_->get_virtual_joy_stick());
        UTILS_BREAK_IF(robot_ == nullptr);

        const auto position = get_object_position(values);

        robot_->setPosition(position);
        layer_back->addChild(robot_);
      }
      else if (values.at("type").asString() == "scenery")
      {
        const auto image = values.at("image").asString();
        const auto name = values.at("name").asString();

        auto sprite = Sprite::createWithSpriteFrameName(image);
        UTILS_BREAK_IF(sprite == nullptr);

        sprite->setAnchorPoint(Vec2(0.5f, 0.f));

        auto position = get_object_position(values);
        position.y += (values.at("height").asFloat() / 2);

        sprite->setPosition(position);
        if (name == "switch")
        {
          layer_back->addChild(sprite);
        }
        else if (name == "saw")
        {
          const auto rotation_time = values.at("rotation_time").asFloat();
          const auto movement = values.at("movement").asFloat();
          const auto movement_time = values.at("movement_time").asFloat();
          const auto stop_time = values.at("stop_time").asFloat();

          position = get_object_center_position(values);
          sprite->setPosition(position);
          sprite->setAnchorPoint(Vec2(0.5f, 0.5f));

          const auto move_right = MoveBy::create(movement_time, Vec2(movement, 0.f));
          const auto move_left = MoveBy::create(movement_time, Vec2(-movement, 0.f));
          const auto delay = DelayTime::create(stop_time);
          const auto movement_sequence = Sequence::create(move_right, delay, move_left, delay, nullptr);
          const auto repeat_movement = RepeatForever::create(movement_sequence);
          sprite->runAction(repeat_movement);

          const auto rotation = RotateBy::create(rotation_time, 360.f);
          const auto repeat_rotation = RepeatForever::create(rotation);
          sprite->runAction(repeat_rotation);

          const auto shape = values.at("shape").asString();
          const auto restitution = values.at("restitution").asFloat();
          add_body_to_node(sprite, shape, restitution);


          layer_walk_back->addChild(sprite);
        }
        else
        {
          layer_walk_back->addChild(sprite);
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
  getDefaultCamera()->setPosition(final_pos);

  const auto ui_pos = Vec2(final_pos.x - (screen_size_.width / 2), final_pos.y - (screen_size_.height / 2));
  game_ui_->setPosition(ui_pos);
}
