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

#include "ScrollingText.h"
#include "ui/UIRichText.h"

using namespace cocos2d::ui;

ScrollingText::ScrollingText()
  : _richText(nullptr), _autoScroll(false), _autoScrollDirection{1.f}, _scrollPos(0.f)
{
}

ScrollingText* ScrollingText::create(const Size& size, const std::string textFile, const bool centered)
{
  ScrollingText* ret = nullptr;

  do
  {
    auto object = new ScrollingText();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(size, textFile, centered))
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

bool ScrollingText::init(const Size& size, const std::string textFile, const bool centered /* = false*/)
{
  auto ret = false;

  do
  {
    setContentSize(size);

    const auto stencil = LayerColor::create(Color4B::WHITE);
    UTILS_BREAK_IF(stencil == nullptr);
    stencil->setContentSize(size);

    const auto clip = ClippingNode::create(stencil);
    UTILS_BREAK_IF(clip == nullptr);
    addChild(clip);

    ValueMap defaults{};
    defaults.insert(std::make_pair(RichText::KEY_FONT_FACE, Value("fonts/tahoma.ttf")));
    defaults.insert(std::make_pair(RichText::KEY_FONT_SIZE, Value(150)));
    defaults.insert(std::make_pair(RichText::KEY_FONT_BIG, Value(200)));
    defaults.insert(std::make_pair(RichText::KEY_FONT_COLOR_STRING, Value("#FFFFFF")));
    defaults.insert(std::make_pair(RichText::KEY_ANCHOR_FONT_COLOR_STRING, Value("#00FFFF")));
    if (centered)
    {
      defaults.insert(std::make_pair(RichText::KEY_HORIZONTAL_ALIGNMENT,
                                     Value(static_cast<int>(RichText::HorizontalAlignment::CENTER))));
    }
    else
    {
      defaults.insert(std::make_pair(RichText::KEY_HORIZONTAL_ALIGNMENT,
                                     Value(static_cast<int>(RichText::HorizontalAlignment::LEFT))));
    }

    const auto fileUtils = FileUtils::getInstance();
    const auto text = fileUtils->getStringFromFile(textFile);

    _richText = RichText::createWithXML(text, defaults);
    UTILS_BREAK_IF(_richText == nullptr);

    _richText->ignoreContentAdaptWithSize(false);
    _richText->setContentSize(size);
    _richText->formatText();
    _richText->setAnchorPoint(Vec2(0, 1));
    _richText->setPosition(Vec2(0, size.height));

    clip->addChild(_richText);

    scheduleUpdate();

    ret = true;
  } while (false);

  return ret;
}

void ScrollingText::update(float delta)
{
  BaseClass::update(delta);
  _richText->setPosition(Vec2(0, getContentSize().height + _scrollPos));

  if (_autoScroll)
  {
    _scrollPos += ((delta * 100) * _autoScrollDirection);
    if (_scrollPos > (_richText->getContentSize().height - getContentSize().height))
    {
      _autoScroll = false;
    }
  }
}

void ScrollingText::setScroll(const float scroll)
{
  _scrollPos = scroll;
}

void ScrollingText::autoScrollIn(const float time)
{
  setScroll(0.f);
  _autoScroll = false;

  const auto start = CallFunc::create(CC_CALLBACK_0(ScrollingText::startScroll, this));
  const auto delay = DelayTime::create(time);
  const auto delayStart = Sequence::create(delay, start, nullptr);

  runAction(delayStart);
}

void ScrollingText::startScroll()
{
  _autoScroll = true;
}
