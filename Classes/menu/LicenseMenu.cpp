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

#include "LicenseMenu.h"

#include "../ui/ScrollingText.h"
#include "../ui/TextButton.h"
#include "../utils/audio/AudioHelper.h"
#include "scenes/MenuScene.h"

LicenseMenu::LicenseMenu() : _backItem(nullptr), _scrollingText(nullptr) {}

LicenseMenu* LicenseMenu::create(AudioHelper* audioHelper)
{
  LicenseMenu* ret = nullptr;

  do
  {
    auto object = new LicenseMenu();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audioHelper))
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

bool LicenseMenu::init(AudioHelper* audioHelper)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init("License", audioHelper, 5100.f, 3000.f));

    ret = true;
  } while (false);

  return ret;
}

bool LicenseMenu::createMenuItems()
{
  auto result = false;
  do
  {
    _backItem = addTextButton("Accept", CC_CALLBACK_0(LicenseMenu::onBack, this));
    UTILS_BREAK_IF(_backItem == nullptr);
    setDefaultMenuItem(_backItem);

    const auto scrollingSize = Size(getContentSize().width - 310, getContentSize().height - 440);
    _scrollingText = ScrollingText::create(scrollingSize, "credits/license.xml");
    UTILS_BREAK_IF(_scrollingText == nullptr);
    addChild(_scrollingText);

    const auto textPosition =
      Vec2(-getContentSize().width / 2, -getContentSize().height / 2) + Vec2(130, 250);
    _scrollingText->setPosition(textPosition);

    result = true;

  } while (false);
  return result;
}

void LicenseMenu::onBack()
{
  getAudioHelper()->playEffect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  menu->displayMainMenu();
}

void LicenseMenu::display()
{
  BaseClass::display();
  _scrollingText->autoScrollIn(5.f);
}
