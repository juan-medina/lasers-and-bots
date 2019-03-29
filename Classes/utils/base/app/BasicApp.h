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

#ifndef __BASIC_APP_H__
#define __BASIC_APP_H__

#include "../../utils.h"

// application base class
class BasicApp : Application
{
public:
  // parent
  typedef Layer parent;

  // constructor
  BasicApp(const char* applicationName);

  // destructor
  ~BasicApp();

  virtual void initGLContextAttrs();

  // Implement Director and Scene init code here.
  virtual bool applicationDidFinishLaunching();

  // The application enter in background
  virtual void applicationDidEnterBackground();

  // the application enter foreground
  virtual void applicationWillEnterForeground();

  // init our scene
  virtual Scene* initScene() = 0;

  // scene name
  CC_SYNTHESIZE_READONLY(string, _name, Name);

protected:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
  // center on screen in windows 32 client
  void CenterWin32Window();
#endif

  // our design resolution
  cocos2d::Size _designResolution;

  // our screen size
  cocos2d::Size _screenSize;
};

#endif // __BASIC_APP_H__
