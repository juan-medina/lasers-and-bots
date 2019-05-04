#include "harm_object.h"

harm_object::harm_object() :
  damage_(0)
{
}


harm_object* harm_object::create(physics_shape_cache* physics_shape_cache, const std::string& shape,
                                 const std::string& sprite_frame_name, const std::string& type, const int damage)
{
  harm_object* ret = nullptr;

  do
  {
    auto object = new harm_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physics_shape_cache, shape, sprite_frame_name, type, damage))
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

harm_object* harm_object::create(physics_shape_cache* physics_shape_cache, const std::string& shape,
                                 const std::string& type, const int damage)
{
  harm_object* ret = nullptr;

  do
  {
    auto object = new harm_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physics_shape_cache, shape, type, damage))
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

harm_object* harm_object::create(const std::string& type, const int damage)
{
  harm_object* ret = nullptr;

  do
  {
    auto object = new harm_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(type, damage))
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

bool harm_object::init(physics_shape_cache* physics_shape_cache, const std::string& shape,
                       const std::string& sprite_frame_name, const std::string& type, const int damage)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(physics_shape_cache, shape, sprite_frame_name, type))

    damage_ = damage;

    ret = true;
  }
  while (false);

  return ret;
}

bool harm_object::init(physics_shape_cache* physics_shape_cache, const std::string& shape, const std::string& type,
                       const int damage)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(physics_shape_cache,shape, type))

    damage_ = damage;

    ret = true;
  }
  while (false);

  return ret;
}

bool harm_object::init(const std::string& type, const int damage)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(type))

    damage_ = damage;

    ret = true;
  }
  while (false);

  return ret;
}
