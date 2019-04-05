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
class basic_app : Application
{
public:
  // base_class
  using base_class = Layer;

  // constructor
  explicit basic_app(std::string application_name);

  // destructor
  ~basic_app();

  // init OpenGL attributes
  void initGLContextAttrs() override;

  // Implement Director and Scene init code here.
  bool applicationDidFinishLaunching() override;

  // The application enter in background
  void applicationDidEnterBackground() override;

  // the application enter foreground
  void applicationWillEnterForeground() override;

  // init our scene
  virtual Scene* init_scene() = 0;

  virtual string get_name() const
  {
    return name_;
  };

protected:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
  // center on screen in windows 32 client
  static void center_win32_window();
#endif

  // our design resolution
  Size design_resolution_;

  // our screen size
  Size screen_size_;

  // scene name
protected:
  string name_;
};

#endif // __BASIC_APP_H__
