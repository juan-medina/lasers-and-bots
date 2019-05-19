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

// forward declarations
class audio_helper;

// application base class
class basic_app : public Application
{
public:
  using base_class = Application;

  explicit basic_app(const std::string& application_name, const float design_width, const float design_height);

  void initGLContextAttrs() override;

  // this function will be called when the app is ready to initialize
  bool applicationDidFinishLaunching() override;

  // this function will be called when the app is active again
  void applicationDidEnterBackground() override;

  // This function will be called when the app is inactive. When comes a phone call,it's be invoked too
  void applicationWillEnterForeground() override;

  virtual Scene* init_scene() = 0;

  virtual const string& application_name() const
  {
    return application_name_;
  };

  void close();

  audio_helper* get_audio_helper() const
  {
    return audio_helper_;
  }

  void set_window_size(const float scale);

  void set_full_screen(const bool full_screen)
  {
    full_screen_ = full_screen;
  }

  void set_fit_all(const bool fit_all)
  {
    fit_all_ = fit_all;
  }

  void set_show_fps(const bool show_fps)
  {
    show_fps_ = show_fps;
  }

  bool is_desktop();

private:

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
  // center on screen in windows 32 client
  static void center_win32_window();
#endif

  audio_helper* audio_helper_;
  float design_width_;
  float design_height_;
  int window_width_;
  int window_height_;
  bool full_screen_;
  bool fit_all_;
  bool show_fps_;
  string application_name_;
};

#endif // __BASIC_APP_H__
