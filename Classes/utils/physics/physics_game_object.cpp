#include "physics_game_object.h"
#include "physics_shape_cache.h"

physics_game_object* physics_game_object::create(const std::string& shape, const std::string& sprite_frame_name,
                                                 const std::string& type)
{
  physics_game_object* ret = nullptr;

  do
  {
    auto object = new physics_game_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(shape, sprite_frame_name, type))
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

  return ret;
}

physics_game_object* physics_game_object::create(const std::string& shape, const std::string& type)
{
  physics_game_object* ret = nullptr;

  do
  {
    auto object = new physics_game_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(shape, type))
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

  return ret;
}

bool physics_game_object::init(const std::string& shape, const std::string& sprite_frame_name, const std::string& type)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(sprite_frame_name, type))

    UTILS_BREAK_IF(!set_shape(shape));

    ret = true;
  }
  while (false);

  return ret;
}

bool physics_game_object::init(const std::string& shape, const std::string& type)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(type))

    UTILS_BREAK_IF(!set_shape(shape));

    ret = true;
  }
  while (false);

  return ret;
}


bool physics_game_object::set_shape(const std::string& shape_name)
{
  auto ret = false;

  do
  {
    if (!shape_name.empty())
    {
      const auto cache = physics_shape_cache::get_instance();

      const auto body = cache->create_body_with_name(shape_name);
      UTILS_BREAK_IF(body == nullptr);

      setPhysicsBody(body);
    }
    ret = true;
  }
  while (false);

  return ret;
}
