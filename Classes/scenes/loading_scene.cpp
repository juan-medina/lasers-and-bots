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

#include "loading_scene.h"
#include "game_scene.h"
#include "menu_scene.h"
#include "../utils/base/app/basic_app.h"
#include "../utils/audio/audio_helper.h"

Scene* loading_scene::game(basic_app* application, const bool debug_grid, const bool debug_physics, const int level)
{
  loading_scene* ret = nullptr;

  do
  {
    auto object = new loading_scene();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(application, load_to::to_game, debug_grid, debug_physics, menu_to_display::main_menu, level))
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

Scene* loading_scene::menu(basic_app* application, menu_to_display menu)
{
  loading_scene* ret = nullptr;

  do
  {
    auto object = new loading_scene();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(application, load_to::to_menu, false, false, menu, -1))
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

loading_scene::loading_scene():
  type_(load_to::to_game),
  debug_grid_(false),
  debug_physics_(false),
  menu_(menu_to_display::main_menu),
  level_(-1)
{
}

loading_scene::~loading_scene()
{
  base_class::removeAllChildrenWithCleanup(true);
}

bool loading_scene::init(basic_app* application, const load_to& type, const bool debug_grid, const bool debug_physics,
                         const menu_to_display menu, const int level)
{
  auto ret = false;

  do
  {
    type_ = type;
    menu_ = menu;
    level_ = level;

    UTILS_BREAK_IF(!base_class::init(application));

    const auto size = Director::getInstance()->getWinSize();

    const auto background = LayerGradient::create(Color4B(0, 0, 0, 255), Color4B(0, 0, 127, 255));
    UTILS_BREAK_IF(background==nullptr);

    addChild(background, 0);

    auto label = Label::createWithTTF("Loading...", "fonts/tahoma.ttf", 500);
    UTILS_BREAK_IF(label == nullptr);

    label->setPosition(Vec2(size.width / 2, size.height / 2));
    label->setTextColor(Color4B(0, 255, 255, 255));

    label->enableGlow(Color4B(0, 127, 127, 127));
    label->enableShadow(Color4B(255, 255, 255, 127), Size(10, -10));
    label->enableOutline(Color4B(255, 255, 255, 255), 10);

    addChild(label, 0);

    const auto delay_exit = DelayTime::create(0.15f);
    UTILS_BREAK_IF(delay_exit == nullptr);

    const auto func = CallFunc::create(CC_CALLBACK_0(loading_scene::go_to_scene, this));
    UTILS_BREAK_IF(func == nullptr);

    const auto sequence = Sequence::create(delay_exit, func, NULL);
    UTILS_BREAK_IF(sequence == nullptr);

    runAction(sequence);

    debug_grid_ = debug_grid;
    debug_physics_ = debug_physics;

    get_audio_helper()->stop_all_sounds();
    get_audio_helper()->unload_all_sounds();

    ret = true;
  }
  while (false);

  return ret;
}


void loading_scene::go_to_scene() const
{
  do
  {
    Scene* scene = nullptr;

    switch (type_)
    {
    case load_to::to_game:
      scene = game_scene::scene(application_, debug_grid_, debug_physics_, level_);
      break;
    case load_to::to_menu:
      scene = menu_scene::scene(application_, menu_);
      break;
    default:
      break;
    }

    UTILS_BREAK_IF(scene==nullptr);
    Director::getInstance()->replaceScene(scene);
  }
  while (false);
}
