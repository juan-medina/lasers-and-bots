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

#include "GameUi.h"

#include "../misc/LevelManager.h"
#include "../utils/audio/AudioHelper.h"
#include "../utils/controller/InputController.h"
#include "LevelCompleted.h"
#include "MessageWindow.h"
#include "PauseWindow.h"
#include "VirtualJoyStick.h"
#include "scenes/GameScene.h"

GameUi::GameUi()
  : _virtualJoyStick(nullptr)
  , _shieldBar(nullptr)
  , _shieldLabel(nullptr)
  , _pauseItem(nullptr)
  , _timeLabel(nullptr)
  , _subTimeLabel(nullptr)
  , _countdownLabel(nullptr)
  , _levelNameLabel(nullptr)
  , _timeLimit(0)
  , _continueCallback(nullptr)
  , _audioHelper(nullptr)
  , _messageWindow(nullptr)
  , _levelCompleted(nullptr)
  , _pauseWindow(nullptr)
  , _levelManager(nullptr)
  , _level(0)
  , _inputController(nullptr)
{
}

GameUi* GameUi::create(AudioHelper* audioHelper, InputController* inputController, LevelManager* levelManager,
                       const unsigned short int level)
{
  GameUi* ret = nullptr;

  do
  {
    auto object = new GameUi();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audioHelper, inputController, levelManager, level))
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

bool GameUi::init(AudioHelper* audioHelper, InputController* inputController, LevelManager* levelManager,
                  const unsigned short int level)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init());

    const auto& size = Director::getInstance()->getVisibleSize();

    _audioHelper = audioHelper;
    _levelManager = levelManager;
    _inputController = inputController;
    _level = level;

    _audioHelper->preLoadEffect("sounds/select.mp3");

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-0.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/ui-1.plist");

    const auto close = Sprite::createWithSpriteFrameName("01_Exit_1.png");
    UTILS_BREAK_IF(close == nullptr);

    const auto closeClick = Sprite::createWithSpriteFrameName("01_Exit_2.png");
    UTILS_BREAK_IF(closeClick == nullptr);

    const auto closeItem = MenuItemSprite::create(close, closeClick, CC_CALLBACK_1(GameUi::onClose, this));
    UTILS_BREAK_IF(closeItem == nullptr);

    closeItem->setPosition(size.width / 2 - close->getContentSize().width / 2,
                           size.height / 2 - close->getContentSize().height / 2);

    const auto pause = Sprite::createWithSpriteFrameName("05_Pause_1.png");
    UTILS_BREAK_IF(pause == nullptr);

    const auto pauseClick = Sprite::createWithSpriteFrameName("05_Pause_2.png");
    UTILS_BREAK_IF(pauseClick == nullptr);

    const auto pauseDisable = Sprite::createWithSpriteFrameName("05_Pause_4.png");
    UTILS_BREAK_IF(pauseClick == nullptr);

    const auto pauseItem = MenuItemSprite::create(pause, pauseClick, pauseDisable);
    UTILS_BREAK_IF(pauseItem == nullptr);

    const auto play = Sprite::createWithSpriteFrameName("06_Play_3.png");
    UTILS_BREAK_IF(play == nullptr);

    const auto playClick = Sprite::createWithSpriteFrameName("06_Play_2.png");
    UTILS_BREAK_IF(playClick == nullptr);

    const auto playDisable = Sprite::createWithSpriteFrameName("06_Play_4.png");
    UTILS_BREAK_IF(playDisable == nullptr);

    const auto playItem = MenuItemSprite::create(play, playClick, playDisable);
    UTILS_BREAK_IF(playItem == nullptr);

    _pauseItem =
      MenuItemToggle::createWithCallback(CC_CALLBACK_1(GameUi::onPause, this), pauseItem, playItem, nullptr);
    UTILS_BREAK_IF(_pauseItem == nullptr);

    const auto gap = Vec2(close->getContentSize().width * 1.25f, 0.f);
    _pauseItem->setPosition(closeItem->getPosition() - gap);

    const auto reload = Sprite::createWithSpriteFrameName("07_Reload_1.png");
    UTILS_BREAK_IF(reload == nullptr);

    const auto reloadClick = Sprite::createWithSpriteFrameName("07_Reload_2.png");
    UTILS_BREAK_IF(reloadClick == nullptr);

    const auto reloadItem =
      MenuItemSprite::create(reload, reloadClick, CC_CALLBACK_1(GameUi::onReload, this));
    UTILS_BREAK_IF(reloadItem == nullptr);

    reloadItem->setPosition(_pauseItem->getPosition() - gap);

    const auto menu = Menu::create(closeItem, _pauseItem, reloadItem, nullptr);
    UTILS_BREAK_IF(menu == nullptr);

    addChild(menu, 100);

    _virtualJoyStick = VirtualJoyStick::create(_inputController);
    addChild(_virtualJoyStick, 100);

    const auto headPos = Vec2(50.f, size.height - 50);

    auto head = Sprite::createWithSpriteFrameName("03_head.png");
    UTILS_BREAK_IF(head == nullptr);

    head->setAnchorPoint(Vec2(0, 1));
    head->setPosition(headPos);
    addChild(head, 100);

    const auto barSprite = Sprite::createWithSpriteFrameName("04_bar.png");
    UTILS_BREAK_IF(barSprite == nullptr);

    const auto barPos =
      headPos + Vec2(head->getContentSize().width + (barSprite->getContentSize().width / 2.f),
                     -head->getContentSize().height / 2.f);
    barSprite->setPosition(barPos);

    addChild(barSprite, 100);

    const auto barFullSprite = Sprite::createWithSpriteFrameName("05_bar_full.png");
    UTILS_BREAK_IF(barFullSprite == nullptr);
    barFullSprite->setOpacity(120);
    barFullSprite->setBlendFunc(BlendFunc::ADDITIVE);

    _shieldBar = ProgressTimer::create(barFullSprite);
    UTILS_BREAK_IF(_shieldBar == nullptr);

    _shieldBar->setType(ProgressTimer::Type::BAR);
    _shieldBar->setMidpoint(Vec2(0, 0));
    _shieldBar->setBarChangeRate(Vec2(1, 0));
    _shieldBar->setPercentage(100.f);
    _shieldBar->setPosition(barPos);

    _shieldBar->setOpacity(200);

    addChild(_shieldBar, 100);

    _shieldLabel = Label::createWithTTF("100%", "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(_shieldLabel == nullptr);

    _shieldLabel->setTextColor(Color4B(255, 255, 255, 255));
    _shieldLabel->setPosition(barPos);

    _shieldLabel->enableOutline(Color4B(0, 0, 0, 255), 5);

    addChild(_shieldLabel, 100);

    _timeLabel = Label::createWithTTF("00:00.00", "fonts/tahoma.ttf", 180);
    UTILS_BREAK_IF(_timeLabel == nullptr);

    _timeLabel->setTextColor(Color4B(0, 255, 255, 255));

    _timeLabel->enableGlow(Color4B(0, 127, 127, 127));
    _timeLabel->enableShadow(Color4B(255, 255, 255, 127), Size(5, -5));
    _timeLabel->enableOutline(Color4B(0, 0, 0, 255), 5);

    _timeLabel->setPosition(Vec2(size.width / 2, size.height - closeItem->getContentSize().height / 2));

    addChild(_timeLabel, 100);

    _subTimeLabel = Label::createWithTTF("00:00.00", "fonts/tahoma.ttf", 90);
    UTILS_BREAK_IF(_subTimeLabel == nullptr);

    _subTimeLabel->setTextColor(Color4B(0, 0, 255, 255));

    _subTimeLabel->enableShadow(Color4B(0, 0, 0, 127), Size(5, -5));
    _subTimeLabel->enableOutline(Color4B(0, 0, 0, 255), 5);

    _subTimeLabel->setPosition(
      Vec2(_timeLabel->getPosition().x + _timeLabel->getContentSize().width, _timeLabel->getPosition().y));

    addChild(_subTimeLabel, 100);

    _countdownLabel = Label::createWithTTF("GO!", "fonts/tahoma.ttf", 500);
    UTILS_BREAK_IF(_countdownLabel == nullptr);

    _countdownLabel->setTextColor(Color4B(0, 255, 255, 255));
    _countdownLabel->enableGlow(Color4B(0, 127, 127, 127));
    _countdownLabel->enableShadow(Color4B(255, 255, 255, 127), Size(10, -10));
    _countdownLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
    _countdownLabel->setPosition(Vec2(size.width / 2, size.height / 2));
    _countdownLabel->setVisible(false);
    addChild(_countdownLabel, 100);

    _levelNameLabel = Label::createWithTTF(_levelManager->getLevelName(_level), "fonts/tahoma.ttf", 500);
    UTILS_BREAK_IF(_levelNameLabel == nullptr);

    _levelNameLabel->setTextColor(Color4B(0, 255, 255, 255));
    _levelNameLabel->enableGlow(Color4B(0, 127, 127, 127));
    _levelNameLabel->enableShadow(Color4B(255, 255, 255, 127), Size(10, -10));
    _levelNameLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
    _levelNameLabel->setPosition(Vec2(size.width / 2, size.height - (size.height / 3)));
    _levelNameLabel->setVisible(false);
    addChild(_levelNameLabel, 100);

    _messageWindow = MessageWindow::create(_audioHelper);
    UTILS_BREAK_IF(_messageWindow == nullptr);

    addChild(_messageWindow);

    _levelCompleted = LevelCompleted::create(_audioHelper, _levelManager);
    UTILS_BREAK_IF(_levelCompleted == nullptr);

    addChild(_levelCompleted);

    _pauseWindow = PauseWindow::create(_audioHelper);
    UTILS_BREAK_IF(_pauseWindow == nullptr);

    addChild(_pauseWindow);

    scheduleUpdate();

    ret = true;
  } while (false);

  return ret;
}

void GameUi::onPause(Ref* sender) const
{
  _audioHelper->playEffect("sounds/select.mp3");
  const auto scene = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene());
  scene->togglePause();

  if (scene->isPaused())
  {
    _pauseItem->setSelectedIndex(1);
    _pauseWindow->display();
  }
  else
  {
    _pauseItem->setSelectedIndex(0);
    _pauseWindow->hide();
  }
}

void GameUi::onClose(Ref* sender) const
{
  _audioHelper->playEffect("sounds/select.mp3");
  const auto scene = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene());
  scene->close();
}

void GameUi::onReload(Ref* sender) const
{
  _audioHelper->playEffect("sounds/select.mp3");
  const auto scene = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene());
  scene->reload();
}

void GameUi::changePauseButton() const
{
  _pauseItem->setSelectedIndex(1);
}

void GameUi::disableButtons(const bool disabled) const
{
  _pauseItem->setEnabled(!disabled);
  _pauseItem->setSelectedIndex(0);
  _virtualJoyStick->disabled(disabled);
}

void GameUi::updateTime(const float time, const unsigned int timeLimit)
{
  _timeLimit = timeLimit;
  const auto timeLeft = timeLimit - time;
  if (timeLeft < 0.f)
  {
    _subTimeLabel->setTextColor(Color4B(255, 0, 0, 255));
  }
  _subTimeLabel->setString(timeMessage(fabs(timeLeft)));
  _timeLabel->setString(timeMessage(time));
}

string GameUi::timeMessage(const float time)
{
  float whole;
  const auto fractional = std::modf(time, &whole);

  const auto minutes = static_cast<int>(whole / 60.f);
  const auto seconds = static_cast<int>(time - (minutes * 60));
  const auto milliseconds = static_cast<int>(fractional * 100);
  return StringFormat("%02d:%02d%c%02d", minutes, seconds, '.', milliseconds);
}

void GameUi::displayMessage(const std::string& message, const std::string& subMessage,
                            const ccMenuCallback& callback)
{
  _continueCallback = callback;
  _messageWindow->display(message, subMessage, CC_CALLBACK_0(GameUi::onContinue, this));
}

void GameUi::displayLevelCompleted(const unsigned short int level, const float time,
                                   const unsigned short int stars, const CompletedResult completion,
                                   const ccMenuCallback& callback)
{
  _continueCallback = callback;
  _levelCompleted->display(level, time, stars, completion, CC_CALLBACK_0(GameUi::onContinue, this));
}

void GameUi::updateCountdown(const int value) const
{
  const auto text = StringFormat("%d", value);
  if (value >= 0)
  {
    _countdownLabel->setString(text);
    _countdownLabel->setVisible(true);
    if (value == 3)
    {
      _levelNameLabel->setVisible(true);
    }
  }
  else
  {
    _countdownLabel->setString("GO!");

    const auto delay = DelayTime::create(1.0f);
    const auto fadeOut = FadeOut::create(0.5);
    const auto remove = RemoveSelf::create(true);
    const auto vanish = Sequence::create(delay, fadeOut, remove, nullptr);
    _countdownLabel->runAction(vanish);
    _levelNameLabel->runAction(vanish->clone());
  }

  const auto scaleUp = ScaleTo::create(0.25f, 1.5f, 1.5f);
  const auto scaleDown = ScaleTo::create(0.25f, 1.0f, 1.0f);
  const auto scale = Sequence::create(scaleUp, scaleDown, nullptr);
  _countdownLabel->runAction(scale);
}

void GameUi::displayPauseWindow() const
{
  _pauseWindow->display();
}

void GameUi::update(float delta)
{
  if (_continueCallback != nullptr)
  {
    if (_inputController->singlePressButtonA() || _inputController->singlePressButtonB() ||
        _inputController->singlePressButtonBack())
    {
      onContinue();
      return;
    }
  }

  if (_inputController->singlePressButtonStart())
  {
    if (_pauseItem->isEnabled())
    {
      _pauseItem->setSelectedIndex(_pauseItem->getSelectedIndex() == 0 ? 1 : 0);
      onPause(this);
      return;
    }
  }

  const auto scene = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene());
  if (scene->isPaused())
  {
    if (_inputController->singlePressUp())
    {
      _pauseWindow->moveSelectionUp();
    }
    if (_inputController->singlePressDown())
    {
      _pauseWindow->moveSelectionDown();
    }
    if (_inputController->singlePressLeft())
    {
      _pauseWindow->moveSelectionLeft();
    }
    if (_inputController->singlePressRight())
    {
      _pauseWindow->moveSelectionRight();
    }
    if (_inputController->singlePressButtonA())
    {
      _pauseWindow->activateSelection();
    }
    if (_inputController->singlePressButtonB() || _inputController->singlePressButtonBack())
    {
      _pauseWindow->selectionBack();
    }
  }
}

void GameUi::onContinue()
{
  if (_continueCallback != nullptr)
  {
    _audioHelper->playEffect("sounds/select.mp3");
    _continueCallback(this);
    _continueCallback = nullptr;
  }
}
