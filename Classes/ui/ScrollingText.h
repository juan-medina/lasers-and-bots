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
#ifndef __SCROLLINGTEXT_H__
#define __SCROLLINGTEXT_H__

#include "../utils/utils.h"

// forward declarations
namespace cocos2d
{
namespace ui
{
class RichText;
}
} // namespace cocos2d

class ScrollingText final : public Layer
{
public:
  using BaseClass = Layer;

  ScrollingText();

  static ScrollingText* create(const Size& size, const std::string& textFile, bool centered = false);

  bool init(const Size& size, const std::string& textFile, bool centered);

  void update(float delta) override;

  void setScroll(float scroll);

  void autoScrollIn(float time);

private:
  void startScroll();

  cocos2d::ui::RichText* _richText;
  bool _autoScroll;
  float _autoScrollDirection;
  float _scrollPos;
};

#endif //__SCROLLINGTEXT_H__
