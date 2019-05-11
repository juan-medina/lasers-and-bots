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
  current_text_button_y_(0)
{
}

bool basic_menu::init(const std::string& name, audio_helper* audio_helper)
{
  auto ret = false;

  do
  {
    audio_helper_ = audio_helper;
    UTILS_BREAK_IF(!base_class::init());

    audio_helper_->pre_load_effect("sounds/select.mp3");

    const auto& size = Director::getInstance()->getVisibleSize();

    const auto background = Sprite::createWithSpriteFrameName("10_message.png");
    UTILS_BREAK_IF(background == nullptr);

    background->setRotation(-90);
    background->setFlippedX(true);
    setCascadeOpacityEnabled(true);
    background->setCascadeOpacityEnabled(true);
    background->setPosition(size.width / 2, size.height / 2);
    background->setColor(Color3B(0, 255, 255));

    addChild(background, 100);

    const auto horizontal_segment = background->getContentSize().height;
    const auto vertical_segment = background->getContentSize().width;

    UTILS_BREAK_IF(!create_menu_items());

    const auto menu = Menu::createWithArray(buttons_);
    UTILS_BREAK_IF(menu == nullptr);

    static const auto gap_menu_y = 150.f;

    const auto first_item = buttons_.front();

    menu->setPosition(size.width / 2,
                      (size.height / 2) - (vertical_segment / 2) + (first_item->getContentSize().height / 2) +
                      gap_menu_y);

    addChild(menu, 100);

    const auto header = Sprite::createWithSpriteFrameName("11_message_header.png");
    UTILS_BREAK_IF(header == nullptr);

    header->setPosition(size.width / 2,
                        (size.height / 2) + (vertical_segment / 2) - (header->getContentSize().height / 4));
    header->setColor(Color3B(0, 127, 127));
    header->setOpacity(255);
    header->setCascadeOpacityEnabled(true);

    addChild(header, 100);

    const auto label = Label::createWithTTF(name, "fonts/tahoma.ttf", 150);
    UTILS_BREAK_IF(label == nullptr);

    label->setTextColor(Color4B(0, 255, 255, 255));
    label->enableOutline(Color4B(0, 0, 0, 255), 5);
    label->enableShadow(Color4B(255, 255, 255, 127), Size(5, -5));

    label->setPosition(header->getContentSize().width / 2, (header->getContentSize().height / 2) + 100);

    header->addChild(label, 100);

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
  setPosition(Vec2(-size.width, 0));

  const auto elastic_in = EaseElasticInOut::create(MoveBy::create(time * 2, Vec2(size.width, 0)), time);
  const auto move_in = Sequence::create(elastic_in, nullptr);

  runAction(move_in);

  get_audio_helper()->play_effect("sounds/SlideClosed.mp3");
}

void basic_menu::hide()
{
  static auto const time = 0.5f;
  const auto& size = Director::getInstance()->getVisibleSize();

  const auto elastic_out = EaseElasticInOut::create(MoveBy::create(time * 2, Vec2(size.width, 0)), time);
  const auto fade = FadeTo::create(0.0f, 0);
  const auto hide = Hide::create();

  const auto move_out = Sequence::create(elastic_out, fade, hide, nullptr);

  runAction(move_out);
  get_audio_helper()->play_effect("sounds/SlideClosed.mp3");
}

void basic_menu::add_button(MenuItem* item, const ccMenuCallback& callback)
{
  item->setCallback(callback);
  buttons_.pushBack(item);

  static const auto button_gap_y = 150.f;
  const auto& size = item->getContentSize();
  current_text_button_y_ += (size.height + button_gap_y);
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

    add_button(item, callback);

    result = item;
  }
  while (false);

  return result;
}
