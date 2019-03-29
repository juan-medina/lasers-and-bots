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

#include "TextButton.h"

TextButton::TextButton()
{
}

TextButton::~TextButton()
{
}

TextButton* TextButton::create(Sprite* sprite, Label* label, const ccMenuCallback& selector)
{
  TextButton* ret = nullptr;

  do
  {
    auto obj = new TextButton();
    UTILS_BREAK_IF(obj == nullptr);

    if (obj->init(sprite, label, selector))
    {
      obj->autorelease();
    }
    else
    {
      delete obj;
    }

    ret = obj;
  } while (0);

  // return the object
  return ret;
}

TextButton* TextButton::createWithSpriteFrameName(const string& spriteFrameName, Label* label, const ccMenuCallback& selector)
{
  TextButton* ret = nullptr;

  do
  {
    auto sprite = Sprite::createWithSpriteFrameName(spriteFrameName);
    UTILS_BREAK_IF(sprite == nullptr);

    auto obj = TextButton::create(sprite, label, selector);
    UTILS_BREAK_IF(obj == nullptr);

    ret = obj;

  } while (0);

  // return the object
  return ret;
}

TextButton* TextButton::createWithSpriteFrameNameAndBMFont(const string& spriteFrameName, const string& bmfontPath, const string& text, const TextHAlignment& hAlignment, const ccMenuCallback& selector)
{
  TextButton* ret = nullptr;

  do
  {
    auto label = Label::createWithBMFont(bmfontPath, text, hAlignment);
    UTILS_BREAK_IF(label == nullptr);

    auto obj = TextButton::createWithSpriteFrameName(spriteFrameName, label, selector);
    UTILS_BREAK_IF(obj == nullptr);

    ret = obj;

  } while (0);

  // return the object
  return ret;
}

// on "init" you need to initialize your instance
bool TextButton::init(Sprite* sprite, Label* label, const ccMenuCallback& selector)
{
  bool ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first

    ret = parent::init(sprite, selector);
    UTILS_BREAK_IF(!ret);

    _label = label;

    _label->setPosition(Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2));

    this->addChild(_label);

  } while (0);

  return ret;
}

void TextButton::setEnabled(bool value)
{
  parent::setEnabled(value);

  if (value)
  {
    this->setOpacity(255);
    _label->setOpacity(255);
  }
  else
  {
    this->setOpacity(64);
    _label->setOpacity(64);
  }
}

void TextButton::setString(const string& string)
{
  _label->setString(string);
}
