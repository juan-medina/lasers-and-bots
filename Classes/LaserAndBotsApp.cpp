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

#include "LaserAndBotsApp.h"

#include "misc/LevelManager.h"
#include "scenes/LoadingScene.h"
#include "scenes/MenuScene.h"
#include "utils/audio/AudioHelper.h"

LaserAndBotsApp::LaserAndBotsApp()
  : BaseClass("Lasers and Bots", 1920 * 4, 1080 * 4)
  , _effectsMuted(false)
  , _musicMuted(false)
  , _debugGrid(false)
  , _debugPhysics(false)
  , _musicVolume(1.f)
  , _effectsVolume(1.f)
  , _levelManager(nullptr)
  , _wantRestart(false)
  , _toOptions(false)
{
#if (GAME_PLATFORM == DESKTOP_GAME)
  if (isDesktop())
  {
    const auto fullScreen = UserDefault::getInstance()->getBoolForKey("full_screen", false);
    setFullScreen(fullScreen);

    if (!fullScreen)
    {
      const auto windowSize = UserDefault::getInstance()->getFloatForKey("window_size", 0.75f);
      setWindowSize(windowSize);
    }
  }
#endif
}

LaserAndBotsApp::~LaserAndBotsApp()
{
  delete _levelManager;
}

Scene *LaserAndBotsApp::initScene()
{
  return _toOptions ? optionsMenuScene() : licenseScene();
}

Scene *LaserAndBotsApp::gameScene(const unsigned short int level)
{
  _effectsMuted = UserDefault::getInstance()->getBoolForKey("effects_muted", _effectsMuted);
  _musicMuted = UserDefault::getInstance()->getBoolForKey("music_muted", _musicMuted);
  _debugGrid = UserDefault::getInstance()->getBoolForKey("debug_grid", _debugGrid);
  _debugPhysics = UserDefault::getInstance()->getBoolForKey("debug_physics", _debugPhysics);
  _musicVolume = UserDefault::getInstance()->getFloatForKey("music_volume", _musicVolume);
  _effectsVolume = UserDefault::getInstance()->getFloatForKey("effects_volume", _effectsVolume);

  getAudioHelper()->setEffectsMuted(_effectsMuted);
  getAudioHelper()->setMusicMuted(_musicMuted);
  getAudioHelper()->setMusicVolume(_musicVolume);
  getAudioHelper()->setSoundVolume(_effectsVolume);
  setupLevelManager();

  return LoadingScene::game(this, _debugGrid, _debugPhysics, level);
}

Scene *LaserAndBotsApp::mainMenuScene()
{
  _effectsMuted = UserDefault::getInstance()->getBoolForKey("effects_muted", _effectsMuted);
  _musicMuted = UserDefault::getInstance()->getBoolForKey("music_muted", _musicMuted);
  _musicVolume = UserDefault::getInstance()->getFloatForKey("music_volume", _musicVolume);
  _effectsVolume = UserDefault::getInstance()->getFloatForKey("effects_volume", _effectsVolume);

  getAudioHelper()->setEffectsMuted(_effectsMuted);
  getAudioHelper()->setMusicMuted(_musicMuted);
  getAudioHelper()->setMusicVolume(_musicVolume);
  getAudioHelper()->setSoundVolume(_effectsVolume);
  setupLevelManager();

  return LoadingScene::menu(this, MenuToDisplay::MainMenu, 1);
}

Scene *LaserAndBotsApp::licenseScene()
{
  _effectsMuted = UserDefault::getInstance()->getBoolForKey("effects_muted", _effectsMuted);
  _musicMuted = UserDefault::getInstance()->getBoolForKey("music_muted", _musicMuted);
  _musicVolume = UserDefault::getInstance()->getFloatForKey("music_volume", _musicVolume);
  _effectsVolume = UserDefault::getInstance()->getFloatForKey("effects_volume", _effectsVolume);

  getAudioHelper()->setEffectsMuted(_effectsMuted);
  getAudioHelper()->setMusicMuted(_musicMuted);
  getAudioHelper()->setMusicVolume(_musicVolume);
  getAudioHelper()->setSoundVolume(_effectsVolume);
  setupLevelManager();

  return LoadingScene::menu(this, MenuToDisplay::LicenseMenu, 1);
}
Scene *LaserAndBotsApp::playMenuScene(const unsigned short int selectedLevel)
{
  _effectsMuted = UserDefault::getInstance()->getBoolForKey("effects_muted", _effectsMuted);
  _musicMuted = UserDefault::getInstance()->getBoolForKey("music_muted", _musicMuted);
  _musicVolume = UserDefault::getInstance()->getFloatForKey("music_volume", _musicVolume);
  _effectsVolume = UserDefault::getInstance()->getFloatForKey("effects_volume", _effectsVolume);

  setupLevelManager();

  getAudioHelper()->setEffectsMuted(_effectsMuted);
  getAudioHelper()->setMusicMuted(_musicMuted);
  getAudioHelper()->setMusicVolume(_musicVolume);
  getAudioHelper()->setSoundVolume(_effectsVolume);

  return LoadingScene::menu(this, MenuToDisplay::PlayMenu, selectedLevel);
}

Scene *LaserAndBotsApp::optionsMenuScene()
{
  _effectsMuted = UserDefault::getInstance()->getBoolForKey("effects_muted", _effectsMuted);
  _musicMuted = UserDefault::getInstance()->getBoolForKey("music_muted", _musicMuted);
  _musicVolume = UserDefault::getInstance()->getFloatForKey("music_volume", _musicVolume);
  _effectsVolume = UserDefault::getInstance()->getFloatForKey("effects_volume", _effectsVolume);

  setupLevelManager();

  getAudioHelper()->setEffectsMuted(_effectsMuted);
  getAudioHelper()->setMusicMuted(_musicMuted);
  getAudioHelper()->setMusicVolume(_musicVolume);
  getAudioHelper()->setSoundVolume(_effectsVolume);

  return LoadingScene::menu(this, MenuToDisplay::OptionsMenu, 1);
}

void LaserAndBotsApp::setEffectsMuted(const bool effectsMuted)
{
  _effectsMuted = effectsMuted;
  UserDefault::getInstance()->setBoolForKey("effects_muted", _effectsMuted);
  getAudioHelper()->setEffectsMuted(_effectsMuted);
}

void LaserAndBotsApp::setMusicMuted(const bool musicMuted)
{
  _musicMuted = musicMuted;
  UserDefault::getInstance()->setBoolForKey("music_muted", _musicMuted);
  getAudioHelper()->setMusicMuted(_musicMuted);
}

void LaserAndBotsApp::setMusicVolume(const float musicVolume)
{
  _musicVolume = musicVolume;
  UserDefault::getInstance()->setFloatForKey("music_volume", _musicVolume);
  getAudioHelper()->setMusicVolume(_musicVolume);
}

void LaserAndBotsApp::setEffectsVolume(const float effectsVolume)
{
  _effectsVolume = effectsVolume;
  UserDefault::getInstance()->setFloatForKey("effects_volume", _effectsVolume);
  getAudioHelper()->setSoundVolume(_effectsVolume);
}

void LaserAndBotsApp::toGame(const unsigned short int level)
{
  Director::getInstance()->replaceScene(gameScene(level));
}

void LaserAndBotsApp::toMainMenu()
{
  Director::getInstance()->replaceScene(mainMenuScene());
}

void LaserAndBotsApp::toPlayMenu(const unsigned short int level)
{
  Director::getInstance()->replaceScene(playMenuScene(level));
}

void LaserAndBotsApp::applicationDidEnterBackground()
{
  const auto scene = Director::getInstance()->getRunningScene();
  scene->pause();
}

void LaserAndBotsApp::changeVideoMode(const bool fullScreen)
{
  UserDefault::getInstance()->setBoolForKey("full_screen", fullScreen);
  closeWithRestart();
}

void LaserAndBotsApp::closeWithRestart()
{
  _wantRestart = true;
  close();
}

int LaserAndBotsApp::run(const bool toOptions)
{
  _toOptions = toOptions;
  return BaseClass::run();
}

void LaserAndBotsApp::setDebugGrid(const bool debugGrid)
{
  _debugGrid = debugGrid;
  UserDefault::getInstance()->setBoolForKey("debug_grid", debugGrid);
}

void LaserAndBotsApp::setDebugPhysics(const bool debugPhysics)
{
  _debugPhysics = debugPhysics;
  UserDefault::getInstance()->setBoolForKey("debug_physics", debugPhysics);
}

void LaserAndBotsApp::setupLevelManager()
{
  if (_levelManager == nullptr)
  {
    _levelManager = new LevelManager();
  }

  _levelManager->init();
}
