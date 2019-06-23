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

#include "TextButton.h"

TextButton::TextButton() : _label(nullptr) {}

TextButton* TextButton::create(const std::string& base, const std::string& text)
{
  TextButton* ret = nullptr;

  do
  {
    auto object = new TextButton();
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
  } while (false);

  return ret;
}

bool TextButton::init(const std::string& base, const std::string& text)
{
  auto ret = false;

  do
  {
    const auto sprite = Sprite::createWithSpriteFrameName(base + "1.png");
    UTILS_BREAK_IF(sprite == nullptr);
    sprite->setOpacity(190);

    const auto spriteClick = Sprite::createWithSpriteFrameName(base + "2.png");
    UTILS_BREAK_IF(spriteClick == nullptr);
    spriteClick->setOpacity(190);

    const auto spriteDisabled = Sprite::createWithSpriteFrameName(base + "4.png");
    UTILS_BREAK_IF(spriteDisabled == nullptr);
    spriteDisabled->setOpacity(190);

    UTILS_BREAK_IF(!BaseClass::initWithNormalSprite(sprite, spriteClick, spriteDisabled, nullptr));

    setContentSize(sprite->getContentSize());

    _label = Label::createWithTTF(text, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(_label == nullptr);

    _label->setPosition(getContentSize().width / 2, getContentSize().height / 2 + 30);
    _label->setTextColor(Color4B(255, 255, 255, 255));
    _label->enableOutline(Color4B(0, 0, 0, 255), 5);

    addChild(_label, 100);

    ret = true;
  } while (false);

  return ret;
}
