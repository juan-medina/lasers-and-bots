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

#include "../utils/base/sprite/game_object.h"

class door_object final : public game_object
{
public:
  // base_class
  using base_class = game_object;

  // constructor
  door_object();

  // create the object
  static door_object* create();

  // init this object
  bool init();

  // is on
  bool is_on() const
  {
    return on_;
  }

  // is on
  bool is_off() const
  {
    return !on_;
  }

  // is open
  bool is_open() const
  {
    return open_;
  }

  // is close
  bool is_closed() const
  {
    return !open_;
  }

  // set on
  void on();

  // open the door
  void open();

private:

  bool on_;
  bool open_;

};

#endif // __DOOR_CLASS__
