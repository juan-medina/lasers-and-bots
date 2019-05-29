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

#include "text_toggle.h"

text_toggle::text_toggle():
  label_(nullptr)
{
}

text_toggle* text_toggle::create(const std::string& base, const std::string& text)
{
  text_toggle* ret = nullptr;

  do
  {
    auto object = new text_toggle();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(base, text))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  }
  while (false);

  return ret;
}

bool text_toggle::init(const std::string& base, const std::string& text)
{
  auto ret = false;

  do
  {
    const auto normal_sprite = Sprite::createWithSpriteFrameName(base + "1.png");
    UTILS_BREAK_IF(normal_sprite == nullptr);
    normal_sprite->setOpacity(190);

    const auto sprite_click = Sprite::createWithSpriteFrameName(base + "2.png");
    UTILS_BREAK_IF(sprite_click == nullptr);
    sprite_click->setOpacity(190);

    const auto sprite_click_disabled = Sprite::createWithSpriteFrameName(base + "4.png");
    UTILS_BREAK_IF(sprite_click_disabled == nullptr);
    sprite_click_disabled->setOpacity(190);

    const auto normal_item = MenuItemSprite::create(normal_sprite, sprite_click, sprite_click_disabled);
    UTILS_BREAK_IF(normal_item == nullptr);

    const auto selected_sprite = Sprite::createWithSpriteFrameName(base + "3.png");
    UTILS_BREAK_IF(selected_sprite == nullptr);
    selected_sprite->setOpacity(190);

    const auto click_selected_sprite = Sprite::createWithSpriteFrameName(base + "2.png");
    UTILS_BREAK_IF(click_selected_sprite == nullptr);
    click_selected_sprite->setOpacity(190);

    const auto selected_item = MenuItemSprite::create(selected_sprite, click_selected_sprite);
    UTILS_BREAK_IF(selected_item == nullptr);

    UTILS_BREAK_IF(!base_class::initWithItem(normal_item));

    addSubItem(selected_item);

    setContentSize(normal_sprite->getContentSize());

    label_ = Label::createWithTTF(text, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label_ == nullptr);

    label_->setPosition(getContentSize().width / 2, getContentSize().height / 2 + 30);
    label_->setTextColor(Color4B(255, 255, 255, 255));
    label_->enableOutline(Color4B(0, 0, 0, 255), 5);

    addChild(label_, 100);

    ret = true;
  }
  while (false);

  return ret;
}
