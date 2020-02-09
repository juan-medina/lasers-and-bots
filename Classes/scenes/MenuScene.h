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

#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "../utils/base/scene/BasicScene.h"

// forward declarations
class BasicMenu;

class MainMenu;

class OptionsMenu;

class PlayMenu;

class CreditsMenu;

class AboutMenu;

class LicenseMenu;

class InputController;

enum class MenuToDisplay
{
  MainMenu,
  PlayMenu,
  OptionsMenu,
  LicenseMenu
};

class MenuScene final : public BasicScene
{
public:
  using base_class = BasicScene;

  MenuScene();

  ~MenuScene();

  static Scene *scene(BasicApp *application, const MenuToDisplay menu,
                      const unsigned short int selectedLevel);

  void goToGame(const unsigned short int level);

  void exitApp();

  void displayOptionsMenu();

  void displayMainMenu();

  void displayPlayMenu();

  void displayCreditsMenu();

  void displayAboutMenu();

  void displayLicenseMenu();

  void changeMusic(const bool disabled) const;

  void changeSound(const bool disabled) const;

  void changeMusicVolume(const float volume) const;

  void changeSoundVolume(const float volume) const;

  void update(float delta) override;

  void pause() override;

  void resume() override;

  void didEnterBackground() override;

  void willEnterForeground() override;

  bool isFullScreen() const;

  void changeApplicationVideoMode(const bool fullScreen) const;

  bool isDebugGrid() const;

  void setDebugGrid(const bool debugGrid) const;

  bool isDebugPhysics() const;

  void setDebugPhysics(const bool debugPhysics) const;

private:
  void handleInput() const;

  void delayToGame() const;

  bool init(BasicApp *application, const MenuToDisplay menu, const unsigned short int selectedLevel);

  bool addBackground();

  bool addRobot();

  bool addLaser();

  BasicMenu *_currentMenu;
  MainMenu *_mainMenu;
  OptionsMenu *_optionsMenu;
  PlayMenu *_playMenu;
  CreditsMenu *_creditsMenu;
  AboutMenu *_aboutMenu;
  LicenseMenu *_licenseMenu;

  Node *_background;
  bool _paused;
  unsigned short int _savedLevel;

  InputController *_inputController;
};

#endif // __MENU_SCENE_H__
