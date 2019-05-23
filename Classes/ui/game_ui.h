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

#ifndef __GAME_UI__CLASS__
#define __GAME_UI__CLASS__

#include "../utils/utils.h"

// forward declarations
class message_window;
class pause_window;
class virtual_joy_stick;
class audio_helper;
class level_manager;

class game_ui final : public Node
{
public:
  using base_class = Node;

  game_ui();

  static game_ui* create(audio_helper* audio_helper, level_manager* level_manager, const unsigned short int level);

  bool init(audio_helper* audio_helper, level_manager* level_manager, const unsigned short int level);

  void on_pause(Ref* sender);
  void on_close(Ref* sender);
  void on_reload(Ref* sender);

  inline virtual_joy_stick* get_virtual_joy_stick() const
  {
    return virtual_joy_stick_;
  }

  inline void set_shield_percentage(const float amount) const
  {
    shield_bar_->setPercentage(amount);
    const auto int_value = static_cast<int>(std::round(amount));
    shield_label_->setString(string_format("%3d %%", int_value));
  }

  void change_pause_button() const;
  void disable_buttons(const bool disabled) const;

  void update_time(const float time, const unsigned int time_limit);

  static string time_message(const float time);

  void display_message(const std::string& message, const std::string& sub_message, const ccMenuCallback& callback,
                       const short int stars = -1);

  void update_countdown(const int value) const;

  void display_pause_window() const;

private:

  void on_continue();

  bool create_keyboard_listener();
  void on_key_pressed(EventKeyboard::KeyCode key_code, Event* event);

  bool create_controller_listener();
  void on_controller_key_down(Controller* controller, int key_code, Event* event);

  virtual_joy_stick* virtual_joy_stick_;
  ProgressTimer* shield_bar_;
  Label* shield_label_;
  MenuItemToggle* pause_item_;
  Label* time_label_;
  Label* sub_time_label_;
  Label* countdown_label_;
  Label* level_name_label_;
  unsigned int time_limit_;
  ccMenuCallback continue_callback_;
  audio_helper* audio_helper_;
  message_window* message_window_;
  pause_window* pause_window_;
  level_manager* level_manager_;
  unsigned short int level_;
};

#endif // __GAME_UI__CLASS__
