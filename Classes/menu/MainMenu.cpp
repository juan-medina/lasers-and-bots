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

#include "MainMenu.h"

#include "../ui/TextButton.h"
#include "../utils/audio/AudioHelper.h"
#include "scenes/MenuScene.h"

MainMenu::MainMenu() : _backItem(nullptr), _playItem(nullptr) {}

MainMenu* MainMenu::create(AudioHelper* audioHelper)
{
  MainMenu* ret = nullptr;

  do
  {
    auto object = new MainMenu();
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

bool MainMenu::init(AudioHelper* audioHelper)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init("Main Menu", audioHelper, 1300.f, 2200.f));

    ret = true;
  } while (false);

  return ret;
}

void MainMenu::display()
{
  const auto needSelectPlay = getSelectedMenuItem() == nullptr;
  BaseClass::display();

  if (needSelectPlay)
  {
    selectMenuItem(_playItem);
  }
}

bool MainMenu::createMenuItems()
{
  auto result = false;
  do
  {
    _backItem = addTextButton("Exit", CC_CALLBACK_0(MainMenu::onExitClick, this));
    UTILS_BREAK_IF(_backItem == nullptr);

    UTILS_BREAK_IF(addTextButton("Credits", CC_CALLBACK_0(MainMenu::onCredits, this)) == nullptr);
    UTILS_BREAK_IF(addTextButton("About", CC_CALLBACK_0(MainMenu::onAbout, this)) == nullptr);
    UTILS_BREAK_IF(addTextButton("Options", CC_CALLBACK_0(MainMenu::onOptions, this)) == nullptr);

    _playItem = addTextButton("PLAY!", CC_CALLBACK_0(MainMenu::onPlay, this));
    UTILS_BREAK_IF(_playItem == nullptr);

    setDefaultMenuItem(_backItem);

    result = true;
  } while (false);
  return result;
}

void MainMenu::onOptions()
{
  getAudioHelper()->playEffect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  menu->displayOptionsMenu();
}

void MainMenu::onPlay()
{
  getAudioHelper()->playEffect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  menu->displayPlayMenu();
}

void MainMenu::onExitClick()
{
  getAudioHelper()->playEffect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  menu->exitApp();
}

void MainMenu::onCredits()
{
  getAudioHelper()->playEffect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  menu->displayCreditsMenu();
}

void MainMenu::onAbout()
{
  getAudioHelper()->playEffect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  menu->displayAboutMenu();
}
