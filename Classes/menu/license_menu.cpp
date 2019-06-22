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

#include "license_menu.h"
#include "../scenes/menu_scene.h"
#include "../ui/scrolling_text.h"
#include "../ui/text_button.h"
#include "../utils/audio/AudioHelper.h"

license_menu::license_menu() : back_item_(nullptr), scrolling_text_(nullptr) {}

license_menu* license_menu::create(AudioHelper* audio_helper)
{
  license_menu* ret = nullptr;

  do
  {
    auto object = new license_menu();
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

bool license_menu::init(AudioHelper* audio_helper)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init("License", audio_helper, 5100.f, 3000.f));

    ret = true;
  } while (false);

  return ret;
}

bool license_menu::create_menu_items()
{
  auto result = false;
  do
  {
    back_item_ = add_text_button("Accept", CC_CALLBACK_0(license_menu::on_back, this));
    UTILS_BREAK_IF(back_item_ == nullptr);
    set_default_menu_item(back_item_);

    const auto scrolling_size = Size(getContentSize().width - 310, getContentSize().height - 440);
    scrolling_text_ = scrolling_text::create(scrolling_size, "credits/license.xml");
    UTILS_BREAK_IF(scrolling_text_ == nullptr);
    addChild(scrolling_text_);

    auto text_position = Vec2(-getContentSize().width / 2, -getContentSize().height / 2) + Vec2(130, 250);
    scrolling_text_->setPosition(text_position);

    result = true;

  } while (false);
  return result;
}

void license_menu::on_back()
{
  get_audio_helper()->playEffect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_main_menu();
}

void license_menu::display()
{
  base_class ::display();
  scrolling_text_->auto_scroll_in(5.f);
}