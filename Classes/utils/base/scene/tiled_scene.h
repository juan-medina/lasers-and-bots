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

#ifndef __TILED_SCENE_H__
#define __TILED_SCENE_H__

#include "grid_scene.h"

class tiled_scene : public grid_scene
{
public:
  // base_class
  using base_class = grid_scene;

  tiled_scene();

  // create with a tmx file
  static tiled_scene* create(Application* application, const std::string& tmx_file);

  // create the scene
  static Scene* scene(Application* application, const std::string& tmx_file);

  // init the scene
  bool init(Application* application, const std::string& tmx_file);

  virtual experimental::TMXTiledMap* get_tiled_map() const noexcept
  {
    return tiled_map_;
  };

protected:
  //our title map
  experimental::TMXTiledMap* tiled_map_;
};

#endif // __TILED_SCENE_H__
