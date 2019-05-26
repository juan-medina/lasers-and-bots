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

  void set_default_menu_item(MenuItem* item);
  void select_menu_item(MenuItem* item);

  MenuItem* get_selected_menu_item() const
  {
    return selected_menu_item_;
  }

  void move_selection_left()
  {
    if (!moving_)
    {
      move_selection(compare_left_, distance_left_);
    }
  }

  void move_selection_right()
  {
    if (!moving_)
    {
      move_selection(compare_right_, distance_right_);
    }
  }

  void move_selection_up()
  {
    if (!moving_)
    {
      move_selection(compare_up_, distance_up_);
    }
  }

  void move_selection_down()
  {
    if (!moving_)
    {
      move_selection(compare_down_, distance_down_);
    }
  }

  void activate_selection() const
  {
    if (!moving_)
    {
      if (selected_menu_item_ != nullptr)
      {
        selected_menu_item_->activate();
      }
    }
  }

  void selection_back() const
  {
    if (!moving_)
    {
      on_menu_back();
    }
  }

protected:

  virtual bool create_menu_items() = 0;

  void move_text_button(MenuItem* item);
  void add_button(MenuItem* item, const ccMenuCallback& callback);

  text_button* add_text_button(const std::string& text, const ccMenuCallback& callback);
  text_toggle* add_toggle_text_button(const std::string& text, const ccMenuCallback& callback,
                                      const bool not_move = false);
  text_toggle* add_small_button(const std::string& text, const ccMenuCallback& callback);
  slider_object* add_slider(MenuItem* attach_to, const float_callback& callback);
  MenuItem* add_row_label(const std::string& text, MenuItem* attach_to, const float left_space);

private:

  void on_movement_end();

  audio_helper* audio_helper_;
  Vector<MenuItem*> buttons_;
  float current_text_button_y_;
  float current_image_button_x_;
  float current_image_button_y_;
  float image_button_start_x_;

  bool moving_;

  DrawNode* selection_;
  DrawNode* previous_selection_;
  MenuItem* selected_menu_item_;
  MenuItem* default_menu_item_;
  Menu* menu_;

  using compare_function = std::function<bool(MenuItem*)>;
  using distance_function = std::function<float(MenuItem*)>;

  const compare_function compare_up_ = [&](MenuItem* item)
  {
    return item->getPosition().y > this->selected_menu_item_->getPosition().y;
  };

  const compare_function compare_down_ = [&](MenuItem* item)
  {
    return item->getPosition().y < this->selected_menu_item_->getPosition().y;
  };

  const compare_function compare_right_ = [&](MenuItem* item)
  {
    return item->getPosition().x > this->selected_menu_item_->getPosition().x;
  };

  const compare_function compare_left_ = [&](MenuItem* item)
  {
    return item->getPosition().x < this->selected_menu_item_->getPosition().x;
  };

  const distance_function distance_up_ = [&](MenuItem* item)
  {
    const auto pos1 = item->getPosition() + Vec2(0, item->getContentSize().height / 2);
    const auto pos2 = this->selected_menu_item_->getPosition() - Vec2(
      0, this->selected_menu_item_->getContentSize().width / 2);
    return pos1.distance(pos2);
  };

  const distance_function distance_down_ = [&](MenuItem* item)
  {
    const auto pos1 = item->getPosition() - Vec2(0, item->getContentSize().height / 2);
    const auto pos2 = this->selected_menu_item_->getPosition() + Vec2(
      0, this->selected_menu_item_->getContentSize().width / 2);
    return pos1.distance(pos2);
  };

  const distance_function distance_right_ = [&](MenuItem* item)
  {
    const auto pos1 = item->getPosition() - Vec2(item->getContentSize().width / 2, 0);
    const auto pos2 = this->selected_menu_item_->getPosition() + Vec2(
      this->selected_menu_item_->getContentSize().width / 2, 0);
    return pos1.distance(pos2);
  };

  const distance_function distance_left_ = [&](MenuItem* item)
  {
    const auto pos1 = item->getPosition() + Vec2(item->getContentSize().width / 2, 0);
    const auto pos2 = this->selected_menu_item_->getPosition() - Vec2(
      this->selected_menu_item_->getContentSize().width / 2, 0);
    return pos1.distance(pos2);
  };

  void move_selection(const compare_function& compare_fun, const distance_function& distance_fun);

  void on_menu_back() const;

  void draw_selection(DrawNode* draw, MenuItem* item, const Color4F& color) const;
};

#endif // __BASIC_MENU_CLASS__
