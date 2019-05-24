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

#include "credits_menu.h"
#include "../utils/audio/audio_helper.h"
#include "../scenes/menu_scene.h"
#include "ui/UIRichText.h"

using namespace cocos2d::ui;

credits_menu* credits_menu::create(audio_helper* audio_helper)
{
  credits_menu* ret = nullptr;

  do
  {
    auto object = new credits_menu();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audio_helper))
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

bool credits_menu::init(audio_helper* audio_helper)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init("Credits", audio_helper, 4000.f, 3700.f));

    ret = true;
  }
  while (false);

  return ret;
}

bool credits_menu::create_menu_items()
{
  auto result = false;
  do
  {
    UTILS_BREAK_IF(add_text_button("Back", CC_CALLBACK_0(credits_menu::on_back, this)) == nullptr);

    ValueMap defaults{};
    defaults.insert(std::make_pair(RichText::KEY_FONT_FACE, Value("fonts/tahoma.ttf")));
    defaults.insert(std::make_pair(RichText::KEY_FONT_SIZE, Value(105)));
    defaults.insert(std::make_pair(RichText::KEY_FONT_COLOR_STRING, Value("#FFFFFF")));
    defaults.insert(std::make_pair(RichText::KEY_ANCHOR_FONT_COLOR_STRING, Value("#00FFFF")));
    defaults.insert(std::make_pair(RichText::KEY_ANCHOR_TEXT_LINE, Value(RichText::VALUE_TEXT_LINE_UNDER)));
    defaults.insert(std::make_pair(RichText::KEY_HORIZONTAL_ALIGNMENT,
                                   Value(static_cast<int>(RichText::HorizontalAlignment::CENTER))));
    const auto file_utils = FileUtils::getInstance();
    const auto text = file_utils->getStringFromFile("credits/credits.xml");

    const auto rich_text = RichText::createWithXML(text, defaults);
    UTILS_BREAK_IF(rich_text == nullptr);

    rich_text->ignoreContentAdaptWithSize(false);
    rich_text->setContentSize(getContentSize() * .85f);
    rich_text->formatText();
    rich_text->setPosition(Vec2(0, 0));

    addChild(rich_text);

    result = true;
  }
  while (false);
  return result;
}

void credits_menu::on_back()
{
  get_audio_helper()->play_effect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_main_menu();
}
