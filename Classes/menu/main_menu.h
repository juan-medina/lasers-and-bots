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

#ifndef __PAUSE_WINDOW_CLASS__
#define __MAIN_MENU_CLASS__

#include "../utils/utils.h"

//foward declarations
class audio_helper;

class main_menu final : public Node
{
public:
  using base_class = Node;

  main_menu();

  static main_menu* create(audio_helper* audio_helper);

  bool init(audio_helper* audio_helper);

  void display();

  void hide();

private:

  void on_options();
  void on_play();
  void on_exit();

  void add_button(MenuItem* item, const ccMenuCallback& callback);
  bool add_text_button(const std::string& text, const ccMenuCallback& callback);

  audio_helper* audio_helper_;
  Vector<MenuItem*> buttons_;
  float current_text_button_y_;
};


#endif // __MAIN_MENU_CLASS__
