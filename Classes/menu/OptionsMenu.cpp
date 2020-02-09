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

#include "OptionsMenu.h"

#include "../ui/SliderObject.h"
#include "../ui/TextButton.h"
#include "../ui/TextToggle.h"
#include "../utils/audio/AudioHelper.h"
#include "scenes/MenuScene.h"

OptionsMenu::OptionsMenu()
  : _desktopApplication(false)
  , _backItem(nullptr)
  , _soundToggle(nullptr)
  , _musicToggle(nullptr)
  , _soundSlider(nullptr)
  , _musicSlider(nullptr)
  , _fullScreenToggle(nullptr)
  , _windowedToggle(nullptr)
  , _debugGridToggle(nullptr)
  , _debugPhysicsToggle(nullptr)
{
}

OptionsMenu* OptionsMenu::create(AudioHelper* audioHelper, const bool isDesktopApplication)
{
  OptionsMenu* ret = nullptr;

  do
  {
    auto object = new OptionsMenu();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audioHelper, isDesktopApplication))
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

bool OptionsMenu::init(AudioHelper* audioHelper, const bool isDesktopApplication)
{
  auto ret = false;

  do
  {
    _desktopApplication = isDesktopApplication;
    const auto width = 3400.f;
    auto height = 2400.f;
    if (_desktopApplication)
    {
      height += 400.f;
    }
    UTILS_BREAK_IF(!base_class::init("Options", audioHelper, width, height));

    ret = true;
  } while (false);

  return ret;
}

void OptionsMenu::display()
{
  base_class::display();

  const auto helper = getAudioHelper();

  _musicToggle->setSelectedIndex(helper->getMusicMuted() ? 0 : 1);
  _soundToggle->setSelectedIndex(helper->getEffectsMuted() ? 0 : 1);

  _musicSlider->setPercentage(helper->getMusicVolume() * 100.f);
  _musicSlider->enable(!helper->getMusicMuted());

  _soundSlider->setPercentage(helper->getSoundVolume() * 100.f);
  _soundSlider->enable(!helper->getEffectsMuted());

  if (_desktopApplication)
  {
    const auto menu = dynamic_cast<MenuScene*>(getParent());
    _fullScreenToggle->setSelectedIndex(menu->isFullScreen() ? 1 : 0);
    _windowedToggle->setSelectedIndex(menu->isFullScreen() ? 0 : 1);
  }

  const auto menu = dynamic_cast<MenuScene*>(getParent());

  _debugPhysicsToggle->setSelectedIndex(menu->isDebugPhysics() ? 1 : 0);
  _debugGridToggle->setSelectedIndex(menu->isDebugGrid() ? 1 : 0);

  updateLabels();
}

bool OptionsMenu::createMenuItems()
{
  auto result = false;
  do
  {
    static auto const LABELS_STARTS = 800.0f;
    _backItem = addTextButton("Back", CC_CALLBACK_0(OptionsMenu::onBack, this));
    UTILS_BREAK_IF(_backItem == nullptr);

    _debugGridToggle = addToggleTextButton("Enabled", CC_CALLBACK_0(OptionsMenu::onDebugGrid, this));
    UTILS_BREAK_IF(_debugGridToggle == nullptr);
    _debugGridToggle->setPosition(Vec2(-400, _debugGridToggle->getPosition().y));
    UTILS_BREAK_IF(addRowLabel("Debug Grid", _debugGridToggle, LABELS_STARTS) == nullptr);

    _debugPhysicsToggle = addToggleTextButton("Enabled", CC_CALLBACK_0(OptionsMenu::onDebugPhysics, this));
    UTILS_BREAK_IF(_debugPhysicsToggle == nullptr);
    _debugPhysicsToggle->setPosition(
      Vec2(_debugGridToggle->getPosition().x, _debugPhysicsToggle->getPosition().y));
    UTILS_BREAK_IF(addRowLabel("Debug Physics", _debugPhysicsToggle, LABELS_STARTS) == nullptr);

    if (_desktopApplication)
    {
      _fullScreenToggle = addToggleTextButton("Full Screen", CC_CALLBACK_0(OptionsMenu::onFullScreen, this));
      UTILS_BREAK_IF(_fullScreenToggle == nullptr);

      _fullScreenToggle->setPosition(
        Vec2(_debugGridToggle->getPosition().x, _fullScreenToggle->getPosition().y));

      UTILS_BREAK_IF(addRowLabel("Video Mode", _fullScreenToggle, LABELS_STARTS) == nullptr);

      _windowedToggle = addToggleTextButton("Windowed", CC_CALLBACK_0(OptionsMenu::onWindowed, this), true);
      UTILS_BREAK_IF(_windowedToggle == nullptr);

      _windowedToggle->setPosition(_fullScreenToggle->getPosition() +
                                   Vec2(_fullScreenToggle->getContentSize().width + 125, 0.f));
    }

    _soundToggle = addToggleTextButton("Enabled", CC_CALLBACK_0(OptionsMenu::onSound, this));
    UTILS_BREAK_IF(_soundToggle == nullptr);

    _soundToggle->setPosition(Vec2(_debugGridToggle->getPosition().x, _soundToggle->getPosition().y));
    UTILS_BREAK_IF(addRowLabel("Sound", _soundToggle, LABELS_STARTS) == nullptr);

    _soundSlider = addSlider(_soundToggle, CC_CALLBACK_1(OptionsMenu::onSoundSliderChange, this));
    UTILS_BREAK_IF(_soundSlider == nullptr);

    _musicToggle = addToggleTextButton("Enabled", CC_CALLBACK_0(OptionsMenu::onMusic, this));
    UTILS_BREAK_IF(_musicToggle == nullptr);

    _musicToggle->setPosition(_soundToggle->getPosition().x, _musicToggle->getPosition().y);
    UTILS_BREAK_IF(addRowLabel("Music", _musicToggle, LABELS_STARTS) == nullptr);

    _musicSlider = addSlider(_musicToggle, CC_CALLBACK_1(OptionsMenu::onMusicSliderChange, this));
    UTILS_BREAK_IF(_musicSlider == nullptr);

    setDefaultMenuItem(_backItem);

    result = true;
  } while (false);
  return result;
}

void OptionsMenu::onBack()
{
  getAudioHelper()->playEffect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  menu->displayMainMenu();
}

void OptionsMenu::onMusic()
{
  getAudioHelper()->playEffect("sounds/select.mp3");

  const auto menu = dynamic_cast<MenuScene*>(getParent());
  const auto disable = _musicToggle->getSelectedIndex() == 0;
  _musicSlider->enable(!disable);
  menu->changeMusic(disable);
  updateLabels();
}

void OptionsMenu::onSound()
{
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  const auto disable = _soundToggle->getSelectedIndex() == 0;
  _soundSlider->enable(!disable);
  menu->changeSound(disable);

  getAudioHelper()->playEffect("sounds/select.mp3");
  updateLabels();
}

void OptionsMenu::onMusicSliderChange(const float percentage)
{
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  menu->changeMusicVolume(percentage / 100);
}

void OptionsMenu::onSoundSliderChange(const float percentage)
{
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  menu->changeSoundVolume(percentage / 100);
}

void OptionsMenu::onFullScreen()
{
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  if (menu->isFullScreen())
  {
    _fullScreenToggle->setSelectedIndex(1);
    return;
  }
  getAudioHelper()->playEffect("sounds/select.mp3");

  const auto fullScreen = _fullScreenToggle->getSelectedIndex() == 1;
  _windowedToggle->setSelectedIndex(fullScreen ? 0 : 1);
  menu->changeApplicationVideoMode(fullScreen);
}

void OptionsMenu::onWindowed()
{
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  if (!menu->isFullScreen())
  {
    _windowedToggle->setSelectedIndex(1);
    return;
  }

  getAudioHelper()->playEffect("sounds/select.mp3");

  const auto full_screen = _windowedToggle->getSelectedIndex() == 0;
  _fullScreenToggle->setSelectedIndex(full_screen ? 1 : 0);
  menu->changeApplicationVideoMode(full_screen);
}

void OptionsMenu::onDebugGrid()
{
  getAudioHelper()->playEffect("sounds/select.mp3");

  const auto menu = dynamic_cast<MenuScene*>(getParent());
  const auto debug = _debugGridToggle->getSelectedIndex() == 1;
  menu->setDebugGrid(debug);

  updateLabels();
}

void OptionsMenu::onDebugPhysics()
{
  getAudioHelper()->playEffect("sounds/select.mp3");

  const auto menu = dynamic_cast<MenuScene*>(getParent());
  const auto debug = _debugPhysicsToggle->getSelectedIndex() == 1;
  menu->setDebugPhysics(debug);

  updateLabels();
}

void OptionsMenu::updateLabels()
{
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  const auto helper = getAudioHelper();

  _soundToggle->setText(helper->getEffectsMuted() ? "Disabled" : "Enabled");
  _musicToggle->setText(helper->getMusicMuted() ? "Disabled" : "Enabled");
  _debugPhysicsToggle->setText(menu->isDebugPhysics() ? "Enabled" : "Disabled");
  _debugGridToggle->setText(menu->isDebugGrid() ? "Enabled" : "Disabled");
}
