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

#include "BasicApp.h"
#include "../../audio/AudioHelper.h"
#include "../../controller/input_controller.h"
#include "../scene/basic_scene.h"

BasicApp::BasicApp(const std::string& applicationName, const float designWidth, const float designHeight)
  : _audioHelper(nullptr)
  , _inputController(nullptr)
  , _designWidth(designWidth)
  , _designHeight(designHeight)
  , _windowWidth(0)
  , _windowHeight(0)
  , _fullScreen(true)
  , _fitAll(false)
  , _applicationName(applicationName)
  , _gameVersion{0, 0, 0, 0, Platform::OS_LINUX}
{
}

void BasicApp::initGLContextAttrs()
{
  // set OpenGL context attributions,now can only set six attributions:
  // red, green, blue, alpha, depth, stencil, multi sampling
  GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

  GLView::setGLContextAttrs(glContextAttrs);
}

static int RegisterAllPackages()
{
  return 0; // flag for packages manager
}

bool BasicApp::applicationDidFinishLaunching()
{
  auto ret = false;

  do
  {
    auto director = Director::getInstance();
    auto openGlView = director->getOpenGLView();

    if (!openGlView)
    {
#if (GAME_PLATFORM == DESKTOP_GAME)
      if (_fullScreen)
      {
        openGlView = GLViewImpl::createWithFullScreen(_applicationName);
      }
      else
      {
        openGlView =
          GLViewImpl::createWithRect(_applicationName, Rect(Vec2::ZERO, Size(_windowWidth, _windowHeight)));
      }
#else
      open_gl_view = GLViewImpl::create(application_name_);
#endif
      director->setOpenGLView(openGlView);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (!_fullScreen)
    {
      centerWin32Window();
    }
#endif

    director->setAnimationInterval(1.0f / 60.0f);

    // get the real screen scale
    const auto realScreen = openGlView->getVisibleSize();

    // Set the design resolution, we scale to our design with so aspect ratio is
    // maintained
    if (_fitAll)
    {
      openGlView->setDesignResolutionSize(_designWidth, _designHeight, ResolutionPolicy::SHOW_ALL);
    }
    else
    {
      const auto ratioX = realScreen.width / _designWidth;
      const auto ratioY = realScreen.height / _designHeight;

      if (ratioX >= ratioY)
      {
        openGlView->setDesignResolutionSize(_designWidth, _designHeight, ResolutionPolicy::FIXED_WIDTH);
      }
      else
      {
        openGlView->setDesignResolutionSize(_designWidth, _designHeight, ResolutionPolicy::FIXED_HEIGHT);
      }
    }

    RegisterAllPackages();

    _audioHelper = new AudioHelper();
    UTILS_BREAK_IF(_audioHelper == nullptr);

    _inputController = input_controller::create();
    UTILS_BREAK_IF(_inputController == nullptr);

    const auto scene = initScene();
    UTILS_BREAK_IF(scene == nullptr);

    UTILS_BREAK_IF(!readVersion());

    director->runWithScene(scene);

    ret = true;
  } while (false);

  return ret;
}

void BasicApp::applicationDidEnterBackground()
{
  const auto director = Director::getInstance();
  director->stopAnimation();

  const auto scene = dynamic_cast<basic_scene*>(director->getRunningScene());
  if (scene != nullptr)
  {
    scene->did_enter_background();
  }
}

void BasicApp::applicationWillEnterForeground()
{
  const auto director = Director::getInstance();
  director->startAnimation();

  const auto scene = dynamic_cast<basic_scene*>(director->getRunningScene());
  if (scene != nullptr)
  {
    scene->will_enter_foreground();
  }
}

void BasicApp::close()
{
  delete _audioHelper;
  _audioHelper = nullptr;

  delete _inputController;
  _inputController = nullptr;

  Director::getInstance()->end();
}

#if (GAME_PLATFORM == DESKTOP_GAME)
void BasicApp::setWindowSize(const float scale)
{
  glfwInit();
  const auto monitor = glfwGetPrimaryMonitor();
  const auto videoMode = glfwGetVideoMode(monitor);
  _windowWidth = static_cast<int>(videoMode->width * scale);
  _windowHeight = static_cast<int>(videoMode->height * scale);
  glfwTerminate();
}
#endif

bool BasicApp::isDesktop()
{
  const auto platform = getTargetPlatform();
  return (platform == Platform::OS_WINDOWS) || (platform == Platform::OS_MAC);
}

std::string BasicApp::getGameVersionString() const
{
  return StringFormat("Version: %d.%d.%d build %d (%s)", _gameVersion.major, _gameVersion.minor,
                      _gameVersion.patch, _gameVersion.build, getPlatformName(_gameVersion.platform).c_str());
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void BasicApp::centerWin32Window()
{
  // get the window handler
  const auto win32Window = Director::getInstance()->getOpenGLView()->getWin32Window();

  if (!win32Window)
  {
    return;
  }

  // get the desktop dimensions
  RECT rcDesktop, rcWindow;
  GetWindowRect(GetDesktopWindow(), &rcDesktop);

  // subtract the task bar
  const auto hTaskBar = FindWindow(TEXT("Shell_TrayWnd"), nullptr);
  if (hTaskBar != nullptr)
  {
    APPBARDATA abd;

    abd.cbSize = sizeof(APPBARDATA);
    abd.hWnd = hTaskBar;

    SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
    SubtractRect(&rcDesktop, &rcDesktop, &abd.rc);
  }

  // get our window dimensions
  GetWindowRect(win32Window, &rcWindow);

  // calculate center position
  int offsetX = rcDesktop.left + (rcDesktop.right - rcDesktop.left - (rcWindow.right - rcWindow.left)) / 2;
  offsetX = (offsetX > 0) ? offsetX : rcDesktop.left;
  int offsetY = rcDesktop.top + (rcDesktop.bottom - rcDesktop.top - (rcWindow.bottom - rcWindow.top)) / 2;
  offsetY = (offsetY > 0) ? offsetY : rcDesktop.top;

  // move the window
  SetWindowPos(win32Window, nullptr, offsetX, offsetY, 0, 0,
               SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}
#endif

bool BasicApp::readVersion()
{
  auto result = false;
  do
  {
    const auto versionData = FileUtils::getInstance()->getValueMapFromFile("version/version.plist");
    UTILS_BREAK_IF(versionData.empty());

    const auto version = versionData.at("version").asValueMap();
    UTILS_BREAK_IF(version.empty());

    _gameVersion.major = version.at("major").asInt();
    _gameVersion.minor = version.at("minor").asInt();
    _gameVersion.patch = version.at("patch").asInt();
    _gameVersion.build = version.at("build").asInt();
    _gameVersion.platform = getTargetPlatform();

    result = true;
  } while (false);

  return result;
}

std::string BasicApp::getPlatformName(const Platform platform)
{
  switch (platform)
  {
    case Platform::OS_WINDOWS:
      return "Windows";
    case Platform::OS_LINUX:
      return "Linux";
    case Platform::OS_MAC:
      return "Mac";
    case Platform::OS_ANDROID:
      return "Android";
    case Platform::OS_IPHONE:
      return "iPhone";
    case Platform::OS_IPAD:
      return "iPad";
    default:
      return "Unknown";
  }
}
