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

#include "menu_scene.h"
#include "../laser_and_bots_app.h"

Scene* menu_scene::scene(basic_app* application)
{
  menu_scene* ret = nullptr;

  do
  {
    auto object = new menu_scene();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(application))
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

menu_scene::menu_scene()
{
}

menu_scene::~menu_scene()
{
  base_class::removeAllChildrenWithCleanup(true);
}

bool menu_scene::init(basic_app* application)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init(application));

    const auto size = Director::getInstance()->getWinSize();

    const auto background = LayerGradient::create(Color4B(0, 0, 0, 255), Color4B(0, 0, 127, 255));
    UTILS_BREAK_IF(background == nullptr);

    addChild(background, 0);

    auto label = Label::createWithTTF("Laser and Bots", "fonts/tahoma.ttf", 500);
    UTILS_BREAK_IF(label == nullptr);

    label->setPosition(Vec2(size.width / 2, size.height - 500));
    label->setTextColor(Color4B(0, 255, 255, 255));

    label->enableGlow(Color4B(0, 127, 127, 127));
    label->enableShadow(Color4B(255, 255, 255, 127), Size(10, -10));
    label->enableOutline(Color4B(255, 255, 255, 255), 10);

    addChild(label, 0);

    auto play = Label::createWithTTF("Play", "fonts/tahoma.ttf", 300);
    UTILS_BREAK_IF(play == nullptr);

    play->setTextColor(Color4B(0, 255, 255, 255));

    play->enableGlow(Color4B(0, 127, 127, 127));
    play->enableShadow(Color4B(255, 255, 255, 127), Size(10, -10));
    play->enableOutline(Color4B(255, 255, 255, 255), 10);

    const auto play_item = MenuItemLabel::create(play, CC_CALLBACK_0(menu_scene::to_game, this));
    UTILS_BREAK_IF(play_item == nullptr);

    play_item->setPosition(Vec2(0, -(size.height / 2) + 500));

    const auto menu = Menu::create(play_item, nullptr);
    UTILS_BREAK_IF(menu == nullptr);

    addChild(menu, 0);

    ret = true;
  }
  while (false);

  return ret;
}

void menu_scene::to_game() const
{
  auto app = dynamic_cast<laser_and_bots_app*>(get_application());
  app->to_game();
}
