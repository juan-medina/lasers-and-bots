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

#include "MenuScene.h"

#include "../laser_and_bots_app.h"
#include "../menu/AboutMenu.h"
#include "../menu/CreditsMenu.h"
#include "../menu/LicenseMenu.h"
#include "../menu/MainMenu.h"
#include "../menu/OptionsMenu.h"
#include "../menu/PlayMenu.h"
#include "../utils/audio/AudioHelper.h"
#include "../utils/controller/InputController.h"

MenuScene::MenuScene()
  : _currentMenu(nullptr)
  , _mainMenu(nullptr)
  , _optionsMenu(nullptr)
  , _playMenu(nullptr)
  , _creditsMenu(nullptr)
  , _aboutMenu(nullptr)
  , _licenseMenu(nullptr)
  , _background(nullptr)
  , _paused(false)
  , _savedLevel(-1)
  , _inputController(nullptr)
{
}

Scene *MenuScene::scene(BasicApp *application, const MenuToDisplay menu,
                        const unsigned short int selectedLevel)
{
  MenuScene *ret = nullptr;

  do
  {
    auto object = new MenuScene();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(application, menu, selectedLevel))
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

MenuScene::~MenuScene()
{
  BaseClass::removeAllChildrenWithCleanup(true);
}

bool MenuScene::init(BasicApp *application, const MenuToDisplay menu, const unsigned short int selectedLevel)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init(application));

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-0.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-1.plist");

    getAudioHelper()->preLoadEffect("sounds/select.mp3");
    getAudioHelper()->preLoadEffect("sounds/SlideClosed.mp3");
    getAudioHelper()->preLoadMusic("sounds/Cellar-I.mp3", "sounds/Cellar-L.mp3");
    const auto &size = Director::getInstance()->getWinSize();

    UTILS_BREAK_IF(!addBackground());
    UTILS_BREAK_IF(!addRobot());
    UTILS_BREAK_IF(!addLaser());

    auto label = Label::createWithTTF("Lasers and Bots", "fonts/tahoma.ttf", 500);
    UTILS_BREAK_IF(label == nullptr);

    label->setPosition(Vec2(size.width / 2, size.height - 300));
    label->setTextColor(Color4B(0, 255, 255, 255));

    label->enableGlow(Color4B(0, 127, 127, 127));
    label->enableShadow(Color4B(255, 255, 255, 127), Size(10, -10));
    label->enableOutline(Color4B(255, 255, 255, 255), 10);

    addChild(label, 0);

    const auto version = _application->getGameVersionString();

    auto versionLabel = Label::createWithTTF(version, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(versionLabel == nullptr);

    versionLabel->setHorizontalAlignment(TextHAlignment::RIGHT);
    versionLabel->setAnchorPoint(Vec2(1, 0));
    versionLabel->setPosition(Vec2(size.width - 60, 60));
    versionLabel->setTextColor(Color4B(255, 255, 255, 255));
    versionLabel->enableOutline(Color4B(0, 0, 0, 255), 5);

    addChild(versionLabel, 0);

    _mainMenu = MainMenu::create(getAudioHelper());
    UTILS_BREAK_IF(_mainMenu == nullptr);

    addChild(_mainMenu, 0);

    _optionsMenu = OptionsMenu::create(getAudioHelper(), application->isDesktop());
    UTILS_BREAK_IF(_optionsMenu == nullptr);

    addChild(_optionsMenu, 0);

    _playMenu = PlayMenu::create(getAudioHelper(), selectedLevel);
    UTILS_BREAK_IF(_playMenu == nullptr);

    addChild(_playMenu, 0);

    _creditsMenu = CreditsMenu::create(getAudioHelper());
    UTILS_BREAK_IF(_creditsMenu == nullptr);

    addChild(_creditsMenu, 0);

    _aboutMenu = AboutMenu::create(getAudioHelper());
    UTILS_BREAK_IF(_aboutMenu == nullptr);

    addChild(_aboutMenu, 0);

    _licenseMenu = LicenseMenu::create(getAudioHelper());
    UTILS_BREAK_IF(_licenseMenu == nullptr);

    addChild(_licenseMenu, 0);

    switch (menu)
    {
      case MenuToDisplay::MainMenu:
        displayMainMenu();
        break;
      case MenuToDisplay::PlayMenu:
        displayPlayMenu();
        break;
      case MenuToDisplay::OptionsMenu:
        displayOptionsMenu();
        break;
      case MenuToDisplay::LicenseMenu:
        displayLicenseMenu();
        break;
    }

    getAudioHelper()->playMusic("sounds/Cellar-I.mp3", "sounds/Cellar-L.mp3");

    scheduleUpdate();

    ret = true;
  } while (false);

  return ret;
}

bool MenuScene::addBackground()
{
  auto result = false;

  do
  {
    _background = experimental::TMXTiledMap::create("maps/intro.tmx");
    UTILS_BREAK_IF(_background == nullptr);

    addChild(_background);

    result = true;
  } while (false);

  return result;
}

bool MenuScene::addRobot()
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
      const auto frame = cache->getSpriteFrameByName(StringFormat(pattern, num));
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

    const auto &size = Director::getInstance()->getWinSize();
    robot->setPosition(Vec2(size.width / 2, -25.f));

    addChild(robot);

    ret = true;
  } while (false);

  return ret;
}

bool MenuScene::addLaser()
{
  auto result = false;

  do
  {
    const auto &size = Director::getInstance()->getWinSize();

    const auto drawNode = DrawNode::create();
    UTILS_BREAK_IF(drawNode == nullptr);

    const auto from = Vec2(-100, size.height / 2);
    const auto to = Vec2((size.width / 2) - 200, 12);

    drawNode->drawSegment(from, to, 8, Color4F::RED);
    drawNode->drawDot(to, 12, Color4F::RED);

    const auto blink = Sequence::create(FadeTo::create(0.5f, 64), FadeTo::create(0.5f, 185.f), nullptr);
    const auto repeatBlink = RepeatForever::create(blink);
    drawNode->runAction(repeatBlink);

    addChild(drawNode);

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
  } while (false);

  return result;
}

void MenuScene::goToGame(const unsigned short int level)
{
  const auto delay = DelayTime::create(1.15f);
  const auto func = CallFunc::create(CC_CALLBACK_0(MenuScene::delayToGame, this));
  const auto sequence = Sequence::create(delay, func, nullptr);
  _savedLevel = level;

  runAction(sequence);
}

void MenuScene::exitApp()
{
  auto app = dynamic_cast<laser_and_bots_app *>(getApplication());

  const auto delay = DelayTime::create(1.15f);
  const auto func = CallFunc::create(CC_CALLBACK_0(laser_and_bots_app::close, app));
  const auto sequence = Sequence::create(delay, func, nullptr);

  runAction(sequence);
}

void MenuScene::displayOptionsMenu()
{
  _currentMenu = _optionsMenu;
  _optionsMenu->display();
}

void MenuScene::displayMainMenu()
{
  _currentMenu = _mainMenu;
  _mainMenu->display();
}

void MenuScene::displayPlayMenu()
{
  _currentMenu = _playMenu;
  _playMenu->display();
}

void MenuScene::displayCreditsMenu()
{
  _currentMenu = _creditsMenu;
  _creditsMenu->display();
}

void MenuScene::displayAboutMenu()
{
  _currentMenu = _aboutMenu;
  _aboutMenu->display();
}

void MenuScene::displayLicenseMenu()
{
  _currentMenu = _licenseMenu;
  _licenseMenu->display();
}

void MenuScene::changeMusic(const bool disabled) const
{
  const auto app = dynamic_cast<laser_and_bots_app *>(getApplication());
  const auto helper = getAudioHelper();

  if (!disabled)
  {
    app->set_music_muted(disabled);
    helper->resumeMusic();
  }
  else
  {
    helper->pauseMusic();
    app->set_music_muted(disabled);
  }
}

void MenuScene::changeSound(const bool disabled) const
{
  const auto app = dynamic_cast<laser_and_bots_app *>(getApplication());
  app->set_effects_muted(disabled);
}

void MenuScene::changeMusicVolume(const float volume) const
{
  const auto app = dynamic_cast<laser_and_bots_app *>(getApplication());
  app->set_music_volume(volume);
}

void MenuScene::changeSoundVolume(const float volume) const
{
  const auto app = dynamic_cast<laser_and_bots_app *>(getApplication());
  app->set_effects_volume(volume);
}

void MenuScene::update(float delta)
{
  handleInput();

  const auto pos = _background->getPosition();
  auto newPos = pos - Vec2(delta * 600, 0);

  if (newPos.x < (-256 * 2))
  {
    newPos = Vec2::ZERO;
  }

  _background->setPosition(newPos);
}

void MenuScene::pause()
{
  BaseClass::pause();
  if (!_paused)
  {
    const auto helper = getAudioHelper();
    if (helper != nullptr)
    {
      helper->pauseMusic();
    }
    _paused = true;
  }
}

void MenuScene::resume()
{
  BaseClass::resume();
  if (_paused)
  {
    const auto helper = getAudioHelper();
    if (helper != nullptr)
    {
      helper->resumeMusic();
    }
    _paused = false;
  }
}

void MenuScene::didEnterBackground()
{
  pause();
}

void MenuScene::willEnterForeground()
{
  resume();
}

bool MenuScene::isFullScreen() const
{
  return getApplication()->isFullScreen();
}

void MenuScene::changeApplicationVideoMode(const bool fullScreen) const
{
  auto app = dynamic_cast<laser_and_bots_app *>(getApplication());
  app->change_video_mode(fullScreen);
}

bool MenuScene::isDebugGrid() const
{
  const auto app = dynamic_cast<laser_and_bots_app *>(getApplication());
  return app->is_debug_grid();
}

void MenuScene::setDebugGrid(const bool debugGrid) const
{
  const auto app = dynamic_cast<laser_and_bots_app *>(getApplication());
  app->set_debug_grid(debugGrid);
}

bool MenuScene::isDebugPhysics() const
{
  const auto app = dynamic_cast<laser_and_bots_app *>(getApplication());
  return app->is_debug_physics();
}

void MenuScene::setDebugPhysics(const bool debugPhysics) const
{
  const auto app = dynamic_cast<laser_and_bots_app *>(getApplication());
  app->set_debug_physics(debugPhysics);
}

void MenuScene::handleInput() const
{
  const auto controller = getInputController();

  if (controller != nullptr)
  {
    if (controller->singlePressUp())
    {
      _currentMenu->moveSelectionUp();
    }
    if (controller->singlePressDown())
    {
      _currentMenu->moveSelectionDown();
    }
    if (controller->singlePressLeft())
    {
      _currentMenu->moveSelectionLeft();
    }
    if (controller->singlePressRight())
    {
      _currentMenu->moveSelectionRight();
    }
    if (controller->singlePressButtonA() || controller->singlePressButtonStart())
    {
      _currentMenu->activateSelection();
    }
    if (controller->singlePressButtonB() || controller->singlePressButtonBack())
    {
      _currentMenu->selectionBack();
    }
  }
}

void MenuScene::delayToGame() const
{
  auto app = dynamic_cast<laser_and_bots_app *>(getApplication());
  app->to_game(_savedLevel);
}
