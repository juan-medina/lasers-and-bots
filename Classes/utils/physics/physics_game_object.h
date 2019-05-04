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
#ifndef __PHYSICS_GAME_OBJECT_CLASS__
#define __PHYSICS_GAME_OBJECT_CLASS__

#include "../base/sprite/game_object.h"
#include "physics_shape_cache.h"

class physics_game_object : public game_object
{
public:
  using base_class = game_object;

  static physics_game_object* create(physics_shape_cache* physics_shape_cache, const std::string& shape, const std::string& sprite_frame_name, const std::string& type);
  static physics_game_object* create(physics_shape_cache* physics_shape_cache, const std::string& shape, const std::string& type);
  static physics_game_object* create(const std::string& type);

  virtual bool init(physics_shape_cache* physics_shape_cache, const std::string& shape, const std::string& sprite_frame_name, const std::string& type);
  bool init(physics_shape_cache* physics_shape_cache, const std::string& shape, const std::string& type);
  bool init(const std::string& type) override;

private:

  bool set_shape(physics_shape_cache* physics_shape_cache, const std::string& shape_name);

};

#endif // __PHYSICS_GAME_OBJECT_CLASS__
