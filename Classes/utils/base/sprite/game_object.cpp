#include "game_object.h"
#include "../../physics/physics_shape_cache.h"

game_object::game_object() :
  animation_(nullptr),
  type_(""),
  damage_(0)
{
}


game_object* game_object::create(const std::string& sprite_frame_name, const std::string& type,
                                 const int damage/* = 0*/)
{
  game_object* ret = nullptr;

  do
  {
    auto object = new game_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(sprite_frame_name, type, damage))
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

game_object* game_object::create(const std::string& type, const int damage /*= 0*/)
{
  game_object* ret = nullptr;

  do
  {
    auto object = new game_object();
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

  // return the object
  return ret;
}

// on "init" you need to initialize your instance
bool game_object::init(const std::string& sprite_frame_name, const std::string& type, const int damage /*= 0*/)
{
  auto ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first
    UTILS_BREAK_IF(!base_class::initWithSpriteFrameName(sprite_frame_name))

    type_ = type;
    damage_ = damage;

    ret = true;
  }
  while (false);

  return ret;
}

bool game_object::init(const std::string& type, const int damage /*= 0*/)
{
  auto ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first
    UTILS_BREAK_IF(!base_class::init())

    type_ = type;
    damage_ = damage;

    ret = true;
  }
  while (false);

  return ret;
}

bool game_object::set_shape(const std::string& shape_name)
{
  auto ret = false;

  do
  {
    const auto cache = physics_shape_cache::get_instance();

    const auto body = cache->create_body_with_name(shape_name);
    UTILS_BREAK_IF(body == nullptr);

    setPhysicsBody(body);
    ret = true;
  }
  while (false);

  return ret;
}

bool game_object::create_anim(const char* pattern, const int max_frame, const float speed, const char* name,
                              unsigned const int loops/* = infinite_loops*/)
{
  auto ret = false;

  do
  {
    auto cache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame *> frames(max_frame);

    auto loaded = 0;
    for (unsigned short int num = 1; num <= max_frame; num++)
    {
      const auto frame = cache->getSpriteFrameByName(string_format(pattern, num));
      UTILS_BREAK_IF(frame == nullptr);

      frames.pushBack(frame);
      loaded++;
    }
    UTILS_BREAK_IF(loaded != max_frame);

    auto anim = Animation::createWithSpriteFrames(frames);
    UTILS_BREAK_IF(anim == nullptr);

    anim->setLoops(loops);
    anim->setDelayPerUnit(speed);

    AnimationCache::getInstance()->addAnimation(anim, name);

    ret = true;
  }
  while (false);

  return ret;
}

void game_object::change_anim(const std::string& name)
{
  if (animation_ != nullptr)
  {
    stopAction(animation_);
    animation_ = nullptr;
  }
  animation_ = Animate::create(AnimationCache::getInstance()->getAnimation(name));
  runAction(animation_);
}

void game_object::change_frame(const string& name)
{
  const auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
  setSpriteFrame(frame);
}
