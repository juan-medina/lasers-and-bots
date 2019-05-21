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
#include "../ui/resizable_window.h"

//foward declarations
class audio_helper;
class slider_object;
class text_button;
class text_toggle;

class basic_menu : public resizable_window
{
public:
  using base_class = resizable_window;
  using float_callback = std::function<void(const float)>;

  basic_menu();

  bool init(const std::string& name, audio_helper* audio_helper, const float width, const float height);

  virtual void display();

  virtual void hide();

  audio_helper* get_audio_helper() const
  {
    return audio_helper_;
  }

protected:

  virtual bool create_menu_items() = 0;

  void move_image_button(MenuItem* item);
  void move_text_button(MenuItem* item);
  void add_button(MenuItem* item, const ccMenuCallback& callback);

  text_button* add_text_button(const std::string& text, const ccMenuCallback& callback);
  text_toggle* add_toggle_text_button(const std::string& text, const ccMenuCallback& callback,
                                      const bool not_move = false);
  text_button* add_small_button(const std::string& text, const ccMenuCallback& callback);
  slider_object* add_slider(MenuItem* attach_to, const float_callback& callback);
  MenuItem* add_row_label(const std::string& text, MenuItem* attach_to, const float left_space);

private:

  audio_helper* audio_helper_;
  Vector<MenuItem*> buttons_;
  float current_text_button_y_;
  float current_image_button_x_;
  float current_image_button_y_;
  float image_button_start_x_;
};


#endif // __BASIC_MENU_CLASS__
