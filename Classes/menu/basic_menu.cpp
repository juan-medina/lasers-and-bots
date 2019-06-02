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

#include "basic_menu.h"
#include "../ui/slider_object.h"
#include "../ui/text_button.h"
#include "../ui/text_toggle.h"
#include "../utils/audio/audio_helper.h"

basic_menu::basic_menu()
  : audio_helper_(nullptr)
  , current_text_button_y_(0)
  , current_image_button_x_(0)
  , current_image_button_y_(0)
  , image_button_start_x_(0)
  , moving_(false)
  , selection_(nullptr)
  , previous_selection_(nullptr)
  , selected_menu_item_(nullptr)
  , default_menu_item_(nullptr)
  , selection_locked_(false)
  , menu_(nullptr)
  , animation_type_(animation_type::slide)
{
}

bool basic_menu::init(const std::string& name, audio_helper* audio_helper, const float width,
                      const float height, const animation_type animation_type /*= animation_type::slide*/)
{
  auto ret = false;

  do
  {
    audio_helper_ = audio_helper;
    animation_type_ = animation_type;

    UTILS_BREAK_IF(!base_class::init(name, width, height));

    audio_helper_->pre_load_effect("sounds/select.mp3");

    const auto& size = Director::getInstance()->getVisibleSize();

    setPosition(size.width / 2, size.height / 2);

    const auto horizontal_segment = getContentSize().height;
    const auto vertical_segment = getContentSize().width;

    image_button_start_x_ = -(horizontal_segment / 2) - 25.f;
    current_image_button_x_ = image_button_start_x_;
    current_image_button_y_ = (vertical_segment / 2) - 100.f;

    setCascadeOpacityEnabled(true);

    UTILS_BREAK_IF(!create_menu_items());

    menu_ = Menu::createWithArray(buttons_);
    UTILS_BREAK_IF(menu_ == nullptr);

    menu_->setPosition(0.f, -getContentSize().height / 2);

    addChild(menu_, 100);

    setOpacity(0);
    setVisible(false);

    if (animation_type_ == animation_type::slide)
    {
      setPosition(-size.width * 2, 0.f);
    }

    ret = true;
  } while (false);

  return ret;
}

void basic_menu::display()
{
  if (moving_)
  {
    return;
  }

  moving_ = true;

  static auto const time = 0.5f;
  const auto call_back = CallFunc::create(CC_CALLBACK_0(basic_menu::on_movement_end, this));

  if (animation_type_ == animation_type::slide)
  {
    const auto& size = Director::getInstance()->getVisibleSize();

    setVisible(true);
    setOpacity(190);
    const auto move = Vec2(size.width, 0);
    setPosition(Vec2(size.width / 2, size.height / 2) - move);

    const auto elastic_in = EaseElasticInOut::create(MoveBy::create(time * 2, move), time);
    const auto move_in = Sequence::create(elastic_in, call_back, nullptr);

    runAction(move_in);

    get_audio_helper()->play_effect("sounds/SlideClosed.mp3");
  }
  else
  {
    const auto fade_in_window = FadeTo::create(time, 190);
    const auto show = Show::create();
    const auto fade_show = Sequence::create(show, fade_in_window, call_back, nullptr);

    runAction(fade_show);
  }

  if ((selected_menu_item_ == nullptr) && (default_menu_item_ != nullptr))
  {
    select_menu_item(default_menu_item_);
  }
}

void basic_menu::hide()
{
  if (moving_)
  {
    return;
  }

  moving_ = true;

  static auto const time = 0.5f;
  const auto call_back = CallFunc::create(CC_CALLBACK_0(basic_menu::on_movement_end, this));

  if (animation_type_ == animation_type::slide)
  {
    const auto& size = Director::getInstance()->getVisibleSize();
    const auto move = Vec2(size.width, 0);

    const auto elastic_out = EaseElasticInOut::create(MoveBy::create(time * 2, move), time);
    const auto fade = FadeTo::create(0.0f, 0);
    const auto hide = Hide::create();

    const auto move_out = Sequence::create(elastic_out, fade, hide, call_back, nullptr);
    runAction(move_out);

    get_audio_helper()->play_effect("sounds/SlideClosed.mp3");
  }
  else
  {
    const auto fade_out_window = FadeTo::create(time, 0);
    const auto hide = Hide::create();
    const auto fade_hide = Sequence::create(fade_out_window, hide, call_back, nullptr);

    runAction(fade_hide);
  }
}

void basic_menu::set_default_menu_item(MenuItem* item)
{
  default_menu_item_ = item;
}

void basic_menu::select_menu_item(MenuItem* item)
{
#if (GAME_PLATFORM == DESKTOP_GAME)

  if (selection_ == nullptr)
  {
    selection_ = DrawNode::create(5);

    animate_selection(selection_);

    addChild(selection_);
  }

  if (selected_menu_item_ != item)
  {
    draw_selection(selection_, item, Color4F(0, 1, 1, 0.75));

    if (selected_menu_item_ != nullptr)
    {
      if (previous_selection_ == nullptr)
      {
        previous_selection_ = DrawNode::create(5);
        addChild(previous_selection_);
      }

      previous_selection_->stopAllActions();
      previous_selection_->setOpacity(127);
      const auto fade_out = FadeTo::create(0.5, 0);
      previous_selection_->runAction(fade_out);

      draw_selection(previous_selection_, selected_menu_item_, Color4F(0, 1, 1, 0.5));
    }
    selected_menu_item_ = item;
  }

#endif
}

void basic_menu::move_selection_left()
{
  if (!moving_)
  {
    if (!selection_locked_)
    {
      move_selection(compare_left_, distance_left_);
    }
    else if (is_selected_item_slider())
    {
      change_slider_value(-5);
    }
  }
}

void basic_menu::move_selection_right()
{
  if (!moving_)
  {
    if (!selection_locked_)
    {
      move_selection(compare_right_, distance_right_);
    }
    else if (is_selected_item_slider())
    {
      change_slider_value(5);
    }
  }
}

void basic_menu::move_selection_up()
{
  if (!moving_)
  {
    if (!selection_locked_)
    {
      move_selection(compare_up_, distance_up_);
    }
    else if (is_selected_item_slider())
    {
      change_slider_value(5);
    }
  }
}

void basic_menu::move_selection_down()
{
  if (!moving_)
  {
    if (!selection_locked_)
    {
      move_selection(compare_down_, distance_down_);
    }
    else if (is_selected_item_slider())
    {
      change_slider_value(-5);
    }
  }
}

void basic_menu::activate_selection()
{
  if (!moving_)
  {
    if (selected_menu_item_ != nullptr)
    {
      if (is_selected_item_slider())
      {
        if (!selection_locked_)
        {
          selection_locked_ = true;
          stop_selection_animation(selection_);
        }
      }
      else
      {
        selected_menu_item_->activate();
      }
    }
  }
}

void basic_menu::selection_back()
{
  if (!moving_)
  {
    if (selection_locked_)
    {
      selection_locked_ = false;
      animate_selection(selection_);
    }
    else
    {
      on_menu_back();
    }
  }
}

void basic_menu::move_text_button(MenuItem* item)
{
  item->setPosition(0, current_text_button_y_);

  static const auto button_gap_y = 150.f;
  const auto& size = item->getContentSize();
  current_text_button_y_ += (size.height + button_gap_y);
}

void basic_menu::add_button(MenuItem* item, const ccMenuCallback& callback)
{
  item->setCallback(callback);
  buttons_.pushBack(item);
}

text_button* basic_menu::add_text_button(const std::string& text, const ccMenuCallback& callback)
{
  text_button* result = nullptr;

  do
  {
    const auto item = text_button::create("08_Text_", text);

    move_text_button(item);
    add_button(item, callback);

    result = item;
  } while (false);

  return result;
}

text_toggle* basic_menu::add_toggle_text_button(const std::string& text, const ccMenuCallback& callback,
                                                const bool not_move /*= false*/)
{
  text_toggle* result = nullptr;

  do
  {
    const auto item = text_toggle::create("08_Text_", text);
    UTILS_BREAK_IF(item == nullptr);

    if (!not_move)
    {
      move_text_button(item);
    }
    add_button(item, callback);

    result = item;
  } while (false);

  return result;
}

text_toggle* basic_menu::add_small_toggle_text_button(const std::string& text, const ccMenuCallback& callback)
{
  text_toggle* result = nullptr;

  do
  {
    const auto item = text_toggle::create("02_joystick_empty_0", text);

    add_button(item, callback);

    result = item;
  } while (false);

  return result;
}

text_toggle* basic_menu::add_toggle_image_button(const std::string& image, const ccMenuCallback& callback)
{
  text_toggle* result = nullptr;

  do
  {
    const auto item = text_toggle::create(image, "");

    add_button(item, callback);

    result = item;
  } while (false);

  return result;
}

slider_object* basic_menu::add_slider(MenuItem* attach_to, const float_callback& callback)
{
  slider_object* result = nullptr;

  do
  {
    const auto slider = slider_object::create("15_slider_empty.png", "15_slider_full.png", callback);
    UTILS_BREAK_IF(slider == nullptr);

    slider->setColor(Color3B(0, 255, 255));

    const auto slider_position =
      Vec2(attach_to->getPosition() +
           Vec2(attach_to->getContentSize().width / 2 + slider->getContentSize().width / 2 + 110.f, 0));

    slider->setPosition(slider_position);

    add_button(slider, nullptr);

    result = slider;
  } while (false);

  return result;
}

MenuItem* basic_menu::add_row_label(const std::string& text, MenuItem* attach_to, const float left_space)
{
  MenuItem* result = nullptr;

  do
  {
    const auto label = Label::createWithTTF(text, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label == nullptr);

    label->setTextColor(Color4B(255, 255, 255, 255));
    label->enableOutline(Color4B(0, 0, 0, 255), 5);

    auto label_item = MenuItemLabel::create(label);
    UTILS_BREAK_IF(label_item == nullptr);

    label_item->setPosition(attach_to->getPosition() - Vec2(attach_to->getContentSize().width / 2 -
                                                              label->getContentSize().width / 2 + left_space,
                                                            -label->getContentSize().height / 4));

    add_button(label_item, nullptr);

    result = label_item;
  } while (false);

  return result;
}

void basic_menu::on_movement_end()
{
  moving_ = false;
}

void basic_menu::move_selection(const compare_function& compare_fun, const distance_function& distance_fun)
{
  if (selected_menu_item_ == nullptr)
  {
    return;
  }

  MenuItem* new_item = nullptr;
  auto min_distance = 1000000.f;
  for (const auto child : menu_->getChildren())
  {
    const auto label = dynamic_cast<MenuItemLabel*>(child);
    if (label != nullptr)
    {
      continue;
    }
    const auto item = dynamic_cast<MenuItem*>(child);
    if (item != nullptr)
    {
      if (item != selected_menu_item_)
      {
        if (item->isEnabled())
        {
          if (compare_fun(item))
          {
            const auto distance = distance_fun(item);
            if (distance < min_distance)
            {
              min_distance = distance;
              new_item = item;
            }
          }
        }
      }
    }
  }

  if (new_item != nullptr)
  {
    select_menu_item(new_item);
  }
}

void basic_menu::on_menu_back() const
{
  if (default_menu_item_ != nullptr)
  {
    default_menu_item_->activate();
  }
}

void basic_menu::draw_selection(DrawNode* draw, MenuItem* item, const Color4F& color) const
{
  draw->clear();

  const auto& item_size = item->getContentSize();
  const auto origin = item->getPosition() - (item_size / 2) - Vec2(0, getContentSize().height / 2);
  const auto destination = origin + item->getContentSize();
  const auto gap = Vec2(10, 10);

  draw->drawRect(origin - gap, destination + gap, color);
}

void basic_menu::animate_selection(DrawNode* draw) const
{
  const auto fade_out = FadeTo::create(0.5, 127);
  const auto fade_in = FadeTo::create(0.5, 255);
  const auto sequence = Sequence::create(fade_out, fade_in, nullptr);
  const auto repeat = RepeatForever::create(sequence);

  draw->runAction(repeat);
}

void basic_menu::stop_selection_animation(DrawNode* draw)
{
  draw->stopAllActions();
  draw->setOpacity(255);
}

bool basic_menu::is_selected_item_slider() const
{
  return dynamic_cast<slider_object*>(selected_menu_item_) != nullptr;
}

void basic_menu::change_slider_value(const float increase) const
{
  const auto slider = dynamic_cast<slider_object*>(selected_menu_item_);
  const auto value = slider->get_percentage();
  slider->set_percentage(value + increase);
}
