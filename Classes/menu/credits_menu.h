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

#ifndef __CREDITS_MENU_CLASS__
#define __CREDITS_MENU_CLASS__

#include "basic_menu.h"

// forward declarations
class AudioHelper;
class text_button;
class scrolling_text;

class credits_menu final : public basic_menu
{
public:
  using base_class = basic_menu;

  credits_menu();

  static credits_menu* create(AudioHelper* audio_helper);

  bool init(AudioHelper* audio_helper);

  virtual void display() override;

protected:
  bool create_menu_items() override;

private:
  void on_back();

  text_button* back_item_;
  scrolling_text* scrolling_text_;
};

#endif // __CREDITS_MENU_CLASS__
