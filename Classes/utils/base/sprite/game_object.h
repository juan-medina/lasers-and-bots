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
  game_object();

  // create the object
  static game_object* create(const std::string& sprite_frame_name, const std::string& type, const int damage = 0);

  // create empty
  static game_object* create(const std::string& type, const int damage = 0);

  // init this object
  virtual bool init(const std::string& sprite_frame_name, const std::string& type, const int damage = 0);

  // init empty
  virtual bool init(const std::string& type, const int damage = 0);

  // get type
  string get_type() const
  {
    return type_;
  }

  // get damage
  int get_damage() const
  {
    return damage_;
  }

protected:

  // create a animation
  static bool create_anim(const char* pattern, int max_frame, float speed, const char* name,
                          unsigned int loops = infinite_loops);

  // change animation
  void change_anim(const std::string& name);

  // change the frame
  void change_frame(const string& name);

private:

  static constexpr int infinite_loops = -1;
  

  // our current animation
  Action* animation_;

  // object type
  string type_;

  // damage
  int damage_;
};

#endif // __GAME_OBJECT__
