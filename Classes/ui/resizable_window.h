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
#ifndef __RESIZABLE_WINDOW_CLASS__
#define __RESIZABLE_WINDOW_CLASS__

#include "../utils/utils.h"

class resizable_window : public Node
{
public:
  using base_class = Node;

  resizable_window();

  static resizable_window* create(const std::string& title, const float width, const float height,
                                  const Color3B& color = Color3B(0, 255, 255));

  bool init(const std::string& title, const float width, const float height,
            const Color3B& color = Color3B(0, 255, 255));

  void set_title(const std::string& title) const { label_title_->setString(title); }

private:
  Label* label_title_;
};

#endif // __RESIZABLE_WINDOW_CLASS__
