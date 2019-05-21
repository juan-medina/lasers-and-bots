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

#include "text_button.h"

text_button::text_button():
  label_(nullptr)
{
}

text_button* text_button::create(const std::string& base, const std::string& text)
{
  text_button* ret = nullptr;

  do
  {
    auto object = new text_button();
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

bool text_button::init(const std::string& base, const std::string& text)
{
  auto ret = false;

  do
  {
    const auto sprite = Sprite::createWithSpriteFrameName(base + "1.png");
    UTILS_BREAK_IF(sprite == nullptr);
    sprite->setOpacity(190);

    const auto sprite_click = Sprite::createWithSpriteFrameName(base + "2.png");
    UTILS_BREAK_IF(sprite_click == nullptr);
    sprite_click->setOpacity(190);

    const auto sprite_disabled = Sprite::createWithSpriteFrameName(base + "4.png");
    UTILS_BREAK_IF(sprite_disabled == nullptr);
    sprite_disabled->setOpacity(190);

    UTILS_BREAK_IF(!base_class::initWithNormalSprite(sprite, sprite_click, sprite_disabled, nullptr));

    setContentSize(sprite->getContentSize());

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
