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

#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "../utils/base/scene/basic_scene.h"

// forward declarations
enum class menu_to_display;

class loading_scene final : public basic_scene
{
public:
  using base_class = basic_scene;

  loading_scene();

  ~loading_scene();

  static Scene* game(basic_app* application, const bool debug_grid, const bool debug_physics,
                     const unsigned short int level);
  static Scene* menu(basic_app* application, const menu_to_display menu,
                     const unsigned short int selected_level);

private:
  enum class load_to
  {
    to_game,
    to_menu
  };

  bool init(basic_app* application, const load_to& type, const bool debug_grid, const bool debug_physics,
            const menu_to_display menu, const unsigned short int level);

  void go_to_scene() const;

  load_to type_;
  bool debug_grid_;
  bool debug_physics_;
  menu_to_display menu_;
  unsigned short int level_;
};

#endif // __LOADING_SCENE_H__
