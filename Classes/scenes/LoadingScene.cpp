/****************************************************************************
 Copyright (c) 2019 Juan Medina

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

#include "LoadingScene.h"

#include "../utils/audio/AudioHelper.h"
#include "../utils/base/app/BasicApp.h"
#include "GameScene.h"
#include "MenuScene.h"

Scene *LoadingScene::game(BasicApp *application, const bool debugGrid, const bool debugPhysics,
                          const unsigned short int level)
{
  LoadingScene *ret = nullptr;

  do
  {
    auto object = new LoadingScene();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(application, LoadTo::ToGame, debugGrid, debugPhysics, MenuToDisplay::MainMenu, level))
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

Scene *LoadingScene::menu(BasicApp *application, const MenuToDisplay menu,
                          const unsigned short int selectedLevel)
{
  LoadingScene *ret = nullptr;

  do
  {
    auto object = new LoadingScene();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(application, LoadTo::ToMenu, false, false, menu, selectedLevel))
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

LoadingScene::LoadingScene()
  : _type(LoadTo::ToGame), _debugGrid(false), _debugPhysics(false), _menu(MenuToDisplay::MainMenu), _level(-1)
{
}

LoadingScene::~LoadingScene()
{
  base_class::removeAllChildrenWithCleanup(true);
}

bool LoadingScene::init(BasicApp *application, const LoadTo &type, const bool debug_grid,
                        const bool debug_physics, const MenuToDisplay menu, const unsigned short int level)
{
  auto ret = false;

  do
  {
    _type = type;
    _menu = menu;
    _level = level;
    _debugGrid = debug_grid;
    _debugPhysics = debug_physics;

    UTILS_BREAK_IF(!base_class::init(application));

    const auto size = Director::getInstance()->getWinSize();

    const auto background = LayerGradient::create(Color4B(0, 0, 0, 255), Color4B(0, 0, 127, 255));
    UTILS_BREAK_IF(background == nullptr);

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

    const auto func = CallFunc::create(CC_CALLBACK_0(LoadingScene::go_to_scene, this));
    UTILS_BREAK_IF(func == nullptr);

    const auto sequence = Sequence::create(delay_exit, func, NULL);
    UTILS_BREAK_IF(sequence == nullptr);

    runAction(sequence);

    getAudioHelper()->stopAllSounds();
    getAudioHelper()->unloadAllSounds();

    ret = true;
  } while (false);

  return ret;
}

void LoadingScene::go_to_scene() const
{
  do
  {
    Scene *scene = nullptr;

    switch (_type)
    {
      case LoadTo::ToGame:
        scene = GameScene::scene(_application, _debugGrid, _debugPhysics, _level);
        break;
      case LoadTo::ToMenu:
        scene = MenuScene::scene(_application, _menu, _level);
        break;
      default:
        break;
    }

    UTILS_BREAK_IF(scene == nullptr);
    Director::getInstance()->replaceScene(scene);
  } while (false);
}
