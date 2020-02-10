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

#include "VirtualJoyStick.h"

#include "../utils/controller/InputController.h"
#include "OnScreenButton.h"

VirtualJoyStick::VirtualJoyStick() : _inputController(nullptr) {}

VirtualJoyStick* VirtualJoyStick::create(InputController* inputController)
{
  VirtualJoyStick* ret = nullptr;

  do
  {
    auto object = new VirtualJoyStick();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(inputController))
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

bool VirtualJoyStick::init(InputController* inputController)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init());

    _inputController = inputController;

#if (GAME_PLATFORM == MOBILE_GAME)
    UTILS_BREAK_IF(!add_on_screen_buttons());
#endif

    ret = true;
  } while (false);

  return ret;
}

OnScreenButton* VirtualJoyStick::addOnScreenButton(const ButtonType& type, const std::string& spriteFrameName,
                                                   const std::string& label)
{
  OnScreenButton* button = nullptr;
  do
  {
    button = OnScreenButton::create(type, spriteFrameName, label);
    UTILS_BREAK_IF(button == nullptr);

    addChild(button);

    _onScreenButtons.push_back(button);
  } while (false);

  return button;
}

bool VirtualJoyStick::addOnScreenButtons()
{
  auto ret = false;
  do
  {
    const auto onScreenButtonLeft = addOnScreenButton(ButtonType::Left, "02_joystick_left");
    UTILS_BREAK_IF(onScreenButtonLeft == nullptr);
    const auto gap =
      Vec2(onScreenButtonLeft->getContentSize().width / 2, onScreenButtonLeft->getContentSize().height / 4);
    const auto leftButtonPos =
      Vec2(onScreenButtonLeft->getContentSize().width / 2, onScreenButtonLeft->getContentSize().height / 2) +
      gap;
    onScreenButtonLeft->setPosition(leftButtonPos);

    const auto onScreenButtonRight = addOnScreenButton(ButtonType::Right, "02_joystick_right");
    UTILS_BREAK_IF(onScreenButtonRight == nullptr);

    const auto rightButtonPos =
      leftButtonPos + Vec2(onScreenButtonLeft->getContentSize().width + gap.x * 2, 0.f);
    onScreenButtonRight->setPosition(rightButtonPos);

    const auto onScreenButtonA = addOnScreenButton(ButtonType::ButtonA, "02_joystick_empty", "A");
    UTILS_BREAK_IF(onScreenButtonA == nullptr);

    const auto size = Director::getInstance()->getOpenGLView()->getVisibleSize();
    const auto aButtonPos = Vec2(size.width - (onScreenButtonA->getContentSize().width / 2),
                                 onScreenButtonA->getContentSize().height / 2) +
                            Vec2(-gap.x, gap.y);
    onScreenButtonA->setPosition(aButtonPos);

    ret = true;
  } while (false);

  return ret;
}

bool VirtualJoyStick::isOnScreenPushed(const ButtonType& type) const
{
  for (const auto button : _onScreenButtons)
  {
    if (button->type() == type)
    {
      return button->isPushed();
    }
  }
  return false;
}

bool VirtualJoyStick::left() const
{
  return _inputController->left() || isOnScreenPushed(ButtonType::Left);
}

bool VirtualJoyStick::right() const
{
  return _inputController->right() || isOnScreenPushed(ButtonType::Right);
}

bool VirtualJoyStick::jump() const
{
  return _inputController->singlePressButtonA() || _inputController->up() ||
         isOnScreenPushed(ButtonType::ButtonA);
}

void VirtualJoyStick::disabled(const bool disabled)
{
  for (const auto button : _onScreenButtons)
  {
    button->disabled(disabled);
  }
}
