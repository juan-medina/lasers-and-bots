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

#include "about_menu.h"
#include "../scenes/menu_scene.h"
#include "../ui/text_button.h"
#include "../utils/audio/audio_helper.h"
#include "ui/UIRichText.h"

using namespace cocos2d::ui;

about_menu::about_menu() : back_item_(nullptr) {}

about_menu* about_menu::create(audio_helper* audio_helper)
{
  about_menu* ret = nullptr;

  do
  {
    auto object = new about_menu();
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
  } while (false);

  return ret;
}

bool about_menu::init(audio_helper* audio_helper)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init("About", audio_helper, 4700.f, 3400.f));

    ret = true;
  } while (false);

  return ret;
}

bool about_menu::create_menu_items()
{
  auto result = false;
  do
  {
    back_item_ = add_text_button("Back", CC_CALLBACK_0(about_menu::on_back, this));
    UTILS_BREAK_IF(back_item_ == nullptr);

    ValueMap defaults{};
    defaults.insert(std::make_pair(RichText::KEY_FONT_FACE, Value("fonts/tahoma.ttf")));
    defaults.insert(std::make_pair(RichText::KEY_FONT_SIZE, Value(120)));
    defaults.insert(std::make_pair(RichText::KEY_FONT_COLOR_STRING, Value("#FFFFFF")));
    defaults.insert(std::make_pair(RichText::KEY_ANCHOR_FONT_COLOR_STRING, Value("#00FFFF")));
    defaults.insert(std::make_pair(RichText::KEY_HORIZONTAL_ALIGNMENT,
                                   Value(static_cast<int>(RichText::HorizontalAlignment::LEFT))));
    const auto file_utils = FileUtils::getInstance();
    const auto text = file_utils->getStringFromFile("credits/about.xml");

    const auto rich_text = RichText::createWithXML(text, defaults);
    UTILS_BREAK_IF(rich_text == nullptr);

    rich_text->ignoreContentAdaptWithSize(false);
    rich_text->setContentSize(Size(getContentSize().width * .925f, getContentSize().height * .85f));
    rich_text->formatText();
    rich_text->setPosition(Vec2(0, 70));

    addChild(rich_text);

    set_default_menu_item(back_item_);

    result = true;
  } while (false);
  return result;
}

void about_menu::on_back()
{
  get_audio_helper()->play_effect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_main_menu();
}
