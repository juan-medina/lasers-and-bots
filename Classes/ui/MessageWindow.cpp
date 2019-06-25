#include "MessageWindow.h"
#include "../ui/TextButton.h"
#include "../utils/audio/AudioHelper.h"

MessageWindow::MessageWindow() : _audioHelper(nullptr), _continueItem(nullptr), _subLabel(nullptr) {}

MessageWindow* MessageWindow::create(AudioHelper* audioHelper)
{
  MessageWindow* ret = nullptr;

  do
  {
    auto object = new MessageWindow();
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

bool MessageWindow::init(AudioHelper* audioHelper)
{
  auto ret = false;

  do
  {
    _audioHelper = audioHelper;

    const auto& size = Director::getInstance()->getVisibleSize();

    const auto darkAll = LayerColor::create(Color4B(0, 0, 0, 127));
    UTILS_BREAK_IF(darkAll == nullptr);

    addChild(darkAll, 0);
    darkAll->setPosition(-size.width / 2, -size.height / 2);

    UTILS_BREAK_IF(!BaseClass::init("message", audioHelper, 1800.f, 1300.f, AnimationType::Fade));

    _subLabel = Label::createWithTTF("", "fonts/tahoma.ttf", 100);
    UTILS_BREAK_IF(_subLabel == nullptr);

    _subLabel->setTextColor(Color4B(255, 255, 255, 255));
    _subLabel->enableOutline(Color4B(0, 0, 0, 255), 5);

    _subLabel->setPosition(0.f, (getContentSize().height / 2) - 250);

    addChild(_subLabel, 100);

    ret = true;
  } while (false);
  return ret;
}

void MessageWindow::display(const std::string& message, const std::string& subMessage,
                            const ccMenuCallback& callback)
{
  BaseClass::display();

  setTitle(message);
  _subLabel->setString(subMessage);
  _continueItem->setCallback(callback);
}

void MessageWindow::hide()
{
  BaseClass::hide();
}

bool MessageWindow::createMenuItems()
{
  auto result = false;
  do
  {
    _continueItem = addTextButton("Continue", nullptr);
    UTILS_BREAK_IF(_continueItem == nullptr);

    setDefaultMenuItem(_continueItem);

    result = true;
  } while (false);

  return result;
}
