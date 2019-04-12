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

#ifndef __PHYSICS_TILED_SCENE_H__
#define __PHYSICS_TILED_SCENE_H__

#include "tiled_scene.h"

class physics_tiled_scene : public tiled_scene
{
public:
  // base_class
  using base_class = tiled_scene;

  // create with a tmx file
  static physics_tiled_scene* create(const std::string& tmx_file, const float gravity, const bool debug_physics);

  // create the scene
  static Scene* scene(const std::string& tmx_file, const float gravity, const bool debug_physics);

  // init the scene
  bool init(const std::string& tmx_file, const float gravity, const bool debug_physics);

private:

  // get the shape string from a tile
  string get_shape_from_tile_gid(const int gid);

  // get the restitution from a tile
  float get_restitution_from_tile_gid(const int gid) const;

  // add a body to sprites
  static bool add_body_to_node(Node* node, const string& shape, const float restitution);

  // create a dummy node to hold physic body
  Node* create_dummy_node(experimental::TMXLayer* layer, const Vec2& tile_pos) const;

  // add physics to our game
  bool add_physics_to_map();

  float gravity_ = 0.0f;

  // init physics
  virtual void init_physics(const bool debug_physics) const;

  // gid shapes cache type
  using gid_shape_cache = std::map<int, string>;

  // gid shapes cache
  gid_shape_cache gid_to_shapes_;
};

#endif // __PHYSICS_TILED_SCENE_H__
