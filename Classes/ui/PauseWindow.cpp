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

#include "PauseWindow.h"
#include "../laser_and_bots_app.h"
#include "../scenes/game_scene.h"
#include "../ui/TextButton.h"
#include "../ui/TextToggle.h"
#include "../utils/audio/AudioHelper.h"
#include "GameUI.h"

PauseWindow::PauseWindow() : _audioHelper(nullptr), _toggleMusicItem(nullptr), _toggleSfxItem(nullptr) {}

PauseWindow* PauseWindow::create(AudioHelper* audioHelper)
{
  PauseWindow* ret = nullptr;

  do
  {
    auto object = new PauseWindow();
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

bool PauseWindow::init(AudioHelper* audioHelper)
{
  auto ret = false;

  do
  {
    const auto& size = Director::getInstance()->getVisibleSize();

    const auto darkAll = LayerColor::create(Color4B(0, 0, 0, 127));
    UTILS_BREAK_IF(darkAll == nullptr);

    addChild(darkAll, 0);
    darkAll->setPosition(-size.width / 2, -size.height / 2);

    _audioHelper = audioHelper;
    UTILS_BREAK_IF(!BaseClass::init("Pause", audioHelper, 1300.f, 1800.0f, AnimationType::Fade));

    _audioHelper->preLoadEffect("sounds/select.mp3");

    ret = true;
  } while (false);

  return ret;
}

void PauseWindow::display()
{
  BaseClass::display();

  const auto musicMuted = _audioHelper->getMusicMuted();
  const auto effectsMuted = _audioHelper->getEffectsMuted();

  _toggleMusicItem->setSelectedIndex(musicMuted ? 0 : 1);
  _toggleSfxItem->setSelectedIndex(effectsMuted ? 0 : 1);
}

void PauseWindow::hide()
{
  BaseClass::hide();
}

void PauseWindow::onMusicToggle()
{
  const auto musicMuted = _toggleMusicItem->getSelectedIndex() == 0;
  const auto ui = dynamic_cast<GameUI*>(getParent());
  const auto game = dynamic_cast<game_scene*>(ui->getParent());
  auto app = dynamic_cast<laser_and_bots_app*>(game->getApplication());

  app->set_music_muted(musicMuted);
  _audioHelper->playEffect("sounds/select.mp3");
}

void PauseWindow::onSfxToggle()
{
  const auto sfxMuted = _toggleSfxItem->getSelectedIndex() == 0;
  const auto ui = dynamic_cast<GameUI*>(getParent());
  const auto game = dynamic_cast<game_scene*>(ui->getParent());
  auto app = dynamic_cast<laser_and_bots_app*>(game->getApplication());

  app->set_effects_muted(sfxMuted);
  _audioHelper->playEffect("sounds/select.mp3");
}

void PauseWindow::onResume()
{
  auto ui = dynamic_cast<GameUI*>(getParent());
  ui->onPause(this);
}

void PauseWindow::onReload()
{
  auto ui = dynamic_cast<GameUI*>(getParent());
  ui->onReload(this);
}

void PauseWindow::onExit()
{
  auto ui = dynamic_cast<GameUI*>(getParent());
  ui->onClose(this);
}

bool PauseWindow::createMenuItems()
{
  auto result = false;

  do
  {
    UTILS_BREAK_IF(addTextButton("Exit", CC_CALLBACK_0(PauseWindow::onExit, this)) == nullptr);

    const auto resumeButton = addTextButton("Resume", CC_CALLBACK_0(PauseWindow::onResume, this));
    UTILS_BREAK_IF(resumeButton == nullptr);

    const auto restartButton = addTextButton("Restart", CC_CALLBACK_0(PauseWindow::onReload, this));
    UTILS_BREAK_IF(restartButton == nullptr);

    _toggleSfxItem = addToggleImageButton("13_sound_0", CC_CALLBACK_0(PauseWindow::onSfxToggle, this));
    UTILS_BREAK_IF(_toggleSfxItem == nullptr);

    const auto sfxPosition =
      restartButton->getPosition() +
      Vec2(-(restartButton->getContentSize().width / 2) + _toggleSfxItem->getContentSize().width / 2,
           restartButton->getContentSize().height + 150.f);

    _toggleSfxItem->setPosition(sfxPosition);

    _toggleMusicItem = addToggleImageButton("12_music_0", CC_CALLBACK_0(PauseWindow::onMusicToggle, this));
    UTILS_BREAK_IF(_toggleMusicItem == nullptr);

    const auto musicPosition =
      Vec2(restartButton->getPosition().x + (restartButton->getContentSize().width / 2) -
             _toggleMusicItem->getContentSize().width / 2,
           sfxPosition.y);

    _toggleMusicItem->setPosition(musicPosition);

    setDefaultMenuItem(resumeButton);

    result = true;
  } while (false);

  return result;
}
