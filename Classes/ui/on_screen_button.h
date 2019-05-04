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

enum class button_type
{
  none,
  up,
  down,
  left,
  right,
  button_a,
  button_b
};

class on_screen_button final : public Node
{
public:
  using base_class = Node;

  on_screen_button();

  static on_screen_button* create(const button_type& type, const std::string& sprite_frame_name,
                                  const std::string& label = "");

  bool init(const button_type& type, const std::string& sprite_frame_name, const std::string& label);

  const cocos2d::Size& getContentSize() const override;

  void pushed(const bool pushed);

  inline constexpr bool is_pushed() const noexcept
  {
    return pushed_;
  }

  void disabled(const bool disabled);

  const button_type& type() const
  {
    return type_;
  }

private:

  void on_status_change() const;

  bool is_touched_by_location(const Vec2& location) const;

  void on_touches_began(const std::vector<Touch*>& touches, Event* unused_event);
  void on_touches_moved(const std::vector<Touch*>& touches, Event* unused_event);
  void on_touches_ended(const std::vector<Touch*>& touches, Event* unused_event);
  void on_touches_cancel(const std::vector<Touch*>& touches, Event* unused_event);

  bool enable_touch(const bool enabled);

  Sprite* normal_sprite_;
  Sprite* pushed_sprite_;
  Sprite* disabled_sprite_;

  Label* label_button_;

  bool pushed_;
  bool disabled_;
  int saved_touch_id_;

  EventListenerTouchAllAtOnce* touch_listener_;

  button_type type_;
};

#endif // __ON_SCREEN_BUTTON_CLASS__
