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
#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include "../../utils.h"

class game_object : public Sprite
{
public:
  // base_class
  using base_class = Sprite;

  // constructor
  game_object() :
    animation_(nullptr)
  {
  }

  // create the object
  static game_object* create(const std::string& sprite_frame_name);

  // init this object
  virtual bool init(const std::string& sprite_frame_name);

protected:

  // helper for fuzzy equals
  static constexpr bool fuzzy_equals(const float a, const float b, const float var = fuzzy_range) noexcept
  {
    return a - var <= b && b <= a + var;
  }


  // create a animation
  static bool create_anim(const char* pattern, int max_frame, float speed, const char* name,
                          unsigned int loops = infinite_loops);

  // change animation
  void change_anim(const std::string& name);

private:

  static constexpr int infinite_loops = -1;
  static constexpr float fuzzy_range = 5.f;

  // our current animation
  Action* animation_;
};

#endif // __GAME_OBJECT__
