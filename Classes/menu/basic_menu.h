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

#ifndef __BASIC_MENU_CLASS__
#define __BASIC_MENU_CLASS__

#include "../utils/utils.h"

//foward declarations
class audio_helper;

class basic_menu : public Node
{
public:
  using base_class = Node;

  basic_menu();

  bool init(const std::string& name, audio_helper* audio_helper);

  void display();

  void hide();

  audio_helper* get_audio_helper() const
  {
    return audio_helper_;
  }

protected:

  virtual bool create_menu_items() =0;
  void add_button(MenuItem* item, const ccMenuCallback& callback);
  bool add_text_button(const std::string& text, const ccMenuCallback& callback);

private:

  audio_helper* audio_helper_;
  Vector<MenuItem*> buttons_;
  float current_text_button_y_;
};


#endif // __BASIC_MENU_CLASS__
