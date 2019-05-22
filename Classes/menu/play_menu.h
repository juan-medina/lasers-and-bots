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

#ifndef __PLAY_MENU_CLASS__
#define __PLAY_MENU_CLASS__

#include "../utils/utils.h"
#include "basic_menu.h"

//foward declarations
class audio_helper;
class level_manager;
class text_button;
class text_toggle;

class play_menu final : public basic_menu
{
public:

  play_menu();

  using base_class = basic_menu;

  static play_menu* create(audio_helper* audio_helper);

  bool init(audio_helper* audio_helper);

  void display() override;

protected:
  bool create_menu_items() override;

private:
  Label* add_labels(const std::string& label_text, const std::string& text, const Vec2& pos, const float separation);
  void on_back();
  void on_level_select(Ref* sender, const int level);
  void on_play();
  void select_level(const int level);
  level_manager* get_level_manager();

  std::map<int, text_toggle*> level_buttons_;
  static constexpr auto star_tag = 0xFF0F;

  Label* level_name_label_;
  Label* level_time_limit_label_;
  Label* level_time_record_label_;
  Label* level_3_stars_time_record_label_;
  Label* level_stars_label_;
  int selected_level_;
};


#endif // __PLAY_MENU_CLASS__
