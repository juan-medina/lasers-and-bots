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

#include "scrolling_text.h"
#include "ui/UIRichText.h"

using namespace cocos2d::ui;

scrolling_text::scrolling_text()
  : rich_text_(nullptr), auto_scroll_(false), auto_scroll_direction_{1.f}, scroll_pos_(0.f)
{
}

scrolling_text* scrolling_text::create(const Size& size, const std::string text_file, const bool centered)
{
  scrolling_text* ret = nullptr;

  do
  {
    auto object = new scrolling_text();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(size, text_file, centered))
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

bool scrolling_text::init(const Size& size, const std::string text_file, const bool centered /* = false*/)
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

    const auto file_utils = FileUtils::getInstance();
    const auto text = file_utils->getStringFromFile(text_file);

    rich_text_ = RichText::createWithXML(text, defaults);
    UTILS_BREAK_IF(rich_text_ == nullptr);

    rich_text_->ignoreContentAdaptWithSize(false);
    rich_text_->setContentSize(size);
    rich_text_->formatText();
    rich_text_->setAnchorPoint(Vec2(0, 1));
    rich_text_->setPosition(Vec2(0, size.height));

    clip->addChild(rich_text_);

    scheduleUpdate();

    ret = true;
  } while (false);

  return ret;
}

void scrolling_text::update(float delta)
{
  base_class::update(delta);
  rich_text_->setPosition(Vec2(0, getContentSize().height + scroll_pos_));

  if (auto_scroll_)
  {
    scroll_pos_ += ((delta * 100) * auto_scroll_direction_);
    if (scroll_pos_ > (rich_text_->getContentSize().height - getContentSize().height))
    {
      auto_scroll_ = false;
    }
  }
}

void scrolling_text::set_scroll(const float scroll)
{
  scroll_pos_ = scroll;
}

void scrolling_text::auto_scroll_in(const float time)
{
  set_scroll(0.f);
  auto_scroll_ = false;

  const auto start = CallFunc::create(CC_CALLBACK_0(scrolling_text::start_scroll, this));
  const auto delay = DelayTime::create(time);
  const auto delay_start = Sequence::create(delay, start, nullptr);

  runAction(delay_start);
}

void scrolling_text::start_scroll()
{
  auto_scroll_ = true;
}
