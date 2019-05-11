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

#include "menu_scene.h"
#include "../laser_and_bots_app.h"
#include "../menu/main_menu.h"
#include "../menu/options_menu.h"
#include "../menu/play_menu.h"
#include "../utils/audio/audio_helper.h"

menu_scene::menu_scene() :
  main_menu_(nullptr),
  options_menu_(nullptr),
  play_menu_(nullptr),
  background_(nullptr),
  paused_(false),
  saved_level_(-1)
{
}

Scene* menu_scene::scene(basic_app* application, menu_to_display menu)
{
  menu_scene* ret = nullptr;

  do
  {
    auto object = new menu_scene();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(application, menu))
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

menu_scene::~menu_scene()
{
  base_class::removeAllChildrenWithCleanup(true);
}

bool menu_scene::init(basic_app* application, menu_to_display menu)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(application));

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-0.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-1.plist");

    get_audio_helper()->pre_load_effect("sounds/select.mp3");
    get_audio_helper()->pre_load_effect("sounds/SlideClosed.mp3");
    get_audio_helper()->pre_load_music("sounds/menu.mp3");
    const auto& size = Director::getInstance()->getWinSize();

    UTILS_BREAK_IF(!add_background());
    UTILS_BREAK_IF(!add_robot());
    UTILS_BREAK_IF(!add_laser());

    auto label = Label::createWithTTF("Lasers and Bots", "fonts/tahoma.ttf", 500);
    UTILS_BREAK_IF(label == nullptr);

    label->setPosition(Vec2(size.width / 2, size.height - 500));
    label->setTextColor(Color4B(0, 255, 255, 255));

    label->enableGlow(Color4B(0, 127, 127, 127));
    label->enableShadow(Color4B(255, 255, 255, 127), Size(10, -10));
    label->enableOutline(Color4B(255, 255, 255, 255), 10);

    addChild(label, 0);

    main_menu_ = main_menu::create(get_audio_helper());
    UTILS_BREAK_IF(main_menu_ == nullptr);

    addChild(main_menu_, 0);

    options_menu_ = options_menu::create(get_audio_helper());
    UTILS_BREAK_IF(options_menu_ == nullptr);

    addChild(options_menu_, 0);

    play_menu_ = play_menu::create(get_audio_helper());
    UTILS_BREAK_IF(play_menu_ == nullptr);

    addChild(play_menu_, 0);

    switch (menu)
    {
    default:
    case menu_to_display::main_menu:
      display_main_menu();
      break;
    case menu_to_display::play_menu:
      display_play_menu();
      break;
    }

    get_audio_helper()->play_music("sounds/menu.mp3");

    scheduleUpdate();

    ret = true;
  }
  while (false);

  return ret;
}

bool menu_scene::add_background()
{
  auto result = false;

  do
  {
    background_ = experimental::TMXTiledMap::create("maps/intro.tmx");
    UTILS_BREAK_IF(background_ == nullptr);

    addChild(background_);

    result = true;
  }
  while (false);

  return result;
}

bool menu_scene::add_robot()
{
  auto ret = false;

  do
  {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot_0.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot_1.plist");

    const auto pattern = std::string("Run_%02d.png");
    const auto max_frame = 8;
    const auto loops = -1;
    const auto speed = 0.10f;

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

    auto robot = Sprite::createWithSpriteFrameName("Idle_01.png");
    UTILS_BREAK_IF(robot == nullptr);

    robot->runAction(Animate::create(anim));

    const auto& size = Director::getInstance()->getWinSize();
    robot->setPosition(Vec2(size.width / 2, -25.f));

    addChild(robot);

    ret = true;
  }
  while (false);

  return ret;
}

bool menu_scene::add_laser()
{
  auto result = false;

  do
  {
    const auto& size = Director::getInstance()->getWinSize();

    const auto draw_node = DrawNode::create();
    UTILS_BREAK_IF(draw_node == nullptr);

    const auto from = Vec2(-100, size.height / 2);
    const auto to = Vec2((size.width / 2) - 200, 12);

    draw_node->drawSegment(from, to, 8, Color4F::RED);
    draw_node->drawDot(to, 12, Color4F::RED);

    const auto blink = Sequence::create(FadeTo::create(0.5f, 64), FadeTo::create(0.5f, 185.f), nullptr);
    const auto repeat_blink = RepeatForever::create(blink);
    draw_node->runAction(repeat_blink);

    addChild(draw_node);

    auto spark_ = ParticleFire::create();
    spark_->setDuration(ParticleSystem::DURATION_INFINITY);
    spark_->setBlendAdditive(true);
    spark_->setColor(Color3B::RED);
    spark_->setOpacityModifyRGB(true);
    spark_->setOpacity(127);
    spark_->setTotalParticles(5000);
    spark_->setEmissionRate(200.f);
    spark_->setGravity(Vec2(-125.f, 250.f));
    spark_->setPosition(to);

    addChild(spark_);
    result = true;
  }
  while (false);

  return result;
}

void menu_scene::go_to_game(const int level)
{
  const auto delay = DelayTime::create(1.15f);
  const auto func = CallFunc::create(CC_CALLBACK_0(menu_scene::delay_to_game, this));
  const auto sequence = Sequence::create(delay, func, nullptr);
  saved_level_ = level;

  runAction(sequence);
}

void menu_scene::exit_app()
{
  auto app = dynamic_cast<laser_and_bots_app*>(get_application());

  const auto delay = DelayTime::create(1.15f);
  const auto func = CallFunc::create(CC_CALLBACK_0(laser_and_bots_app::close, app));
  const auto sequence = Sequence::create(delay, func, nullptr);

  runAction(sequence);
}

void menu_scene::display_options_menu() const
{
  options_menu_->display();
}

void menu_scene::display_main_menu() const
{
  main_menu_->display();
}

void menu_scene::display_play_menu() const
{
  play_menu_->display();
}

void menu_scene::change_music(const bool disabled) const
{
  const auto app = dynamic_cast<laser_and_bots_app*>(get_application());
  const auto helper = get_audio_helper();

  if (!disabled)
  {
    app->set_music_muted(disabled);
    helper->resume_music();
  }
  else
  {
    helper->pause_music();
    app->set_music_muted(disabled);
  }
}

void menu_scene::change_sound(const bool disabled) const
{
  const auto app = dynamic_cast<laser_and_bots_app*>(get_application());
  app->set_effects_muted(disabled);
}

void menu_scene::update(float delta)
{
  const auto pos = background_->getPosition();
  auto new_pos = pos - Vec2(delta * 600, 0);

  if (new_pos.x < (-256 * 2))
  {
    new_pos = Vec2::ZERO;
  }

  background_->setPosition(new_pos);
}

void menu_scene::pause()
{
  base_class::pause();
  if (!paused_)
  {
    const auto helper = get_audio_helper();
    if (helper != nullptr)
    {
      helper->pause_music();
    }
    paused_ = true;
  }
}

void menu_scene::resume()
{
  base_class::resume();
  if (paused_)
  {
    const auto helper = get_audio_helper();
    if (helper != nullptr)
    {
      helper->resume_music();
    }
    paused_ = false;
  }
}

void menu_scene::did_enter_background()
{
  pause();
}

void menu_scene::will_enter_foreground()
{
  resume();
}

void menu_scene::delay_to_game()
{
  auto app = dynamic_cast<laser_and_bots_app*>(get_application());
  app->to_game(saved_level_);
}
