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

#ifndef __GAME_UI__CLASS__
#define __GAME_UI__CLASS__

#include "../utils/utils.h"

class game_ui final : public Node
{
public:
  // base_class
  using base_class = Node;

  // constructor
  game_ui();

  // destructor
  ~game_ui();

  // create the object
  static game_ui* create();

  // init this object
  bool init() override;

  // when close
  void on_close(Ref* sender);

private:
  // get a touch location in node space
  Vec2 get_location_in_node_space(Touch* touch) const;

  // begin touch event
  bool on_touch_began(Touch* touch, Event* unused_event) const;

  // touch moved event
  void on_touch_moved(Touch* touch, Event* unused_event);

  // touch end event
  void on_touch_ended(Touch* touch, Event* unused_event);

  // touch cancel
  void on_touch_cancel(Touch* touch, Event* unused_event);

  // register touch events
  void register_touch();

  // update velocity
  void update_velocity(Point point);

  // listener for touch events
  EventListenerTouchOneByOne* listener_;

  // joystick
  Sprite* joy_stick_;

  // thumb
  Sprite* thumb_;

  // velocity
  Vec2 velocity_;
};

#endif // __GAME_UI__CLASS__
