#include "HarmObject.h"

HarmObject::HarmObject() : _damage(0) {}

HarmObject* HarmObject::create(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
                               const std::string& spriteFrameName, const std::string& type, const int damage)
{
  HarmObject* ret = nullptr;

  do
  {
    auto object = new HarmObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physicsShapeCache, shape, spriteFrameName, type, damage))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  } while (false);

  return ret;
}

HarmObject* HarmObject::create(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
                               const std::string& type, const int damage)
{
  HarmObject* ret = nullptr;

  do
  {
    auto object = new HarmObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physicsShapeCache, shape, type, damage))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  } while (false);

  return ret;
}

HarmObject* HarmObject::create(const std::string& type, const int damage)
{
  HarmObject* ret = nullptr;

  do
  {
    auto object = new HarmObject();
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
  } while (false);

  return ret;
}

bool HarmObject::init(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
                      const std::string& spriteFrameName, const std::string& type, const int damage)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init(physicsShapeCache, shape, spriteFrameName, type))

    _damage = damage;

    ret = true;
  } while (false);

  return ret;
}

bool HarmObject::init(PhysicsShapeCache* physicsShapeCache, const std::string& shape, const std::string& type,
                      const int damage)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init(physicsShapeCache, shape, type))

    _damage = damage;

    ret = true;
  } while (false);

  return ret;
}

bool HarmObject::init(const std::string& type, const int damage)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init(type))

    _damage = damage;

    ret = true;
  } while (false);

  return ret;
}
