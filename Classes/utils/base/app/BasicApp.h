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

#ifndef __BASIC_APP_H__
#define __BASIC_APP_H__

#include "../../utils.h"

// forward declarations
class AudioHelper;
class InputController;

// application base class
class BasicApp : public Application
{
public:
  using BaseClass = Application;

  explicit BasicApp(const std::string& applicationName, const float designWidth, const float designHeight);

  void initGLContextAttrs() override;

  // this function will be called when the app is ready to initialize
  bool applicationDidFinishLaunching() override;

  // this function will be called when the app is active again
  void applicationDidEnterBackground() override;

  // This function will be called when the app is inactive. When comes a phone
  // call,it's be invoked too
  void applicationWillEnterForeground() override;

  virtual Scene* initScene() = 0;

  virtual const string& applicationName() const { return _applicationName; };

  void close();

  AudioHelper* getAudioHelper() const { return _audioHelper; }
#if (GAME_PLATFORM == DESKTOP_GAME)
  void setWindowSize(const float scale);
#endif
  void setFullScreen(const bool fullScreen) { _fullScreen = fullScreen; }

  bool isFullScreen() const { return _fullScreen; }

  void setFitAll(const bool fitAll) { _fitAll = fitAll; }

  bool isDesktop();

  std::string getGameVersionString() const;

  InputController* getInputController() const { return _inputController; }

private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
  // center on screen in windows 32 client
  static void centerWin32Window();
#endif

  bool readVersion();
  static std::string getPlatformName(const Platform platform);

  AudioHelper* _audioHelper;
  InputController* _inputController;

  float _designWidth;
  float _designHeight;
  int _windowWidth;
  int _windowHeight;
  bool _fullScreen;
  bool _fitAll;
  string _applicationName;

  struct GameVersion
  {
    unsigned short int major;
    unsigned short int minor;
    unsigned short int patch;
    unsigned int build;
    Platform platform;
  };

  GameVersion _gameVersion;
};

#endif // __BASIC_APP_H__
