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

#include "basic_app.h"
#include <utility>
#include "../../audio/audio_helper.h"

basic_app::basic_app(std::string application_name):
  name_(application_name)
{
}

basic_app::~basic_app()
{
  audio_helper::get_instance()->app_exit();
}

// if you want a different context,just modify the value of glContextAttrs
// it will takes effect on all platforms
void basic_app::initGLContextAttrs()
{
  // set OpenGL context attributions,now can only set six attributions:
  // red,green,blue,alpha,depth,stencil
  GLContextAttrs gl_context_attrs = {8, 8, 8, 8, 24, 8, 0};

  GLView::setGLContextAttrs(gl_context_attrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
  return 0; // flag for packages manager
}

bool basic_app::applicationDidFinishLaunching()
{
  auto ret = false;

  do
  {
    // read settings

    const auto design_width = UserDefault::getInstance()->getIntegerForKey("design_width", 4.0f * 1920.f);
    const auto design_height = UserDefault::getInstance()->getIntegerForKey("design_height", 4.0f * 1080.f);

    UserDefault::getInstance()->setIntegerForKey("design_width", design_width);
    UserDefault::getInstance()->setIntegerForKey("design_height", design_height);

    const auto screen_width = UserDefault::getInstance()->getIntegerForKey("screen_width", 1920);
    const auto screen_height = UserDefault::getInstance()->getIntegerForKey("screen_height", 1080);

    UserDefault::getInstance()->setIntegerForKey("screen_width", screen_width);
    UserDefault::getInstance()->setIntegerForKey("screen_height", screen_height);

    design_resolution_ = Size(design_width, design_height);
    screen_size_ = Size(screen_width, screen_height);

    const auto full_screen = UserDefault::getInstance()->getBoolForKey("full_screen", false);
    UserDefault::getInstance()->setBoolForKey("full_screen", full_screen);

    const auto fit_all = UserDefault::getInstance()->getBoolForKey("fit_all", false);
    UserDefault::getInstance()->setBoolForKey("fit_all", fit_all);

    // initialize director and view
    auto director = Director::getInstance();
    auto open_gl_view = director->getOpenGLView();

    if (!open_gl_view)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
      if (full_screen)
      {
        open_gl_view = GLViewImpl::createWithFullScreen(name_);
      }
      else
      {
        open_gl_view = GLViewImpl::createWithRect(name_, Rect(0, 0, screen_size_.width, screen_size_.height));
      }
#else
      open_gl_view = GLViewImpl::create(name_);
#endif
      director->setOpenGLView(open_gl_view);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (!full_screen)
    {
      center_win32_window();
    }
#endif
    // show fps flag, by default false
    auto show_fps = false;

    // get store value for show fps
    show_fps = UserDefault::getInstance()->getBoolForKey("show_fps", show_fps);

    // store the value
    UserDefault::getInstance()->setBoolForKey("show_fps", show_fps);
    UserDefault::getInstance()->flush();

    // turn on display FPS
    director->setDisplayStats(show_fps);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60.0f);

    // get the real screen scale
    const auto real_screen = open_gl_view->getVisibleSize();

    // Set the design resolution, we scale to our design with so aspect ratio is maintained
    if (fit_all)
    {
      open_gl_view->setDesignResolutionSize(design_resolution_.width, design_resolution_.height,
                                            ResolutionPolicy::SHOW_ALL);
    }
    else
    {
      const auto ratio_x = real_screen.width / design_resolution_.width;
      const auto ratio_y = real_screen.height / design_resolution_.height;

      if (ratio_x >= ratio_y)
      {
        open_gl_view->setDesignResolutionSize(design_resolution_.width, design_resolution_.height,
                                              ResolutionPolicy::FIXED_WIDTH);
      }
      else
      {
        open_gl_view->setDesignResolutionSize(design_resolution_.width, design_resolution_.height,
                                              ResolutionPolicy::FIXED_HEIGHT);
      }
    }

    // for plugins
    register_all_packages();

    // create a scene. it's an autorelease object
    const auto scene = init_scene();
    UTILS_BREAK_IF(scene == nullptr);

    // run
    director->runWithScene(scene);

    ret = true;
  }
  while (false);

  return ret;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void basic_app::applicationDidEnterBackground()
{
  Director::getInstance()->stopAnimation();

  audio_helper::get_instance()->app_to_bg();
}

// this function will be called when the app is active again
void basic_app::applicationWillEnterForeground()
{
  Director::getInstance()->startAnimation();

  audio_helper::get_instance()->app_to_fg();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void basic_app::center_win32_window()
{
  // get the window handler
  const auto win32_window = Director::getInstance()->getOpenGLView()->getWin32Window();

  if (!win32_window)
  {
    return;
  }

  // get the desktop dimensions
  RECT rc_desktop, rc_window;
  GetWindowRect(GetDesktopWindow(), &rc_desktop);

  // subtract the task bar
  const auto h_task_bar = FindWindow(TEXT("Shell_TrayWnd"), nullptr);
  if (h_task_bar != nullptr)
  {
    APPBARDATA abd;

    abd.cbSize = sizeof(APPBARDATA);
    abd.hWnd = h_task_bar;

    SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
    SubtractRect(&rc_desktop, &rc_desktop, &abd.rc);
  }

  // get our window dimensions
  GetWindowRect(win32_window, &rc_window);

  // calculate center position
  int offset_x = rc_desktop.left + (rc_desktop.right - rc_desktop.left - (rc_window.right - rc_window.left)) / 2;
  offset_x = (offset_x > 0) ? offset_x : rc_desktop.left;
  int offset_y = rc_desktop.top + (rc_desktop.bottom - rc_desktop.top - (rc_window.bottom - rc_window.top)) / 2;
  offset_y = (offset_y > 0) ? offset_y : rc_desktop.top;

  // move the window
  SetWindowPos(win32_window, nullptr, offset_x, offset_y, 0, 0,
               SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}
#endif
