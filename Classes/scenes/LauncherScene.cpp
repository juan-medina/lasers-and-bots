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

#include "LauncherScene.h"
#include "Launcher.h"

LauncherScene* LauncherScene::create()
{
  // create the game
  auto scene = new LauncherScene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init())
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }
  }

  // return the scene
  return scene;
}

Scene* LauncherScene::scene()
{
  Scene* result = nullptr;

  do
  {
    // 'layer' is an autorelease object
    auto layer = LauncherScene::create();
    UTILS_BREAK_IF(layer == nullptr);

    // 'scene' is an autorelease object
    auto scene = parent::createScene(layer);
    UTILS_BREAK_IF(scene == nullptr);

    result = scene;
  } while (0);

  // return the scene
  return result;
}

// on "init" you need to initialize your instance
bool LauncherScene::init()
{
  bool ret = false;

  do
  {
    ret = parent::init();
    UTILS_BREAK_IF(!ret);

    // init vars
    this->loadSettings();

    //////////////////////////////////////////////////////////////////////////
    // Menu Tittle
    //////////////////////////////////////////////////////////////////////////

    // create the text for the title
    auto titleFont = Label::createWithTTF("Lasers and Bots : Launcher", "fonts/Marker Felt.ttf", 40);
    UTILS_BREAK_IF(!titleFont);

    titleFont->setPosition(Vec2(_screenSize.width / 2, _screenSize.height - 50));
    this->addChild(titleFont);

    //////////////////////////////////////////////////////////////////////////
    // Menu Screen Mode
    //////////////////////////////////////////////////////////////////////////

    // create the text for the label
    _screenModeLabel = Label::createWithTTF("Screen Mode: Windowed", "fonts/Marker Felt.ttf", 20);
    UTILS_BREAK_IF(!_screenModeLabel);

    // create the label menu item
    auto screenModeItem = MenuItemLabel::create(_screenModeLabel, CC_CALLBACK_1(LauncherScene::screenModeClick, this));
    UTILS_BREAK_IF(!screenModeItem);

    // position menu item
    screenModeItem->setPosition(Vec2(_screenSize.width / 2, _screenSize.height - 100));

    //////////////////////////////////////////////////////////////////////////
    // Menu Resolution
    //////////////////////////////////////////////////////////////////////////

    // create the text for the label
    _resolutionLabel = Label::createWithTTF("Size : 9999 x 9999", "fonts/Marker Felt.ttf", 20);
    UTILS_BREAK_IF(!_resolutionLabel);

    // create the label menu item
    auto resolutionItem = MenuItemLabel::create(_resolutionLabel, CC_CALLBACK_1(LauncherScene::resolutionClick, this));
    UTILS_BREAK_IF(!resolutionItem);

    // position menu item
    resolutionItem->setPosition(Vec2(_screenSize.width / 2, _screenSize.height - 150));


    //////////////////////////////////////////////////////////////////////////
    // Effects mutted
    //////////////////////////////////////////////////////////////////////////

    // create the text for the label
    _effectsMutedLabel = Label::createWithTTF("Sfx Sound: true ", "fonts/Marker Felt.ttf", 20);
    UTILS_BREAK_IF(!_effectsMutedLabel);

    // create the label menu item
    auto effectsMutedItem = MenuItemLabel::create(_effectsMutedLabel, CC_CALLBACK_1(LauncherScene::effectsMutedClick, this));
    UTILS_BREAK_IF(!effectsMutedItem);

    // position menu item
    effectsMutedItem->setPosition(Vec2(_screenSize.width / 2, _screenSize.height - 200));

    //////////////////////////////////////////////////////////////////////////
    // Music mutted
    //////////////////////////////////////////////////////////////////////////

    // create the text for the label
    _musicMutedLabel = Label::createWithTTF("Music: true ", "fonts/Marker Felt.ttf", 20);
    UTILS_BREAK_IF(!_musicMutedLabel);

    // create the label menu item
    auto musicMutedItem = MenuItemLabel::create(_musicMutedLabel, CC_CALLBACK_1(LauncherScene::musicMutedClick, this));
    UTILS_BREAK_IF(!musicMutedItem);

    // position menu item
    musicMutedItem->setPosition(Vec2(_screenSize.width / 2, _screenSize.height - 250));

    //////////////////////////////////////////////////////////////////////////
    // Menu Play
    //////////////////////////////////////////////////////////////////////////

    // create the text for the label
    auto playLabel = Label::createWithTTF("PLAY!", "fonts/Marker Felt.ttf", 30);
    UTILS_BREAK_IF(!playLabel);

    // create the label menu item
    auto playItem = MenuItemLabel::create(playLabel, CC_CALLBACK_1(LauncherScene::play, this));
    UTILS_BREAK_IF(!playItem);

    // position menu item
    playItem->setPosition(Vec2(_screenSize.width / 2, _screenSize.height - 350));

    //////////////////////////////////////////////////////////////////////////
    // Menu
    //////////////////////////////////////////////////////////////////////////

    // Create a menu with the items
    auto OptionsMenu = Menu::create(screenModeItem, resolutionItem, effectsMutedItem, musicMutedItem, playItem, NULL);
    UTILS_BREAK_IF(!OptionsMenu);

    OptionsMenu->setPosition(Point::ZERO);

    this->updateLabels();

    // Add the menu to Menu layer as a child layer.
    this->addChild(OptionsMenu, 1);

    ret = true;

  } while (0);

  return ret;
}

void LauncherScene::loadSettings()
{
  _fullScreen = UserDefault::getInstance()->getBoolForKey("fullScreen", false);
  _screenWidth = UserDefault::getInstance()->getIntegerForKey("screenWidth", 1920);
  _screenHeight = UserDefault::getInstance()->getIntegerForKey("screenHeight", 1080);
  _effectsMuted = UserDefault::getInstance()->getBoolForKey("effectsMuted", false);
  _musicMuted = UserDefault::getInstance()->getBoolForKey("musicMuted", false);
}

void LauncherScene::saveSettings()
{
  UserDefault::getInstance()->setBoolForKey("fullScreen", _fullScreen);
  UserDefault::getInstance()->setIntegerForKey("screenWidth", _screenWidth);
  UserDefault::getInstance()->setIntegerForKey("screenHeight", _screenHeight);
  UserDefault::getInstance()->setBoolForKey("effectsMuted", _effectsMuted);
  UserDefault::getInstance()->setBoolForKey("musicMuted", _musicMuted);
  UserDefault::getInstance()->setBoolForKey("fitAll", false);
}

void LauncherScene::play(Ref* sender)
{
  Launcher* app = (Launcher*)Application::getInstance();
  app->setWantToPlay(true);

  this->saveSettings();

  Director::getInstance()->end();
}

void LauncherScene::screenModeClick(Ref* sender)
{
  _fullScreen = !_fullScreen;
  this->updateLabels();
}

void LauncherScene::resolutionClick(Ref* sender)
{
  if (!_fullScreen)
  {
    if ((_screenWidth == 1920) && (_screenHeight == 1080))
    {
      _screenWidth = 1334;
      _screenHeight = 750;
    }
    else if ((_screenWidth == 1334) && (_screenHeight == 750))
    {
      _screenWidth = 1136;
      _screenHeight = 640;
    }
    else if ((_screenWidth == 1136) && (_screenHeight == 640))
    {
      _screenWidth = 1024;
      _screenHeight = 768;
    }
    else if ((_screenWidth == 1024) && (_screenHeight == 768))
    {
      _screenWidth = 960;
      _screenHeight = 640;
    }
    else if ((_screenWidth == 960) && (_screenHeight == 640))
    {
      _screenWidth = 2048;
      _screenHeight = 1536;
    }
    else
    {
      _screenWidth = 1920;
      _screenHeight = 1080;
    }
    this->updateLabels();
  }
}


void LauncherScene::effectsMutedClick(Ref* sender)
{
  _effectsMuted = !_effectsMuted;
  this->updateLabels();
}

void LauncherScene::musicMutedClick(Ref* sender)
{
  _musicMuted = !_musicMuted;
  this->updateLabels();
}

void LauncherScene::updateLabels()
{

  if (_fullScreen)
  {
    _screenModeLabel->setString("Screen Mode: Full Screen");
    _resolutionLabel->setVisible(false);
  }
  else
  {
    _screenModeLabel->setString("Screen Mode: Windowed");
    _resolutionLabel->setVisible(true);
  }

  _resolutionLabel->setString(string_format("Size : %d x %d", _screenWidth, _screenHeight));


  _effectsMutedLabel->setString(string_format("Sfx Sound : %s", _effectsMuted ? "false" : "true"));

  _musicMutedLabel->setString(string_format("Music : %s", _musicMuted ? "false" : "true"));
}
