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

Scene* menu_scene::scene(basic_app* application)
{
  menu_scene* ret = nullptr;

  do
  {
    auto object = new menu_scene();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(application))
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

menu_scene::menu_scene():
  main_menu_(nullptr)
{
}

menu_scene::~menu_scene()
{
  base_class::removeAllChildrenWithCleanup(true);
}

bool menu_scene::init(basic_app* application)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(application));

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-0.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-1.plist");

    const auto size = Director::getInstance()->getWinSize();

    const auto background = LayerGradient::create(Color4B(0, 0, 0, 255), Color4B(0, 0, 127, 255));
    UTILS_BREAK_IF(background == nullptr);

    addChild(background, 0);

    auto label = Label::createWithTTF("Laser and Bots", "fonts/tahoma.ttf", 500);
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

    main_menu_->display();

    ret = true;
  }
  while (false);

  return ret;
}

void menu_scene::to_game()
{
  auto app = dynamic_cast<laser_and_bots_app*>(get_application());

  const auto delay = DelayTime::create(1.15f);
  const auto func = CallFunc::create(CC_CALLBACK_0(laser_and_bots_app::to_game, app));
  const auto sequence = Sequence::create(delay, func, nullptr);

  runAction(sequence);
}

void menu_scene::exit()
{
  auto app = dynamic_cast<laser_and_bots_app*>(get_application());

  const auto delay = DelayTime::create(1.15f);
  const auto func = CallFunc::create(CC_CALLBACK_0(laser_and_bots_app::close, app));
  const auto sequence = Sequence::create(delay, func, nullptr);

  runAction(sequence);
}
