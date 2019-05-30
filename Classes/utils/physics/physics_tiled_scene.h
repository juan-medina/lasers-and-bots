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

#ifndef __PHYSICS_TILED_SCENE_H__
#define __PHYSICS_TILED_SCENE_H__

#include "../base/scene/tiled_scene.h"

// forward declarations
class physics_shape_cache;

class physics_tiled_scene : public tiled_scene
{
public:
  using base_class = tiled_scene;

  physics_tiled_scene();
  ~physics_tiled_scene();

  static physics_tiled_scene* create(basic_app* application, const std::string& tmx_file, const float gravity,
                                     const bool debug_physics);

  static Scene* scene(basic_app* application, const std::string& tmx_file, const float gravity,
                      const bool debug_physics);

  bool init(basic_app* application, const std::string& tmx_file, const float gravity, const bool debug_physics);

  physics_shape_cache* get_physics_shape_cache() const
  {
    return physics_shape_cache_;
  }

protected:

  bool add_body_to_node(Node* node, const string& shape) const;
  virtual Node* provide_physics_node(const int gid);
  string get_shape_from_tile_gid(const int gid);

private:

  virtual void init_physics(const bool debug_physics) const;
  bool add_physics_to_map();

  // create a dummy node to hold physic body
  Node* create_dummy_node(experimental::TMXLayer* layer, const Vec2& tile_pos, const int gid);

  float gravity_ = 0.0f;
  std::map<int, string> gid_to_shapes_;

  physics_shape_cache* physics_shape_cache_;
};

#endif // __PHYSICS_TILED_SCENE_H__
