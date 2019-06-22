#include "GameObject.h"

GameObject::GameObject() : _animation(nullptr), _type("") {}

GameObject* GameObject::create(const std::string& spriteFrameName, const std::string& type)
{
  GameObject* ret = nullptr;

  do
  {
    auto object = new GameObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(spriteFrameName, type))
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

GameObject* GameObject::create(const std::string& type)
{
  GameObject* ret = nullptr;

  do
  {
    auto object = new GameObject();
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

bool GameObject::init(const std::string& spriteFrameName, const std::string& type)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::initWithSpriteFrameName(spriteFrameName))

    _type = type;

    ret = true;
  } while (false);

  return ret;
}

bool GameObject::init(const std::string& type)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init())

    _type = type;

    ret = true;
  } while (false);

  return ret;
}

bool GameObject::createAnim(const char* pattern, const int maxFrame, const float speed, const char* name,
                            const int loops /* = INFINITE_LOOPS*/)
{
  auto ret = false;

  do
  {
    auto cache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> frames(maxFrame);

    auto loaded = 0;
    for (unsigned short int num = 1; num <= maxFrame; num++)
    {
      const auto frame = cache->getSpriteFrameByName(StringFormat(pattern, num));
      UTILS_BREAK_IF(frame == nullptr);

      frames.pushBack(frame);
      loaded++;
    }
    UTILS_BREAK_IF(loaded != maxFrame);

    auto anim = Animation::createWithSpriteFrames(frames);
    UTILS_BREAK_IF(anim == nullptr);

    anim->setLoops(loops);
    anim->setDelayPerUnit(speed);

    AnimationCache::getInstance()->addAnimation(anim, name);

    ret = true;
  } while (false);

  return ret;
}

void GameObject::changeAnim(const std::string& name)
{
  if (_animation != nullptr)
  {
    stopAction(_animation);
    _animation = nullptr;
  }
  _animation = Animate::create(AnimationCache::getInstance()->getAnimation(name));
  runAction(_animation);
}

void GameObject::changeFrame(const string& name)
{
  const auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
  setSpriteFrame(frame);
}
