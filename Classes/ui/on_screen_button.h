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
#ifndef __ON_SCREEN_BUTTON_CLASS__
#define __ON_SCREEN_BUTTON_CLASS__

#include "../utils/utils.h"

class on_screen_button final : public Node
{
public:
  // base_class
  using base_class = Node;

  // constructor
  on_screen_button();

  // create the object
  static on_screen_button* create(const std::string& sprite_frame_name);

  // init this object
  bool init(const std::string& sprite_frame_name);

  // get this content size
  const cocos2d::Size& getContentSize() const override;

  // the button is pushed
  void pushed(const bool pushed);

  // is the button pushed
  inline constexpr bool is_pushed() const noexcept
  {
    return pushed_;
  }

  // disable the button
  void disabled(const bool disabled);

private:

  // the status has change
  void on_status_change() const;

  // does a location touch this button
  bool is_touched_by_location(const Vec2& location) const;

  // begin touches event
  void on_touches_began(const std::vector<Touch*>& touches, Event* unused_event);

  // touches moved event
  void on_touches_moved(const std::vector<Touch*>& touches, Event* unused_event);

  // touches end event
  void on_touches_ended(const std::vector<Touch*>& touches, Event* unused_event);

  // touches cancel
  void on_touches_cancel(const std::vector<Touch*>& touches, Event* unused_event);

  // create touch listener
  bool enable_touch(const bool enabled);

  // normal sprite
  Sprite* normal_sprite_;

  // normal sprite
  Sprite* pushed_sprite_;

  // normal sprite
  Sprite* disabled_sprite_;

  // is the button pushed
  bool pushed_;

  // is the button disabled
  bool disabled_;

  // the saved touch id
  int saved_touch_id_;

  // the touch listener
  EventListenerTouchAllAtOnce* touch_listener_;
};

#endif // __ON_SCREEN_BUTTON_CLASS__
