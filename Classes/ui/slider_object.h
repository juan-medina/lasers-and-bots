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
#ifndef __SLIDER_OBJECT_CLASS__
#define __SLIDER_OBJECT_CLASS__

#include "../utils/utils.h"


class slider_object final : public MenuItemSprite
{
public:
  using base_class = MenuItemSprite;
  using slider_change_callback = std::function<void(const float)>;

  slider_object();

  static slider_object* create(const std::string& background, const std::string& progress,
                               const slider_change_callback& callback);

  bool init(const std::string& background, const std::string& progress, const slider_change_callback& callback);

  void set_percentage(const float percentage);
  float get_percentage() const;

  bool is_enabled() const
  {
    return enabled_;
  }

  void enable(const bool enabled);
  void setColor(const Color3B& color) override;
private:

  ProgressTimer* progress_;
  Label* label_;
  bool enabled_;

  bool enable_touch(const bool enabled);

  bool on_touch_began(Touch* touch, Event* unused_event);
  void on_touch_moved(Touch* touch, Event* unused_event);
  void on_touch_ended(Touch* touch, Event* unused_event);
  void on_touch_cancel(Touch* touch, Event* unused_event);

  bool is_touched_by_location(Node* node, const Vec2& location);
  void update_location(const Vec2& location);

  EventListenerTouchOneByOne* touch_listener_;
  Vec2 touch_begin_at_;
  float percent_begin_touch_;
  Sprite* arrow_;
  float last_percentage_;

  void on_percentage_change(const float percentage) const;
  std::function<void(const float)> callback_;
};

#endif // __SLIDER_OBJECT_CLASS__
