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

#include "LauncherApp.h"

LauncherApp::LauncherApp(const char* applicationName, float width, float height)
  : parent(applicationName), _wantToPlay(false)
{
  _screenSize = cocos2d::Size(width, height);
  _designResolution = cocos2d::Size(width, height);
}

LauncherApp::~LauncherApp()
{
}

bool LauncherApp::applicationDidFinishLaunching()
{

  bool ret = false;

  do
  {
    // init vars

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if (!glview)
    {

      glview = GLViewImpl::createWithRect(_name, Rect(0, 0, _screenSize.width, _screenSize.height));

      director->setOpenGLView(glview);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    this->CenterWin32Window();
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60.0f);

    glview->setDesignResolutionSize(_designResolution.width, _designResolution.height, ResolutionPolicy::FIXED_WIDTH);

    // create a scene. it's an autorelease object
    auto scene = this->initScene();

    UTILS_BREAK_IF(scene == nullptr);

    // run
    director->runWithScene(scene);

    ret = true;
  } while (0);

  return ret;
}
