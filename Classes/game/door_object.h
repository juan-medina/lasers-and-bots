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
#ifndef __DOOR_CLASS__
#define __DOOR_CLASS__

#include "../utils/physics/physics_game_object.h"
#include "../utils/audio/audio_helper.h"

class door_object final : public physics_game_object
{
public:
  using base_class = physics_game_object;

  door_object();

  static door_object* create(audio_helper* audio_helper);

  bool init(audio_helper* audio_helper);

  bool is_on() const
  {
    return on_;
  }

  bool is_off() const
  {
    return !on_;
  }

  bool is_open() const
  {
    return open_;
  }

  bool is_closed() const
  {
    return !open_;
  }

  void on();

  void open();

private:

  bool on_;
  bool open_;

  audio_helper* audio_helper_;
};

#endif // __DOOR_CLASS__
