/****************************************************************************
 Copyright (c) 2019 Juan Medina

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
#ifndef __SCROLLING_TEXT_CLASS__
#define __SCROLLING_TEXT_CLASS__

#include "../utils/utils.h"

// forward declarations
namespace cocos2d
{
namespace ui
{
class RichText;
}
} // namespace cocos2d

class scrolling_text final : public Layer
{
public:
  using base_class = Layer;

  scrolling_text();

  static scrolling_text* create(const Size& size, const std::string text_file, const bool centered = false);

  bool init(const Size& size, const std::string text_file, const bool centered);

  virtual void update(float delta) override;

  void set_scroll(const float scroll);

  void auto_scroll_in(const float time);

private:
  void start_scroll();

  cocos2d::ui::RichText* rich_text_;
  bool auto_scroll_;
  float auto_scroll_direction_;
  float scroll_pos_;
};

#endif // __SCROLLING_TEXT_CLASS__
