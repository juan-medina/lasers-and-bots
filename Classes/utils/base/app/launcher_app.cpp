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

#include "launcher_app.h"

launcher_app::launcher_app(const std::string application_name, const float width, const float height):
  base_class(application_name),
  want_to_play_(false)
{
  screen_size_ = Size(width, height);
  design_resolution_ = Size(width, height);
  screen_size_ = Size(width, height);
}

bool launcher_app::applicationDidFinishLaunching()
{
  auto ret = false;

  do
  {
    // initialize director
    auto director = Director::getInstance();
    auto open_gl_view = director->getOpenGLView();

    if (!open_gl_view)
    {
      open_gl_view = GLViewImpl::createWithRect(name_, Rect(0, 0, screen_size_.width, screen_size_.height));

      director->setOpenGLView(open_gl_view);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    center_win32_window();
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60.0f);

    open_gl_view->setDesignResolutionSize(design_resolution_.width, design_resolution_.height,
                                          ResolutionPolicy::FIXED_WIDTH);

    // create a scene. it's an autorelease object
    const auto scene = this->init_scene();

    UTILS_BREAK_IF(scene == nullptr);

    // run
    director->runWithScene(scene);

    ret = true;
  }
  while (false);

  return ret;
}
