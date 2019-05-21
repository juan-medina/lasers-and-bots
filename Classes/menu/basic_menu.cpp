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

#include "basic_menu.h"
#include "../utils/audio/audio_helper.h"
#include "../ui/slider_object.h"
#include "../ui/text_button.h"
#include "../ui/text_toggle.h"

basic_menu::basic_menu():
  audio_helper_(nullptr),
  current_text_button_y_(0),
  current_image_button_x_(0),
  current_image_button_y_(0),
  image_button_start_x_(0)
{
}

bool basic_menu::init(const std::string& name, audio_helper* audio_helper, const float width, const float height)
{
  auto ret = false;

  do
  {
    audio_helper_ = audio_helper;

    /*const auto desired_width = horizontal ? 1800.f : 1300.f;
    const auto desired_height = horizontal ? 1300.f : 1300.f;*/

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

    const auto menu = Menu::createWithArray(buttons_);
    UTILS_BREAK_IF(menu == nullptr);

    menu->setPosition(0.f, -getContentSize().height / 2);

    addChild(menu, 100);

    setOpacity(0);
    setVisible(false);

    ret = true;
  }
  while (false);

  return ret;
}

void basic_menu::display()
{
  static auto const time = 0.5f;
  const auto& size = Director::getInstance()->getVisibleSize();

  setVisible(true);
  setOpacity(190);
  const auto move = Vec2(size.width, 0);
  setPosition(Vec2(size.width / 2, size.height / 2) - move);

  const auto elastic_in = EaseElasticInOut::create(MoveBy::create(time * 2, move), time);
  const auto move_in = Sequence::create(elastic_in, nullptr);

  runAction(move_in);

  get_audio_helper()->play_effect("sounds/SlideClosed.mp3");
}

void basic_menu::hide()
{
  static auto const time = 0.5f;
  const auto& size = Director::getInstance()->getVisibleSize();
  const auto move = Vec2(size.width, 0);


  const auto elastic_out = EaseElasticInOut::create(MoveBy::create(time * 2, move), time);
  const auto fade = FadeTo::create(0.0f, 0);
  const auto hide = Hide::create();

  const auto move_out = Sequence::create(elastic_out, fade, hide, nullptr);

  runAction(move_out);
  get_audio_helper()->play_effect("sounds/SlideClosed.mp3");
}

void basic_menu::move_image_button(MenuItem* item)
{
  item->setPosition(current_image_button_x_, current_image_button_y_);

  static const auto button_gap_x = 45.f;
  static const auto button_gap_y = 50.f;

  const auto& size = item->getContentSize();
  current_image_button_x_ += (size.width + button_gap_x);
  static auto button_count = 0;
  button_count++;
  if (button_count > 4)
  {
    current_image_button_y_ -= (size.height + button_gap_y);
    current_image_button_x_ = image_button_start_x_;
    button_count = 0;
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
  }
  while (false);

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
  }
  while (false);

  return result;
}

text_toggle* basic_menu::add_small_button(const std::string& text, const ccMenuCallback& callback)
{
  text_toggle* result = nullptr;

  do
  {
    const auto item = text_toggle::create("02_joystick_empty_0", text);

    add_button(item, callback);

    result = item;
  }
  while (false);

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

    const auto slider_position = Vec2(attach_to->getPosition() +
      Vec2(attach_to->getContentSize().width / 2 + slider->getContentSize().width / 2 + 110.f, 0));

    slider->setPosition(slider_position);

    add_button(slider, nullptr);

    result = slider;
  }
  while (false);

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

    label_item->setPosition(attach_to->getPosition() -
      Vec2(attach_to->getContentSize().width / 2 - label->getContentSize().width / 2 + left_space,
           -label->getContentSize().height / 4));

    add_button(label_item, nullptr);

    result = label_item;
  }
  while (false);

  return result;
}
