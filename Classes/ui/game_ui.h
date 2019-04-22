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

class virtual_joy_stick;

class game_ui final : public Node
{
public:
  // base_class
  using base_class = Node;

  // constructor
  game_ui();

  // destructor
  ~game_ui();

  // create the object
  static game_ui* create();

  // init this object
  bool init() override;

  // when close
  void on_pause(Ref* sender);

  // when pause
  void on_close(Ref* sender);

  // when reload
  void on_reload(Ref* sender);

  // get the joystick
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

  // disable pause
  inline void disable_pause() const
  {
    pause_item_->setEnabled(false);
  }

  // update our time
  void update_time(const float time) const;

  // get time  string for the message
  static string time_message(const unsigned int time);

  // display a message
  void display_message(const std::string& message, const bool extended = false, const unsigned short int stars = 0,
                       const unsigned int limit_seconds = 0);

private:

  // our joy stick
  virtual_joy_stick* virtual_joy_stick_;

  // the shield bar
  ProgressTimer* shield_bar_;

  // shield label
  Label* shield_label_;

  // pause menu item
  MenuItemToggle* pause_item_;

  // the time label
  Label* time_label_;
};

#endif // __GAME_UI__CLASS__
