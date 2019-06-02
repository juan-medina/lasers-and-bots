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

#include "basic_app.h"
#include "../../audio/audio_helper.h"
#include "../../controller/input_controller.h"
#include "../scene/basic_scene.h"

basic_app::basic_app(const std::string& application_name, const float design_width, const float design_height)
  : audio_helper_(nullptr)
  , input_controller_(nullptr)
  , design_width_(design_width)
  , design_height_(design_height)
  , window_width_(0)
  , window_height_(0)
  , full_screen_(true)
  , fit_all_(false)
  , application_name_(application_name)
  , game_version_{0, 0, 0, 0, Platform::OS_LINUX}
{
}

void basic_app::initGLContextAttrs()
{
  // set OpenGL context attributions,now can only set six attributions:
  // red, green, blue, alpha, depth, stencil, multi sampling
  GLContextAttrs gl_context_attrs = {8, 8, 8, 8, 24, 8, 0};

  GLView::setGLContextAttrs(gl_context_attrs);
}

static int register_all_packages()
{
  return 0; // flag for packages manager
}

bool basic_app::applicationDidFinishLaunching()
{
  auto ret = false;

  do
  {
    auto director = Director::getInstance();
    auto open_gl_view = director->getOpenGLView();

    if (!open_gl_view)
    {
#if (GAME_PLATFORM == DESKTOP_GAME)
      if (full_screen_)
      {
        open_gl_view = GLViewImpl::createWithFullScreen(application_name_);
      }
      else
      {
        open_gl_view = GLViewImpl::createWithRect(application_name_,
                                                  Rect(Vec2::ZERO, Size(window_width_, window_height_)));
      }
#else
      open_gl_view = GLViewImpl::create(application_name_);
#endif
      director->setOpenGLView(open_gl_view);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (!full_screen_)
    {
      center_win32_window();
    }
#endif

    director->setAnimationInterval(1.0f / 60.0f);

    // get the real screen scale
    const auto real_screen = open_gl_view->getVisibleSize();

    // Set the design resolution, we scale to our design with so aspect ratio is
    // maintained
    if (fit_all_)
    {
      open_gl_view->setDesignResolutionSize(design_width_, design_height_, ResolutionPolicy::SHOW_ALL);
    }
    else
    {
      const auto ratio_x = real_screen.width / design_width_;
      const auto ratio_y = real_screen.height / design_height_;

      if (ratio_x >= ratio_y)
      {
        open_gl_view->setDesignResolutionSize(design_width_, design_height_, ResolutionPolicy::FIXED_WIDTH);
      }
      else
      {
        open_gl_view->setDesignResolutionSize(design_width_, design_height_, ResolutionPolicy::FIXED_HEIGHT);
      }
    }

    register_all_packages();

    audio_helper_ = new audio_helper();
    UTILS_BREAK_IF(audio_helper_ == nullptr);

    input_controller_ = input_controller::create();
    UTILS_BREAK_IF(input_controller_ == nullptr);

    const auto scene = init_scene();
    UTILS_BREAK_IF(scene == nullptr);

    UTILS_BREAK_IF(!read_version());

    director->runWithScene(scene);

    ret = true;
  } while (false);

  return ret;
}

void basic_app::applicationDidEnterBackground()
{
  const auto director = Director::getInstance();
  director->stopAnimation();

  const auto scene = dynamic_cast<basic_scene*>(director->getRunningScene());
  if (scene != nullptr)
  {
    scene->did_enter_background();
  }
}

void basic_app::applicationWillEnterForeground()
{
  const auto director = Director::getInstance();
  director->startAnimation();

  const auto scene = dynamic_cast<basic_scene*>(director->getRunningScene());
  if (scene != nullptr)
  {
    scene->will_enter_foreground();
  }
}

void basic_app::close()
{
  delete audio_helper_;
  audio_helper_ = nullptr;

  delete input_controller_;
  input_controller_ = nullptr;

  Director::getInstance()->end();
}

#if (GAME_PLATFORM == DESKTOP_GAME)
void basic_app::set_window_size(const float scale)
{
  glfwInit();
  const auto monitor = glfwGetPrimaryMonitor();
  const auto video_mode = glfwGetVideoMode(monitor);
  window_width_ = static_cast<int>(video_mode->width * scale);
  window_height_ = static_cast<int>(video_mode->height * scale);
  glfwTerminate();
}
#endif

bool basic_app::is_desktop()
{
  const auto platform = getTargetPlatform();
  return (platform == Platform::OS_WINDOWS) || (platform == Platform::OS_MAC);
}

std::string basic_app::get_game_version_string() const
{
  return string_format("Version: %d.%d.%d build %d (%s)", game_version_.major, game_version_.minor,
                       game_version_.patch, game_version_.build,
                       get_platform_name(game_version_.platform).c_str());
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
  int offset_x =
    rc_desktop.left + (rc_desktop.right - rc_desktop.left - (rc_window.right - rc_window.left)) / 2;
  offset_x = (offset_x > 0) ? offset_x : rc_desktop.left;
  int offset_y =
    rc_desktop.top + (rc_desktop.bottom - rc_desktop.top - (rc_window.bottom - rc_window.top)) / 2;
  offset_y = (offset_y > 0) ? offset_y : rc_desktop.top;

  // move the window
  SetWindowPos(win32_window, nullptr, offset_x, offset_y, 0, 0,
               SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}
#endif

bool basic_app::read_version()
{
  auto result = false;
  do
  {
    const auto version_data = FileUtils::getInstance()->getValueMapFromFile("version/version.plist");
    UTILS_BREAK_IF(version_data.empty());

    const auto version = version_data.at("version").asValueMap();
    UTILS_BREAK_IF(version.empty());

    game_version_.major = version.at("major").asInt();
    game_version_.minor = version.at("minor").asInt();
    game_version_.patch = version.at("patch").asInt();
    game_version_.build = version.at("build").asInt();
    game_version_.platform = getTargetPlatform();

    result = true;
  } while (false);

  return result;
}

std::string basic_app::get_platform_name(const Platform platform)
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
