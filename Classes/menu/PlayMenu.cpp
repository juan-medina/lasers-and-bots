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

#include "PlayMenu.h"

#include "../laser_and_bots_app.h"
#include "../misc/LevelManager.h"
#include "../ui/GameUI.h"
#include "../ui/TextButton.h"
#include "../ui/TextToggle.h"
#include "../utils/audio/AudioHelper.h"
#include "scenes/MenuScene.h"

PlayMenu::PlayMenu()
  : _backItem(nullptr)
  , _playItem(nullptr)
  , _levelNameLabel(nullptr)
  , _levelTimeLimitLabel(nullptr)
  , _levelTimeRecordLabel(nullptr)
  , _level3StarsTimeRecordLabel(nullptr)
  , _levelStarsLabel(nullptr)
  , _selectedLevel(1)
{
}

PlayMenu* PlayMenu::create(AudioHelper* audioHelper, const unsigned short int selectedLevel)
{
  PlayMenu* ret = nullptr;

  do
  {
    auto object = new PlayMenu();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audioHelper, selectedLevel))
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

bool PlayMenu::init(AudioHelper* audioHelper, const unsigned short int selectedLevel)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init("Level Select", audioHelper, 3600.f, 1800.f));

    _selectedLevel = selectedLevel;

    ret = true;
  } while (false);

  return ret;
}

void PlayMenu::display()
{
  BaseClass::display();

  const auto levels = getLevelManager();
  const auto maxLevel = levels->getNumLevels();

  for (const auto pair : _levelButtons)
  {
    const auto id = pair.first;
    const auto item = pair.second;

    item->setVisible(id <= maxLevel);

    const auto stars = levels->getLevelStars(id);
    item->setEnabled(levels->isLevelEnabled(id));

    for (auto starCounter = 0; starCounter < 3; ++starCounter)
    {
      const auto star = item->getChildByTag(STAR_TAG + starCounter);
      const auto isGold = (starCounter + 1) <= stars;

      star->setColor(isGold ? Color3B(0, 255, 255) : Color3B(127, 127, 127));
    }
  }
  selectLevel(_selectedLevel);
  selectMenuItem(_levelButtons.at(_selectedLevel));
}

bool PlayMenu::createMenuItems()
{
  auto result = false;
  do
  {
    _backItem = addTextButton("Back", CC_CALLBACK_0(PlayMenu::onBack, this));
    UTILS_BREAK_IF(_backItem == nullptr);

    _playItem = addTextButton("Play", CC_CALLBACK_0(PlayMenu::onPlay, this));
    UTILS_BREAK_IF(_playItem == nullptr);

    _playItem->setPosition(-550 + _playItem->getContentSize().width / 2, _playItem->getPosition().y);

    const auto margin = Vec2(300.f, 370.f);
    const auto firstButtonPos =
      Vec2(-(getContentSize().width / 2) + margin.x, getContentSize().height - margin.y);

    const auto firstLabelPos = Vec2(-550.f, (getContentSize().height / 2) - margin.y);
    auto labelPos = firstLabelPos;
    static const auto SEPARATION = 900.f;

    _levelNameLabel = addLabels("Level", "One long level name", labelPos, SEPARATION);
    UTILS_BREAK_IF(_levelNameLabel == nullptr);
    labelPos.y -= 200;

    _levelTimeLimitLabel = addLabels("Time Limit", "00:00", labelPos, SEPARATION);
    UTILS_BREAK_IF(_levelTimeLimitLabel == nullptr);
    labelPos.y -= 200;

    _levelStarsLabel = addLabels("Stars", "", labelPos, SEPARATION);
    UTILS_BREAK_IF(_levelStarsLabel == nullptr);
    labelPos.y -= 200;

    auto labelStarPos =
      _levelStarsLabel->getPosition() + Vec2(_levelStarsLabel->getContentSize().width + 60.f, 0.f);

    for (auto starCounter = 0; starCounter < 3; ++starCounter)
    {
      auto labelStar = Sprite::createWithSpriteFrameName("09_star_01.png");
      UTILS_BREAK_IF(labelStar == nullptr);

      labelStar->setScale(0.50f);
      labelStar->setPosition(labelStarPos);

      addChild(labelStar);
      labelStarPos.x += (labelStar->getContentSize().width * labelStar->getScale()) + 30.f;
      labelStar->setTag(STAR_TAG + starCounter);
    }

    _levelTimeRecordLabel = addLabels("Time Record", "00:00", labelPos, SEPARATION);
    UTILS_BREAK_IF(_levelTimeRecordLabel == nullptr);
    labelPos.y -= 200;

    _level3StarsTimeRecordLabel = addLabels("3 Stars Record", "00:00", labelPos, SEPARATION);
    UTILS_BREAK_IF(_level3StarsTimeRecordLabel == nullptr);

    auto buttonPos = firstButtonPos;
    auto col = 1;
    for (unsigned short int buttonCount = 1; buttonCount <= 12; ++buttonCount)
    {
      auto text = StringFormat("%02d", buttonCount);
      auto button = addSmallToggleTextButton(text, CC_CALLBACK_1(PlayMenu::onLevelSelect, this, buttonCount));
      UTILS_BREAK_IF(button == nullptr);

      button->setPosition(buttonPos);

      auto starPos = Vec2(60.f, 90.f);

      for (auto starCounter = 0; starCounter < 3; ++starCounter)
      {
        auto star = Sprite::createWithSpriteFrameName("09_star_01.png");
        UTILS_BREAK_IF(star == nullptr);

        star->setScale(0.20f);
        star->setPosition(starPos);

        button->addChild(star);
        starPos.x += (star->getContentSize().width * star->getScale());
        star->setTag(STAR_TAG + starCounter);
      }

      button->setVisible(false);
      _levelButtons.insert(std::make_pair(buttonCount, button));

      buttonPos.x += button->getContentSize().width + 50.0f;
      col++;
      if (col > 3)
      {
        col = 1;
        buttonPos.x = firstButtonPos.x;
        buttonPos.y -= (button->getContentSize().height) + 50.0f;
      }
    }

    setDefaultMenuItem(_backItem);

    result = true;
  } while (false);
  return result;
}

Label* PlayMenu::addLabels(const std::string& labelText, const std::string& text, const Vec2& pos,
                           const float separation)
{
  Label* result = nullptr;
  do
  {
    auto label = Label::createWithTTF(labelText, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label == nullptr);

    label->setPosition(pos - Vec2(-label->getContentSize().width / 2, -label->getContentSize().height / 2));
    label->setTextColor(Color4B(255, 255, 255, 255));
    label->enableOutline(Color4B(0, 0, 0, 255), 5);
    addChild(label);

    auto value = Label::createWithTTF(text, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label == nullptr);

    value->setAnchorPoint(Vec2(0.f, 0.5f));
    value->setHorizontalAlignment(TextHAlignment::LEFT);
    value->setPosition(pos.x + separation, label->getPosition().y);
    value->setTextColor(Color4B(0, 255, 255, 255));
    value->enableOutline(Color4B(0, 0, 0, 255), 5);
    addChild(value);

    result = value;
  } while (false);

  return result;
}

void PlayMenu::onBack()
{
  getAudioHelper()->playEffect("sounds/select.mp3");
  hide();
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  menu->displayMainMenu();
}

void PlayMenu::onLevelSelect(Ref*, const unsigned short int level)
{
  getAudioHelper()->playEffect("sounds/select.mp3");

  if (level == _selectedLevel)
  {
    _levelButtons.at(level)->setSelectedIndex(1);
  }
  else
  {
    selectLevel(level);
  }
  selectMenuItem(_playItem);
}

void PlayMenu::onPlay()
{
  getAudioHelper()->playEffect("sounds/select.mp3");

  hide();
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  menu->goToGame(_selectedLevel);
}

void PlayMenu::selectLevel(const unsigned short int level)
{
  const auto levels = getLevelManager();
  const auto levelName = levels->getLevelName(level);
  const auto levelTimeLimit = levels->getLevelTimeLimit(level);
  const auto levelTimeLimitString = GameUI::timeMessage(levelTimeLimit);
  const auto levelTimeRecord = levels->getLevelTimeRecord(level);
  const auto levelTimeRecordString = GameUI::timeMessage(levelTimeRecord);
  const auto level3StarsRecord = levels->getLevel3StarsRecord(level);
  const auto level3StarsRecordString = GameUI::timeMessage(level3StarsRecord);
  const auto stars = levels->getLevelStars(level);

  _levelNameLabel->setString(levelName);
  _levelTimeLimitLabel->setString(levelTimeLimitString);

  if (levelTimeRecord != LevelManager::NO_TIME_RECORD)
  {
    _levelTimeRecordLabel->setString(levelTimeRecordString);
  }
  else
  {
    _levelTimeRecordLabel->setString("n/a");
  }

  if (level3StarsRecord != LevelManager::NO_TIME_RECORD)
  {
    _level3StarsTimeRecordLabel->setString(level3StarsRecordString);
  }
  else
  {
    _level3StarsTimeRecordLabel->setString("n/a");
  }

  for (const auto pair : _levelButtons)
  {
    const auto id = pair.first;
    const auto item = pair.second;
    item->setSelectedIndex(id == level ? 1 : 0);
  }

  for (auto starCounter = 0; starCounter < 3; ++starCounter)
  {
    const auto star = getChildByTag(STAR_TAG + starCounter);
    const auto isGold = (starCounter + 1) <= stars;

    star->setColor(isGold ? Color3B(0, 255, 255) : Color3B::WHITE);
  }

  _selectedLevel = level;
}

LevelManager* PlayMenu::getLevelManager()
{
  const auto menu = dynamic_cast<MenuScene*>(getParent());
  const auto app = dynamic_cast<laser_and_bots_app*>(menu->getApplication());
  return app->get_level_manager();
}
