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

#ifndef __OPTIONS_MENU_CLASS__
#define __OPTIONS_MENU_CLASS__

#include "../utils/utils.h"
#include "basic_menu.h"

//foward declarations
class audio_helper;
class slider_object;

class options_menu final : public basic_menu
{
public:

  using base_class = basic_menu;

  options_menu();

  static options_menu* create(audio_helper* audio_helper, const bool is_desktop_application);

  bool init(audio_helper* audio_helper, const bool is_desktop_application);

  void display() override;

protected:

  bool create_menu_items() override;

private:

  bool desktop_application_;

  MenuItemToggle* sound_toggle_;
  MenuItemToggle* music_toggle_;
  slider_object* sound_slider_;
  slider_object* music_slider_;
  MenuItemToggle* full_screen_toggle_;
  MenuItemToggle* windowed_toggle_;

  void on_back();
  void on_music();
  void on_sound();
  void on_music_slider_change(const float percentage);
  void on_sound_slider_change(const float percentage);
  void on_full_screen();
  void on_windowed();
  void update_labels() const;
  static Label* get_button_label(MenuItem* button);
};


#endif // __OPTIONS_MENU_CLASS__
