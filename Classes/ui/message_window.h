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
#ifndef __MESSAGE_WINDOW_CLASS__
#define __MESSAGE_WINDOW_CLASS__

#include "../menu/basic_menu.h"
#include "../utils/utils.h"

// foward declarations
class audio_helper;

class message_window final : public basic_menu
{
public:
  using base_class = basic_menu;

  message_window();

  static message_window* create(audio_helper* audio_helper);

  bool init(audio_helper* audio_helper);

  void display(const std::string& message, const std::string& sub_message, const ccMenuCallback& callback);

  void hide() override;

protected:
  bool create_menu_items() override;

private:
  audio_helper* audio_helper_;
  text_button* continue_item_;
  Label* sub_label_;
};

#endif // __MESSAGE_WINDOW_CLASS__
