
#include "GameObject.h"

GameObject::GameObject() :
  _animation(nullptr)
{
}

GameObject::~GameObject()
{
}

GameObject* GameObject::create(const std::string& spriteFrameName)
{
  GameObject* ret = nullptr;

  do
  {
    auto object = new GameObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(spriteFrameName))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  } while (0);

  // return the object
  return ret;
}

// on "init" you need to initialize your instance
bool GameObject::init(const std::string& spriteFrameName)
{
  bool ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first
    UTILS_BREAK_IF(!parent::initWithSpriteFrameName(spriteFrameName))

    ret = true;

  } while (0);

  return ret;
}

bool GameObject::createAnim(const char* pattern, const int maxFrame, const float speed, const char* name, unsigned const int loops/* = -1*/)
{
  bool ret = false;

  do
  {
    auto cache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame *> frames(maxFrame);

    int loaded = 0;
    for (unsigned short int num = 1; num <= maxFrame; num++)
    {
      char name[255];
      std::snprintf(name, 255, pattern, num);

      auto frame = cache->getSpriteFrameByName(name);
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
  } while (0);

  return ret;
}

void GameObject::changeAnim(const char* name)
{
  if (_animation != nullptr)
  {
    stopAction(_animation);
    _animation = nullptr;
  }
  _animation = Animate::create(AnimationCache::getInstance()->getAnimation(name));
  runAction(_animation);
}

bool GameObject::fuzzyEquals(const float a, const float b, float var/*=5.0f*/) const
{
  if (a - var <= b && b <= a + var)
    return true;
  return false;
}
