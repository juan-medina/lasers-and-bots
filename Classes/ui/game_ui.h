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

#ifndef __GAME_UI__CLASS__
#define __GAME_UI__CLASS__

#include "../utils/utils.h"

// forward declarations
class MessageWindow;
class level_completed;
class PauseWindow;
class VirtualJoyStick;
class AudioHelper;
class LevelManager;
class InputController;
enum class CompletedResult;

class game_ui final : public Node
{
public:
  using base_class = Node;

  game_ui();

  static game_ui* create(AudioHelper* audio_helper, InputController* input_controller,
                         LevelManager* level_manager, const unsigned short int level);

  bool init(AudioHelper* audio_helper, InputController* input_controller, LevelManager* level_manager,
            const unsigned short int level);

  void on_pause(Ref* sender);
  void on_close(Ref* sender);
  void on_reload(Ref* sender);

  inline VirtualJoyStick* get_virtual_joy_stick() const { return virtual_joy_stick_; }

  inline void set_shield_percentage(const float amount) const
  {
    shield_bar_->setPercentage(amount);
    const auto int_value = static_cast<int>(std::round(amount));
    shield_label_->setString(StringFormat("%3d %%", int_value));
  }

  void change_pause_button() const;
  void disable_buttons(const bool disabled) const;

  void update_time(const float time, const unsigned int time_limit);

  static string time_message(const float time);

  void display_message(const std::string& message, const std::string& sub_message,
                       const ccMenuCallback& callback);

  void display_level_completed(const unsigned short int level, const float time,
                               const unsigned short int stars, const CompletedResult completion,
                               const ccMenuCallback& callback);

  void update_countdown(const int value) const;

  void display_pause_window() const;

  void update(float delta) override;

private:
  void on_continue();

  VirtualJoyStick* virtual_joy_stick_;
  ProgressTimer* shield_bar_;
  Label* shield_label_;
  MenuItemToggle* pause_item_;
  Label* time_label_;
  Label* sub_time_label_;
  Label* countdown_label_;
  Label* level_name_label_;
  unsigned int time_limit_;
  ccMenuCallback continue_callback_;
  AudioHelper* audio_helper_;
  MessageWindow* message_window_;
  level_completed* level_completed_;
  PauseWindow* pause_window_;
  LevelManager* level_manager_;
  unsigned short int level_;
  InputController* input_controller_;
};

#endif // __GAME_UI__CLASS__
