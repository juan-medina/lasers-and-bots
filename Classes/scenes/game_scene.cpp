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
    const auto debug_physics = UserDefault::getInstance()->getBoolForKey("debug_physics", false);

    UTILS_BREAK_IF(!base_class::init("maps/map.tmx", gravity, debug_physics));

    //create bot
    UTILS_BREAK_IF(!add_robot());

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

bool game_scene::add_robot()
{
  auto result = false;

  do
  {
    robot_ = robot_object::create();
    UTILS_BREAK_IF(robot_ == nullptr);
    addChild(robot_);

    const auto start_row = get_tiled_map()->getProperty("startX").asInt();
    const auto start_col = get_tiled_map()->getProperty("startY").asInt();
    auto pos = get_block_position(start_col + 1, start_row);
    const auto block_size = get_block_size();
    const auto robot_pos = Vec2(pos.getMinX() - (block_size.width / 2), pos.getMinY() - (block_size.height / 4));
    robot_->setPosition(robot_pos);

    result = true;
  }
  while (false);

  return result;
}


bool game_scene::add_lasers_to_game()
{
  auto result = false;

  do
  {
    const auto map = get_tiled_map();

    auto layer = map->getLayer("laser");
    UTILS_BREAK_IF(layer == nullptr);

    for (auto col = 0; col < blocks_.height; col++)
    {
      for (auto row = 0; row < blocks_.width; row++)
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
  getDefaultCamera()->setPosition(final_pos);
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

    const auto blink = Sequence::create(FadeTo::create(0.5f, 64), FadeTo::create(0.5f, 255), nullptr);
    const auto repeat_blink = RepeatForever::create(blink);
    sprite->runAction(repeat_blink);
    laser->runAction(repeat_blink->clone());
    laser->setCascadeOpacityEnabled(true);
    ret = true;
  }
  while (false);

  return ret;
}
