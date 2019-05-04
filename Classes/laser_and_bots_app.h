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

#ifndef _APP_DELEGATE_H_
#define _APP_DELEGATE_H_

#include "utils/base/app/basic_app.h"

// this game application
class laser_and_bots_app final : public basic_app
{
public:
  using base_class = basic_app;

  laser_and_bots_app();

  laser_and_bots_app(const int screen_width, const int screen_height, const bool full_screen);

  Scene* init_scene() override;

  void set_effects_muted(const bool effects_muted);

  void set_music_muted(const bool music_muted);

  void to_game();

  void applicationDidEnterBackground() override;


private:

  bool effects_muted_;
  bool music_muted_;
  bool debug_grid_;
  bool debug_physics_;
};

#endif // _APP_DELEGATE_H_
