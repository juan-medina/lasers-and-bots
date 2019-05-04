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

#ifndef __PHYSICS_SHAPE_CACHE_CLASS__
#define __PHYSICS_SHAPE_CACHE_CLASS__

#include "../utils.h"

class physics_shape_cache
{
public:

  physics_shape_cache();

  ~physics_shape_cache();

  bool add_shapes_with_file(const std::string& plist);

  bool add_shapes_with_file(const std::string& plist, float scale_factor);

  void remove_shapes_with_file(const std::string& plist);

  void remove_all_shapes();

  PhysicsBody* create_body_with_name(const std::string& name);


private:
  typedef enum
  {
    fixture_polygon,
    fixture_circle
  } fixture_type;


  class polygon
  {
  public:
    Point* vertices;
    int num_vertices;
  };


  class fixture_data
  {
  public:
    fixture_type fixture_type;

    float density;
    float restitution;
    float friction;

    int tag;
    int group;
    int category_mask;
    int collision_mask;
    int contact_test_mask;

    // for circles
    Point center;
    float radius;

    std::vector<polygon *> polygons;
  };

  class body_def
  {
  public:
    Point anchor_point;
    std::vector<fixture_data *> fixtures;

    bool is_dynamic;
    bool affected_by_gravity;
    bool allows_rotation;

    float linear_damping;
    float angular_damping;
    float velocity_limit;
    float angular_velocity_limit;
  };

  static void safe_delete_body_def(body_def* body_def);
  body_def* get_body_def(const std::string& name);
  static void set_body_properties(PhysicsBody* body, body_def* bd);
  static void set_shape_properties(PhysicsShape* shape, fixture_data* fd);

  std::map<std::string, body_def *> body_defs_;
  std::map<std::string, std::vector<body_def *>> bodies_in_file_;
};


#endif // __PHYSICS_SHAPE_CACHE_CLASS__
