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

#ifndef __LASERANDBOTSAPP_H__
#define __LASERANDBOTSAPP_H__

#include "utils/base/app/BasicApp.h"

// forward declarations
class LevelManager;
class InputController;

// this game application
class LaserAndBotsApp final : public BasicApp
{
public:
  using BaseClass = BasicApp;

  LaserAndBotsApp();

  ~LaserAndBotsApp() override;

  Scene* initScene() override;

  Scene* gameScene(unsigned short int level);
  Scene* mainMenuScene();
  Scene* licenseScene();
  Scene* playMenuScene(unsigned short int selectedLevel);
  Scene* optionsMenuScene();

  void setEffectsMuted(bool effectsMuted);
  void setMusicMuted(bool musicMuted);
  void setMusicVolume(float musicVolume);
  void setEffectsVolume(float effectsVolume);

  void toGame(unsigned short int level);
  void toMainMenu();
  void toPlayMenu(unsigned short int level);

  void applicationDidEnterBackground() override;

  LevelManager* getLevelManager() const { return _levelManager; }

  void changeVideoMode(bool fullScreen);

  bool wantARestart() const { return _wantRestart; }

  void closeWithRestart();

  int run(bool toOptions);

  bool isDebugGrid() const { return _debugGrid; }

  void setDebugGrid(bool debugGrid);

  bool isDebugPhysics() const { return _debugPhysics; }

  void setDebugPhysics(bool debugPhysics);

private:
  void setupLevelManager();

  bool _effectsMuted;
  bool _musicMuted;
  bool _debugGrid;
  bool _debugPhysics;

  float _musicVolume;
  float _effectsVolume;
  LevelManager* _levelManager;
  bool _wantRestart;
  bool _toOptions;
};

#endif //__LASERANDBOTSAPP_H__
