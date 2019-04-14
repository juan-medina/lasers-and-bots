#include "game_object.h"

game_object* game_object::create(const std::string& sprite_frame_name, const std::string& type)
{
  game_object* ret = nullptr;

  do
  {
    auto object = new game_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(sprite_frame_name, type))
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
bool game_object::init(const std::string& sprite_frame_name, const std::string& type)
{
  auto ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first
    UTILS_BREAK_IF(!base_class::initWithSpriteFrameName(sprite_frame_name))

    type_ = type;

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
