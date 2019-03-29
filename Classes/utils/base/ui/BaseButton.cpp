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

#include "BaseButton.h"

BaseButton::BaseButton()
{
}

BaseButton::~BaseButton()
{
}

BaseButton* BaseButton::create(Sprite* sprite, const ccMenuCallback& selector)
{
  BaseButton* ret = nullptr;

  do
  {
    auto obj = new BaseButton();
    UTILS_BREAK_IF(obj == nullptr);

    if (obj->init(sprite, selector))
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

BaseButton* BaseButton::createWithSpriteFrameName(const string& spriteFrameName, const ccMenuCallback& selector)
{
  BaseButton* ret = nullptr;

  do
  {
    auto sprite = Sprite::createWithSpriteFrameName(spriteFrameName);
    UTILS_BREAK_IF(sprite == nullptr);

    auto obj = BaseButton::create(sprite, selector);
    UTILS_BREAK_IF(obj == nullptr);

    ret = obj;

  } while (0);

  // return the object
  return ret;
}

// on "init" you need to initialize your instance
bool BaseButton::init(Sprite* sprite, const ccMenuCallback& selector)
{
  bool ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first

    ret = parent::initWithLabel(sprite, selector);
    UTILS_BREAK_IF(!ret);

  } while (0);

  return ret;
}

void BaseButton::setEnabled(bool value)
{
  parent::setEnabled(value);

  if (value)
  {
    this->setOpacity(255);
  }
  else
  {
    this->setOpacity(64);
  }
}
