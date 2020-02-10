#include "PhysicsGameObject.h"

#include "PhysicsShapeCache.h"

PhysicsGameObject* PhysicsGameObject::create(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
                                             const std::string& spriteFrameName, const std::string& type)
{
  PhysicsGameObject* ret = nullptr;

  do
  {
    auto object = new PhysicsGameObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physicsShapeCache, shape, spriteFrameName, type))
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

PhysicsGameObject* PhysicsGameObject::create(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
                                             const std::string& type)
{
  PhysicsGameObject* ret = nullptr;

  do
  {
    auto object = new PhysicsGameObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physicsShapeCache, shape, type))
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

PhysicsGameObject* PhysicsGameObject::create(const std::string& type)
{
  PhysicsGameObject* ret = nullptr;

  do
  {
    auto object = new PhysicsGameObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(type))
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

bool PhysicsGameObject::init(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
                             const std::string& spriteFrameName, const std::string& type)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init(spriteFrameName, type))

    UTILS_BREAK_IF(!setShape(physicsShapeCache, shape));

    ret = true;
  } while (false);

  return ret;
}

bool PhysicsGameObject::init(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
                             const std::string& type)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init(type))

    UTILS_BREAK_IF(!setShape(physicsShapeCache, shape));

    ret = true;
  } while (false);

  return ret;
}

bool PhysicsGameObject::init(const std::string& type)
{
  return BaseClass::init(type);
}

bool PhysicsGameObject::setShape(PhysicsShapeCache* physicsShapeCache, const std::string& shapeName)
{
  auto ret = false;

  do
  {
    if (!shapeName.empty())
    {
      const auto body = physicsShapeCache->createBodyWithName(shapeName);
      UTILS_BREAK_IF(body == nullptr);

      setPhysicsBody(body);
    }
    ret = true;
  } while (false);

  return ret;
}
