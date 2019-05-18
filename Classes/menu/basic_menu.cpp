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

MenuItem* basic_menu::create_image_button_base(const std::string& base) const
{
  MenuItem* result = nullptr;
  do
  {
    const auto sprite = Sprite::createWithSpriteFrameName(base + "_01.png");
    UTILS_BREAK_IF(sprite == nullptr);
    sprite->setOpacity(190);

    const auto sprite_click = Sprite::createWithSpriteFrameName(base + "_02.png");
    UTILS_BREAK_IF(sprite_click == nullptr);
    sprite_click->setOpacity(190);

    const auto sprite_disabled = Sprite::createWithSpriteFrameName(base + "_04.png");
    UTILS_BREAK_IF(sprite_disabled == nullptr);
    sprite_disabled->setOpacity(190);

    auto item = MenuItemSprite::create(sprite, sprite_click, sprite_disabled);
    UTILS_BREAK_IF(item == nullptr);

    item->setContentSize(sprite->getContentSize());

    result = item;
  }
  while (false);

  return result;
}

MenuItem* basic_menu::create_text_button_base() const
{
  MenuItem* result = nullptr;
  do
  {
    const auto sprite = Sprite::createWithSpriteFrameName("08_Text_1.png");
    UTILS_BREAK_IF(sprite == nullptr);
    sprite->setOpacity(190);

    const auto sprite_click = Sprite::createWithSpriteFrameName("08_Text_2.png");
    UTILS_BREAK_IF(sprite_click == nullptr);
    sprite_click->setOpacity(190);

    auto item = MenuItemSprite::create(sprite, sprite_click);
    UTILS_BREAK_IF(item == nullptr);

    item->setPosition(0, current_text_button_y_);
    item->setContentSize(sprite->getContentSize());

    result = item;
  }
  while (false);

  return result;
}

MenuItemToggle* basic_menu::create_toggle_button_base() const
{
  MenuItemToggle* result = nullptr;
  do
  {
    const auto normal_sprite = Sprite::createWithSpriteFrameName("08_Text_1.png");
    UTILS_BREAK_IF(normal_sprite == nullptr);
    normal_sprite->setOpacity(190);

    const auto sprite_click = Sprite::createWithSpriteFrameName("08_Text_2.png");
    UTILS_BREAK_IF(sprite_click == nullptr);
    sprite_click->setOpacity(190);

    const auto normal_item = MenuItemSprite::create(normal_sprite, sprite_click);
    UTILS_BREAK_IF(normal_item == nullptr);

    const auto selected_sprite = Sprite::createWithSpriteFrameName("08_Text_3.png");
    UTILS_BREAK_IF(selected_sprite == nullptr);
    selected_sprite->setOpacity(190);

    const auto click_selected_sprite = Sprite::createWithSpriteFrameName("08_Text_2.png");
    UTILS_BREAK_IF(click_selected_sprite == nullptr);
    click_selected_sprite->setOpacity(190);

    const auto selected_item = MenuItemSprite::create(selected_sprite, click_selected_sprite);
    UTILS_BREAK_IF(selected_item == nullptr);

    const auto item_toggle = MenuItemToggle::create(normal_item);
    UTILS_BREAK_IF(item_toggle == nullptr);

    item_toggle->addSubItem(selected_item);

    item_toggle->setPosition(0, current_text_button_y_);
    item_toggle->setContentSize(normal_sprite->getContentSize());

    result = item_toggle;
  }
  while (false);

  return result;
}

Label* basic_menu::add_label(const std::string& text, MenuItem* item) const
{
  Label* result = nullptr;

  do
  {
    const auto label_button = Label::createWithTTF(text, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label_button == nullptr);

    label_button->setPosition(item->getContentSize().width / 2,
                              item->getContentSize().height / 2 + 30);
    label_button->setTextColor(Color4B(255, 255, 255, 255));
    label_button->enableOutline(Color4B(0, 0, 0, 255), 5);

    item->addChild(label_button, 100);

    result = label_button;
  }
  while (false);

  return result;
}

MenuItem* basic_menu::add_text_button(const std::string& text, const ccMenuCallback& callback)
{
  MenuItem* result = nullptr;

  do
  {
    const auto item = create_text_button_base();
    UTILS_BREAK_IF(item == nullptr);

    const auto label = add_label(text, item);
    UTILS_BREAK_IF(label == nullptr);

    move_text_button(item);
    add_button(item, callback);

    result = item;
  }
  while (false);

  return result;
}

MenuItemToggle* basic_menu::add_toggle_text_button(const std::string& text, const ccMenuCallback& callback)
{
  MenuItemToggle* result = nullptr;

  do
  {
    const auto item = create_toggle_button_base();
    UTILS_BREAK_IF(item == nullptr);

    const auto label = add_label(text, item);
    UTILS_BREAK_IF(label == nullptr);

    move_text_button(item);
    add_button(item, callback);

    result = item;
  }
  while (false);

  return result;
}

MenuItem* basic_menu::add_image_button(const std::string& base_image, const std::string& text,
                                       const ccMenuCallback& callback)
{
  MenuItem* result = nullptr;

  do
  {
    const auto item = create_image_button_base(base_image);
    UTILS_BREAK_IF(item == nullptr);

    const auto label = add_label(text, item);
    UTILS_BREAK_IF(label == nullptr);

    move_image_button(item);
    add_button(item, callback);

    result = item;
  }
  while (false);

  return result;
}
