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
#include "../utils/base/sprite/game_object.h"
#include "../game/robot_object.h"
#include "../game/laser_object.h"
#include "../game/switch_object.h"
#include "../game/door_object.h"
#include "../utils/physics/physics_shape_cache.h"
#include "../ui/game_ui.h"
#include "../ui/virtual_joy_stick.h"
#include "../utils/audio/audio_helper.h"
#include "loading_scene.h"

game_scene::game_scene() :
  robot_(nullptr),
  game_ui_(nullptr),
  last_robot_position_(Vec2::ZERO),
  last_camera_position_(Vec2::ZERO),
  min_camera_pos_(Vec2::ZERO),
  max_camera_pos_(Vec2::ZERO),
  paused_(false),
  total_time_(0.f),
  barrel_count_(0),
  time_limit_(0)
{
}

game_scene::~game_scene()
{
  audio_helper::get_instance()->end();
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

    // calculate the maximum position that we could move  
    min_camera_pos_ = Vec2(screen_size_.width / 2, screen_size_.height / 2);
    max_camera_pos_ = Vec2(total_size_.width - min_camera_pos_.x, total_size_.height - min_camera_pos_.y);

    //load objects
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("objects/objects.plist");

    // game ui
    game_ui_ = game_ui::create();
    UTILS_BREAK_IF(game_ui_ == nullptr);

    game_ui_->setAnchorPoint(Vec2(0.f, 0.f));

    addChild(game_ui_);

    // add the objects
    UTILS_BREAK_IF(!add_objects_to_game());

    // clear the cache
    physics_shape_cache::get_instance()->remove_all_shapes();

    // get the time limit
    time_limit_ = static_cast<unsigned int>(get_tiled_map()->getProperty("time_limit").asInt());

    //get updates
    scheduleUpdate();

    const auto debug_grid = UserDefault::getInstance()->getBoolForKey("debug_grid", false);
    if (debug_grid)
    {
      UTILS_BREAK_IF(!create_debug_grid("fonts/tahoma.ttf"));
    }

    auto contact_listener = EventListenerPhysicsContact::create();
    contact_listener->onContactBegin = CC_CALLBACK_1(game_scene::on_contact_begin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contact_listener, this);

    audio_helper::pre_load_music("sounds/music.ogg");
    audio_helper::pre_load_effect("sounds/fail.ogg");
    audio_helper::pre_load_effect("sounds/victory.ogg");

    audio_helper::get_instance()->play_music("sounds/music.ogg", 0.30f);
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

Node* game_scene::provide_physics_node(const int gid) const
{
  auto damage = 0;
  const auto map = get_tiled_map();
  if (map->getPropertiesForGID(gid).getType() == Value::Type::MAP)
  {
    const auto properties = map->getPropertiesForGID(gid).asValueMap();
    if (properties.count("damage") == 1)
    {
      damage = properties.at("damage").asInt();
    }
  }

  return game_object::create("dummy", damage);
}

void game_scene::update(float delta)
{
  if (!paused_)
  {
    total_time_ += delta;
    game_ui_->update_time(total_time_);

    const auto shield = robot_->get_shield_percentage();
    game_ui_->set_shield_percentage(shield);

    if (shield == 0.0f)
    {
      game_over(false);
      return;
    }

    robot_->update(delta);
    const auto new_position = robot_->getPosition();

    if (last_robot_position_ != new_position)
    {
      update_camera(delta);

      last_robot_position_ = new_position;
    }
  }
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

bool game_scene::add_laser(const ValueMap& values, Node* layer)
{
  auto ret = false;

  do
  {
    const auto name = values.at("name").asString();
    const auto rotation = values.at("rotation").asFloat();
    const auto position = get_object_center_position(values);
    const auto damage = values.at("damage").asInt();
    const auto speed = values.at("speed").asFloat();

    auto laser = laser_object::create(rotation, speed, damage);
    UTILS_BREAK_IF(laser == nullptr);

    laser->setPosition(position);
    layer->addChild(laser);

    game_objects_[name] = laser;

    ret = true;
  }
  while (false);


  return ret;
}

bool game_scene::add_robot(const ValueMap& values, Node* layer)
{
  auto ret = false;
  do
  {
    const auto shield = values.at("shield").asInt();
    robot_ = robot_object::create(game_ui_->get_virtual_joy_stick(), shield);
    UTILS_BREAK_IF(robot_ == nullptr);

    auto position = get_object_center_position(values);
    position.y -= (robot_->getContentSize().height - block_size_.height) / 2;
    robot_->setPosition(position);
    layer->addChild(robot_);

    ret = true;
  }
  while (false);

  return ret;
}

bool game_scene::add_object(const vector<Value>::value_type& object)
{
  auto ret = false;
  do
  {
    const auto map = get_tiled_map();
    UTILS_BREAK_IF(map == nullptr);

    const auto layer_walk = map->getLayer("walk");
    UTILS_BREAK_IF(layer_walk == nullptr);

    const auto layer_walk_back = map->getLayer("walk_back");
    UTILS_BREAK_IF(layer_walk_back == nullptr);

    const auto& values = object.asValueMap();
    if (values.at("type").asString() == "laser")
    {
      UTILS_BREAK_IF(!add_laser(values, layer_walk));
    }
    else if (values.at("type").asString() == "robot")
    {
      UTILS_BREAK_IF(!add_robot(values, layer_walk));
    }
    else if (values.at("type").asString() == "saw")
    {
      UTILS_BREAK_IF(!add_saw(values, layer_walk_back));
    }
    else if (values.at("type").asString() == "barrel")
    {
      UTILS_BREAK_IF(!add_barrel(values, layer_walk_back));
    }
    else if (values.at("type").asString() == "switch")
    {
      UTILS_BREAK_IF(!add_switch(values, layer_walk));
    }
    else if (values.at("type").asString() == "door")
    {
      UTILS_BREAK_IF(!add_door(values, layer_walk_back));
    }

    ret = true;
  }
  while (false);
  return ret;
}

bool game_scene::add_switch(const ValueMap& values, Node* layer)
{
  auto ret = false;

  do
  {
    const auto name = values.at("name").asString();
    const auto target = values.at("target").asString();

    auto switch_game_object = switch_object::create(target);
    UTILS_BREAK_IF(switch_game_object == nullptr);

    switch_game_object->setAnchorPoint(Vec2(0.5f, 0.f));

    auto position = get_object_position(values);
    position.y += (values.at("height").asFloat() / 2);

    switch_game_object->setPosition(position);

    layer->addChild(switch_game_object);

    game_objects_[name] = switch_game_object;

    ret = true;
  }
  while (false);

  return ret;
}

bool game_scene::add_door(const ValueMap& values, Node* layer)
{
  auto ret = false;

  do
  {
    const auto name = values.at("name").asString();
    auto door_game_object = door_object::create();
    UTILS_BREAK_IF(door_game_object == nullptr);

    door_game_object->setAnchorPoint(Vec2(0.5f, 0.f));

    auto position = get_object_position(values);
    position.y += (values.at("height").asFloat() / 2);

    door_game_object->setPosition(position);

    layer->addChild(door_game_object);

    game_objects_[name] = door_game_object;

    ret = true;
  }
  while (false);

  return ret;
}

bool game_scene::add_barrel(const ValueMap& values, Node* layer)
{
  auto ret = false;

  do
  {
    const auto name = values.at("name").asString();
    const auto image = values.at("image").asString();
    auto sprite = game_object::create(image, "barrel");
    UTILS_BREAK_IF(sprite == nullptr);

    sprite->setAnchorPoint(Vec2(0.5f, 0.f));

    const auto position = get_object_center_position(values);
    sprite->setPosition(position);
    sprite->setAnchorPoint(Vec2(0.5f, 0.5f));

    // making look like random but is not, base on how may barrels we add    
    barrel_count_++;

    const auto gap = barrel_count_ * 10.f;
    auto step_time = 0.f;
    auto step = 0.f;
    auto amount = 0.f;

    amount = 50.f;
    step = amount + gap;
    step_time = step / amount;
    const auto move_down_step_1 = MoveBy::create(step_time, Vec3(0.0f, step, 0.0f));

    amount = 40.f;
    step = -(amount + gap);
    step_time = step / -amount;
    const auto move_up_step_1 = MoveBy::create(step_time, Vec3(0.0f, step, 0.0f));

    amount = 30.f;
    step = amount + gap;
    step_time = step / amount;
    const auto move_down_step_2 = MoveBy::create(step_time, Vec3(0.0f, step, 0.0f));

    amount = 40.f;
    step = -(amount + gap);
    step_time = step / -amount;
    const auto move_up_step_2 = MoveBy::create(step_time, Vec3(0.0f, step, 0.0f));

    const auto movement = Sequence::create(move_down_step_1, move_up_step_1, move_down_step_2, move_up_step_2, nullptr);
    const auto repeat_movement = RepeatForever::create(movement);
    sprite->runAction(repeat_movement);

    const auto shape = values.count("shape") == 1 ? values.at("shape").asString() : "";
    add_body_to_node(sprite, shape);

    const auto rotation = values.at("rotation").asFloat();
    sprite->setRotation(rotation);

    layer->addChild(sprite);

    game_objects_[name] = sprite;

    ret = true;
  }
  while (false);

  return ret;
}

bool game_scene::add_saw(const ValueMap& values, Node* layer)
{
  auto ret = false;

  do
  {
    const auto name = values.at("name").asString();
    const auto image = values.at("image").asString();
    const auto damage = values.at("damage").asInt();
    auto sprite = game_object::create(image, "saw", damage);
    UTILS_BREAK_IF(sprite == nullptr);

    sprite->setAnchorPoint(Vec2(0.5f, 0.f));

    const auto rotation_time = values.at("rotation_time").asFloat();
    const auto movement = values.at("movement").asFloat();
    const auto movement_time = values.at("movement_time").asFloat();
    const auto stop_time = values.at("stop_time").asFloat();

    const auto position = get_object_center_position(values);
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
    add_body_to_node(sprite, shape);

    layer->addChild(sprite);

    game_objects_[name] = sprite;

    ret = true;
  }
  while (false);

  return ret;
}

bool game_scene::add_objects_to_game()
{
  auto result = false;

  do
  {
    const auto map = get_tiled_map();
    const auto objects = map->getObjectGroup("objects");

    for (const auto& object : objects->getObjects())
    {
      UTILS_BREAK_IF(!add_object(object));
    }

    result = true;
  }
  while (false);

  return result;
}

unsigned short int game_scene::calculate_stars() const
{
  unsigned short int stars = 1;

  if (total_time_ <= time_limit_)
  {
    stars = 2;
    if (robot_->get_shield_percentage() == 100.f)
    {
      stars = 3;
    }
  }

  return stars;
}

void game_scene::game_over(const bool win)
{
  do
  {
    pause();
    game_ui_->disable_pause();

    if (win)
    {
      audio_helper::get_instance()->play_effect("sounds/victory.ogg");
      game_ui_->display_message("Level Completed", true, calculate_stars(), time_limit_);
    }
    else
    {
      audio_helper::get_instance()->play_effect("sounds/fail.ogg");
      game_ui_->display_message("Game Over", false);
    }
  }
  while (false);
}

void game_scene::pause()
{
  paused_ = true;
  base_class::pause();
  getPhysicsWorld()->setAutoStep(false);
  robot_->pause();
  for (const auto& game_object : game_objects_)
  {
    game_object.second->pause();
  }
  audio_helper::get_instance()->pause_music();

  game_ui_->get_virtual_joy_stick()->pause();
}

void game_scene::resume()
{
  base_class::resume();
  getPhysicsWorld()->setAutoStep(true);
  robot_->resume();
  for (const auto& game_object : game_objects_)
  {
    game_object.second->resume();
  }
  audio_helper::get_instance()->resume_music();
  game_ui_->get_virtual_joy_stick()->resume();

  paused_ = false;
}

void game_scene::toggle_pause()
{
  if (paused_)
  {
    resume();
  }
  else
  {
    pause();
  }
}

void game_scene::reload()
{
  pause();
  game_ui_->disable_pause();
  Director::getInstance()->replaceScene(loading_scene::game());
}

void game_scene::update_camera(const float delta)
{
  // move the camera to the clamped position
  const auto final_pos = robot_->getPosition().getClampPoint(min_camera_pos_, max_camera_pos_);
  if (final_pos != last_camera_position_)
  {
    getDefaultCamera()->setPosition(final_pos);
    getDefaultCamera()->update(delta);

    const auto ui_pos = Vec2(final_pos.x - (screen_size_.width / 2), final_pos.y - (screen_size_.height / 2));
    game_ui_->setPosition(ui_pos);

    last_camera_position_ = final_pos;
  }
}

void game_scene::handle_switch(switch_object* switch_game_object)
{
  if (switch_game_object->is_off())
  {
    const auto target = switch_game_object->get_target();
    if (game_objects_.count(target) == 1)
    {
      const auto target_object = game_objects_.at(target);

      if (target_object->get_type() == "switch")
      {
        switch_game_object->on();

        const auto target_switch = dynamic_cast<switch_object*>(target_object);
        target_switch->on();

        const auto target_target = target_switch->get_target();
        if (game_objects_.count(target_target) == 1)
        {
          const auto target_target_object = game_objects_.at(target_target);
          if (target_target_object->get_type() == "door")
          {
            const auto target_door = dynamic_cast<door_object*>(target_target_object);
            if (target_door->is_off())
            {
              target_door->on();
            }
          }
        }
      }
    }
  }
}

void game_scene::handle_door(door_object* door_game_object)
{
  if (door_game_object->is_on())
  {
    if (door_game_object->is_closed())
    {
      door_game_object->open();
    }
    else
    {
      game_over(true);
    }
  }
}

bool game_scene::on_contact_begin(PhysicsContact& contact)
{
  if (!paused_)
  {
    const auto robot = get_object_from_contact<robot_object>(contact, bit_mask_robot);
    if (robot != nullptr)
    {
      const auto door = get_object_from_contact<door_object>(contact, bit_mask_door);
      if (door != nullptr)
      {
        handle_door(door);
      }
      else
      {
        const auto switch_game_object = get_object_from_contact<switch_object>(contact, bit_mask_switch);
        if (switch_game_object != nullptr)
        {
          handle_switch(switch_game_object);
        }
        else
        {
          const auto block_objects = bit_mask_blocks | bit_mask_spikes | bit_mask_acid | bit_mask_saw | bit_mask_barrel;

          const auto block_game_object = get_object_from_contact<game_object>(contact, block_objects);
          if (block_game_object != nullptr)
          {
            if (block_game_object->getPositionY() < robot->getPositionY())
            {
              robot->on_land_on_block();
            }
            const auto damage = block_game_object->get_damage();
            if (damage != 0)
            {
              robot->damage_shield(damage);
            }
          }
        }
      }
    }
  }

  return true;
}


template <class Type>
Type* game_scene::get_object_from_contact(PhysicsContact& contact, const unsigned short category)
{
  Type* object = nullptr;

  if (contact.getShapeA()->getCategoryBitmask() & category)
  {
    object = dynamic_cast<Type*>(contact.getShapeA()->getBody()->getNode());
  }
  else if (contact.getShapeB()->getCategoryBitmask() & category)
  {
    object = dynamic_cast<Type*>(contact.getShapeB()->getBody()->getNode());
  }

  return object;
}
