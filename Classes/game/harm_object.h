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
#ifndef __harm_object_CLASS__
#define __harm_object_CLASS__

#include "../utils/physics/PhysicsGameObject.h"

class harm_object : public PhysicsGameObject
{
public:
  using base_class = PhysicsGameObject;

  harm_object();

  static harm_object* create(PhysicsShapeCache* physics_shape_cache, const std::string& shape,
                             const std::string& sprite_frame_name, const std::string& type, const int damage);
  static harm_object* create(PhysicsShapeCache* physics_shape_cache, const std::string& shape,
                             const std::string& type, const int damage);
  static harm_object* create(const std::string& type, const int damage);

  virtual bool init(PhysicsShapeCache* physics_shape_cache, const std::string& shape,
                    const std::string& sprite_frame_name, const std::string& type, const int damage);
  virtual bool init(PhysicsShapeCache* physics_shape_cache, const std::string& shape,
                    const std::string& type, const int damage);
  virtual bool init(const std::string& type, const int damage);

  int get_damage() const { return damage_; }

private:
  int damage_;
};

#endif // __harm_object_CLASS__
