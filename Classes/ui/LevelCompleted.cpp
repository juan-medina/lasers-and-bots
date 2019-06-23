#include "LevelCompleted.h"
#include "../misc/LevelManager.h"
#include "../utils/audio/AudioHelper.h"
#include "TextButton.h"
#include "GameUI.h"

LevelCompleted::LevelCompleted()
  : _audioHelper(nullptr)
  , _continueItem(nullptr)
  , _levelNameLabel(nullptr)
  , _levelManager(nullptr)
  , _levelTotalTimeLabel(nullptr)
  , _levelTimeLimitLabel(nullptr)
  , _levelTimeRecordLabel(nullptr)
  , _level3StarsRecordLabel(nullptr)
{
}

LevelCompleted* LevelCompleted::create(AudioHelper* audioHelper, LevelManager* levelManager)
{
  LevelCompleted* ret = nullptr;

  do
  {
    auto object = new LevelCompleted();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(audioHelper, levelManager))
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

bool LevelCompleted::init(AudioHelper* audioHelper, LevelManager* levelManager)
{
  auto ret = false;

  do
  {
    _audioHelper = audioHelper;
    _levelManager = levelManager;

    const auto& size = Director::getInstance()->getVisibleSize();

    const auto darkAll = LayerColor::create(Color4B(0, 0, 0, 127));
    UTILS_BREAK_IF(darkAll == nullptr);

    addChild(darkAll, 0);
    darkAll->setPosition(-size.width / 2, -size.height / 2);

    UTILS_BREAK_IF(!BaseClass::init("Level Completed", audioHelper, 1800.f, 2100.f, animation_type::fade));

    _audioHelper->preLoadEffect("sounds/star.mp3");

    const auto horizontalSegment = getContentSize().width;

    _levelNameLabel = Label::createWithTTF("", "fonts/tahoma.ttf", 100);
    UTILS_BREAK_IF(_levelNameLabel == nullptr);

    _levelNameLabel->setTextColor(Color4B(255, 255, 255, 255));
    _levelNameLabel->enableOutline(Color4B(0, 0, 0, 255), 5);

    _levelNameLabel->setPosition(0.f, (getContentSize().height / 2) - 250);

    addChild(_levelNameLabel, 100);

    const auto firstStartPos = Vec2(-horizontalSegment / 3, _levelNameLabel->getPosition().y - 350.f);

    for (unsigned short int startCounter = 0; startCounter < 3; ++startCounter)
    {
      const auto starGray = Sprite::createWithSpriteFrameName("09_star_01.png");
      UTILS_BREAK_IF(starGray == nullptr);

      _grayStars.push_back(starGray);
      starGray->setCascadeOpacityEnabled(true);
      starGray->setOpacity(0);

      const auto starPos = firstStartPos + Vec2((horizontalSegment / 3) * startCounter, 0.f);
      starGray->setPosition(starPos);
      addChild(starGray, 100);

      auto starTex = string("level\ncompleted");
      if (startCounter == 1)
      {
        starTex = "under\n 00:00.00";
      }
      else if (startCounter == 2)
      {
        starTex = "with 100%\nshield";
      }

      const auto labelStar = Label::createWithTTF(starTex, "fonts/tahoma.ttf", 70);
      UTILS_BREAK_IF(labelStar == nullptr);
      _labelStars.push_back(labelStar);

      const auto labelPos =
        Vec2(starGray->getContentSize().width / 2, -(starGray->getContentSize().height / 2) + 40.f);
      labelStar->setHorizontalAlignment(TextHAlignment::CENTER);
      labelStar->setPosition(labelPos);
      labelStar->setTextColor(Color4B(255, 255, 255, 255));
      labelStar->enableOutline(Color4B(0, 0, 0, 255), 5);

      starGray->addChild(labelStar, 100);

      const auto starGold = Sprite::createWithSpriteFrameName("09_star_01.png");
      UTILS_BREAK_IF(starGold == nullptr);

      _goldStars.push_back(starGold);

      starGold->setPosition(starPos);
      starGold->setOpacity(0);

      addChild(starGold, 110);
    }

    const auto firstStar = _grayStars.at(0);
    const auto firstLabelPos = firstStar->getPosition() - Vec2(firstStar->getContentSize().width / 2,
                                                               firstStar->getContentSize().height * 1.9);
    auto labelPos = firstLabelPos;

    static const auto SEPARATION = 950.f;

    _levelTotalTimeLabel = addLabels("Total Time", "00:00.00", labelPos, SEPARATION);
    UTILS_BREAK_IF(_levelTotalTimeLabel == nullptr);
    labelPos.y -= 200;

    _levelTimeLimitLabel = addLabels("Time Limit", "00:00.00", labelPos, SEPARATION);
    UTILS_BREAK_IF(_levelTimeLimitLabel == nullptr);
    labelPos.y -= 200;

    _levelTimeRecordLabel = addLabels("Time Record", "00:00.00", labelPos, SEPARATION);
    UTILS_BREAK_IF(_levelTimeLimitLabel == nullptr);
    labelPos.y -= 200;

    _level3StarsRecordLabel = addLabels("3 Stars Record", "00:00.00", labelPos, SEPARATION);
    UTILS_BREAK_IF(_levelTimeLimitLabel == nullptr);

    ret = true;
  } while (false);
  return ret;
}

void LevelCompleted::display(const unsigned short int level, const float time, const unsigned short int stars,
                             const CompletedResult completion, const ccMenuCallback& callback)
{
  BaseClass::display();

  const auto levelName = _levelManager->getLevelName(level);
  const auto levelTimeLimit = _levelManager->getLevelTimeLimit(level);
  const auto levelTimeRecord = _levelManager->getLevelTimeRecord(level);
  const auto level3StarsRecord = _levelManager->getLevel3StarsRecord(level);

  _levelTotalTimeLabel->setString(GameUI::timeMessage(time));
  _levelTimeLimitLabel->setString(GameUI::timeMessage(levelTimeLimit));

  if (levelTimeRecord != LevelManager::NO_TIME_RECORD)
  {
    _levelTimeRecordLabel->setString(GameUI::timeMessage(levelTimeRecord));
    if ((completion == CompletedResult::NewLevelRecord) ||
        (completion == CompletedResult::NewLevelRecordAnd3StarsRecord))
    {
      animateLabel(_levelTimeRecordLabel);
    }
  }
  else
  {
    _levelTimeRecordLabel->setString("n/a");
  }

  if (level3StarsRecord != LevelManager::NO_TIME_RECORD)
  {
    _level3StarsRecordLabel->setString(GameUI::timeMessage(level3StarsRecord));
    if ((completion == CompletedResult::NewLevel3StarsRecord) ||
        (completion == CompletedResult::NewLevelRecordAnd3StarsRecord))
    {
      animateLabel(_level3StarsRecordLabel);
    }
  }
  else
  {
    _level3StarsRecordLabel->setString("n/a");
  }

  _levelNameLabel->setString(levelName);
  _continueItem->setCallback(callback);
  setVisible(true);

  if (stars > 0)
  {
    for (unsigned short int startCounter = 0; startCounter < 3; ++startCounter)
    {
      const auto isGold = startCounter + 1 <= stars;

      _grayStars.at(startCounter)->setOpacity(255);

      if (startCounter == 1)
      {
        const auto starTex = "under\n " + GameUI::timeMessage(levelTimeLimit);
        _labelStars.at(startCounter)->setString(starTex);
      }

      if (isGold)
      {
        const auto starGold = _goldStars.at(startCounter);

        starGold->setColor(Color3B(0, 255, 255));

        const auto delay = DelayTime::create(0.5f + (static_cast<float>(startCounter) * 1.f));
        const auto playSound = CallFunc::create(CC_CALLBACK_0(LevelCompleted::starSound, this));

        const auto fadeIn = FadeIn::create(1.f);
        const auto appear = Sequence::create(delay, fadeIn, nullptr);
        starGold->runAction(appear);

        const auto scaleUp = ScaleTo::create(0.5f, 1.5f, 1.5f);
        const auto scaleDown = ScaleTo::create(0.5f, 1.0f, 1.0f);
        const auto scale = Sequence::create(delay->clone(), scaleUp, scaleDown, playSound->clone(), nullptr);
        starGold->runAction(scale);
      }
    }
  }
}

void LevelCompleted::hide()
{
  BaseClass::hide();
}

void LevelCompleted::starSound() const
{
  _audioHelper->playEffect("sounds/star.mp3");
}

void LevelCompleted::animateLabel(Label* label) const
{
  const auto inkDown = TintTo::create(0.5f, 180, 180, 180);
  const auto inkUp = TintTo::create(0.5f, 255, 255, 255);
  const auto sequence = Sequence::create(inkDown, inkUp, nullptr);
  const auto repeat = RepeatForever::create(sequence);
  label->runAction(repeat);
}

bool LevelCompleted::create_menu_items()
{
  auto result = false;
  do
  {
    _continueItem = add_text_button("Continue", nullptr);
    UTILS_BREAK_IF(_continueItem == nullptr);

    set_default_menu_item(_continueItem);

    result = true;
  } while (false);

  return result;
}

Label* LevelCompleted::addLabels(const std::string& labelText, const std::string& text, const Vec2& pos,
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
