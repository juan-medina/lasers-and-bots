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

#ifndef __PHYSICS_SHAPE_CACHE_H__
#define __PHYSICS_SHAPE_CACHE_H__

#include "../utils.h"

// class that store physics body shapes cache
class physics_body_cache final
{
public:

  // constructor
  physics_body_cache();

  // destructor
  ~physics_body_cache();

  // get default audio helper
  static physics_body_cache* get_instance()
  {
    // create if not create default helper and return it
    static physics_body_cache cache;

    return &cache;
  }

  // init helper
  bool init();

  // end helper
  void end();

private:
  // is the object initiated ?
  bool initiated_;

  class saved_shape
  {
  protected:
    // constructor
    saved_shape(const unsigned short int num_vertex);

    // destructor
    ~saved_shape();

    Vec2* vertex_ = nullptr;
    unsigned short int num_vertex_ = 0;
  };

  // gid shapes cache type
  using string_shape_cache = std::map<string, saved_shape*>;

  // gid shapes cache
  string_shape_cache cache_;
};

#endif // __PHYSICS_SHAPE_CACHE_H__
