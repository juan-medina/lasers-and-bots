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

#include "options_menu.h"
#include "../utils/audio/audio_helper.h"
#include "scenes/menu_scene.h"

options_menu* options_menu::create(audio_helper* audio_helper)
{
  options_menu* ret = nullptr;

  do
  {
    auto object = new options_menu();
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

bool options_menu::init(audio_helper* audio_helper)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init("Options", audio_helper));

    ret = true;
  }
  while (false);

  return ret;
}

bool options_menu::create_menu_items()
{
  auto result = false;
  do
  {
    UTILS_BREAK_IF(!add_text_button("Back", CC_CALLBACK_0(options_menu::on_back, this)));

    result = true;
  }
  while (false);
  return result;
}

void options_menu::on_back()
{
  get_audio_helper()->play_effect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<menu_scene*>(getParent());
  menu->display_main_menu();
}
