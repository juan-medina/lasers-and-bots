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

#include "BasicMenu.h"

#include "../ui/SliderObject.h"
#include "../ui/TextButton.h"
#include "../ui/TextToggle.h"
#include "../utils/audio/AudioHelper.h"

BasicMenu::BasicMenu()
  : _audioHelper(nullptr)
  , _currentTextButtonY(0)
  , _currentImageButtonX(0)
  , _currentImageButtonY(0)
  , _imageButtonStartX(0)
  , _moving(false)
  , _selection(nullptr)
  , _previousSelection(nullptr)
  , _selectedMenuItem(nullptr)
  , _defaultMenuItem(nullptr)
  , _selectionLocked(false)
  , _menu(nullptr)
  , _animationType(AnimationType::Slide)
{
}

bool BasicMenu::init(const std::string& name, AudioHelper* audioHelper, const float width, const float height,
                     const AnimationType animationType /*= AnimationType::Slide*/)
{
  auto ret = false;

  do
  {
    _audioHelper = audioHelper;
    _animationType = animationType;

    UTILS_BREAK_IF(!BaseClass::init(name, width, height));

    _audioHelper->preLoadEffect("sounds/select.mp3");

    const auto& size = Director::getInstance()->getVisibleSize();

    setPosition(size.width / 2, size.height / 2);

    const auto horizontalSegment = getContentSize().height;
    const auto verticalSegment = getContentSize().width;

    _imageButtonStartX = -(horizontalSegment / 2) - 25.f;
    _currentImageButtonX = _imageButtonStartX;
    _currentImageButtonY = (verticalSegment / 2) - 100.f;

    setCascadeOpacityEnabled(true);

    UTILS_BREAK_IF(!createMenuItems());

    _menu = Menu::createWithArray(_buttons);
    UTILS_BREAK_IF(_menu == nullptr);

    _menu->setPosition(0.f, -getContentSize().height / 2);

    addChild(_menu, 100);

    setOpacity(0);
    setVisible(false);

    if (_animationType == AnimationType::Slide)
    {
      setPosition(-size.width * 2, 0.f);
    }

    ret = true;
  } while (false);

  return ret;
}

void BasicMenu::display()
{
  if (_moving)
  {
    return;
  }

  _moving = true;

  static auto const TIME = 0.5f;
  const auto callBack = CallFunc::create(CC_CALLBACK_0(BasicMenu::onMovementEnd, this));

  if (_animationType == AnimationType::Slide)
  {
    const auto& size = Director::getInstance()->getVisibleSize();

    setVisible(true);
    setOpacity(190);
    const auto move = Vec2(size.width, 0);
    setPosition(Vec2(size.width / 2, size.height / 2) - move);

    const auto elasticIn = EaseElasticInOut::create(MoveBy::create(TIME * 2, move), TIME);
    const auto moveIn = Sequence::create(elasticIn, callBack, nullptr);

    runAction(moveIn);

    getAudioHelper()->playEffect("sounds/SlideClosed.mp3");
  }
  else
  {
    const auto fadeInWindow = FadeTo::create(TIME, 190);
    const auto show = Show::create();
    const auto fadeShow = Sequence::create(show, fadeInWindow, callBack, nullptr);

    runAction(fadeShow);
  }

  if ((_selectedMenuItem == nullptr) && (_defaultMenuItem != nullptr))
  {
    selectMenuItem(_defaultMenuItem);
  }
}

void BasicMenu::hide()
{
  if (_moving)
  {
    return;
  }

  _moving = true;

  static auto const TIME = 0.5f;
  const auto callBack = CallFunc::create(CC_CALLBACK_0(BasicMenu::onMovementEnd, this));

  if (_animationType == AnimationType::Slide)
  {
    const auto& size = Director::getInstance()->getVisibleSize();
    const auto move = Vec2(size.width, 0);

    const auto elasticOut = EaseElasticInOut::create(MoveBy::create(TIME * 2, move), TIME);
    const auto fade = FadeTo::create(0.0f, 0);
    const auto hide = Hide::create();

    const auto moveOut = Sequence::create(elasticOut, fade, hide, callBack, nullptr);
    runAction(moveOut);

    getAudioHelper()->playEffect("sounds/SlideClosed.mp3");
  }
  else
  {
    const auto fadeOutWindow = FadeTo::create(TIME, 0);
    const auto hide = Hide::create();
    const auto fadeHide = Sequence::create(fadeOutWindow, hide, callBack, nullptr);

    runAction(fadeHide);
  }
}

void BasicMenu::setDefaultMenuItem(MenuItem* item)
{
  _defaultMenuItem = item;
}

void BasicMenu::selectMenuItem(MenuItem* item)
{
#if (GAME_PLATFORM == DESKTOP_GAME)

  if (_selection == nullptr)
  {
    _selection = DrawNode::create(5);

    animateSelection(_selection);

    addChild(_selection);
  }

  if (_selectedMenuItem != item)
  {
    drawSelection(_selection, item, Color4F(0, 1, 1, 0.75));

    if (_selectedMenuItem != nullptr)
    {
      if (_previousSelection == nullptr)
      {
        _previousSelection = DrawNode::create(5);
        addChild(_previousSelection);
      }

      _previousSelection->stopAllActions();
      _previousSelection->setOpacity(127);
      const auto fadeOut = FadeTo::create(0.5, 0);
      _previousSelection->runAction(fadeOut);

      drawSelection(_previousSelection, _selectedMenuItem, Color4F(0, 1, 1, 0.5));
    }
    _selectedMenuItem = item;
  }

#endif
}

void BasicMenu::moveSelectionLeft()
{
  if (!_moving)
  {
    if (!_selectionLocked)
    {
      moveSelection(_compareLeft, _distanceLeft);
    }
    else if (isSelectedItemSlider())
    {
      changeSliderValue(-5);
    }
  }
}

void BasicMenu::moveSelectionRight()
{
  if (!_moving)
  {
    if (!_selectionLocked)
    {
      moveSelection(_compareRight, _distanceRight);
    }
    else if (isSelectedItemSlider())
    {
      changeSliderValue(5);
    }
  }
}

void BasicMenu::moveSelectionUp()
{
  if (!_moving)
  {
    if (!_selectionLocked)
    {
      moveSelection(_compareUp, _distanceUp);
    }
    else if (isSelectedItemSlider())
    {
      changeSliderValue(5);
    }
  }
}

void BasicMenu::moveSelectionDown()
{
  if (!_moving)
  {
    if (!_selectionLocked)
    {
      moveSelection(_compareDown, _distanceDown);
    }
    else if (isSelectedItemSlider())
    {
      changeSliderValue(-5);
    }
  }
}

void BasicMenu::activateSelection()
{
  if (!_moving)
  {
    if (_selectedMenuItem != nullptr)
    {
      if (isSelectedItemSlider())
      {
        if (!_selectionLocked)
        {
          _selectionLocked = true;
          stopSelectionAnimation(_selection);
        }
      }
      else
      {
        _selectedMenuItem->activate();
      }
    }
  }
}

void BasicMenu::selectionBack()
{
  if (!_moving)
  {
    if (_selectionLocked)
    {
      _selectionLocked = false;
      animateSelection(_selection);
    }
    else
    {
      onMenuBack();
    }
  }
}

void BasicMenu::moveTextButton(MenuItem* item)
{
  item->setPosition(0, _currentTextButtonY);

  static const auto BUTTON_GAP_Y = 150.f;
  const auto& size = item->getContentSize();
  _currentTextButtonY += (size.height + BUTTON_GAP_Y);
}

void BasicMenu::addButton(MenuItem* item, const ccMenuCallback& callback)
{
  item->setCallback(callback);
  _buttons.pushBack(item);
}

TextButton* BasicMenu::addTextButton(const std::string& text, const ccMenuCallback& callback)
{
  TextButton* result = nullptr;

  do
  {
    const auto item = TextButton::create("08_Text_", text);

    moveTextButton(item);
    addButton(item, callback);

    result = item;
  } while (false);

  return result;
}

TextToggle* BasicMenu::addToggleTextButton(const std::string& text, const ccMenuCallback& callback,
                                           const bool notMove /*= false*/)
{
  TextToggle* result = nullptr;

  do
  {
    const auto item = TextToggle::create("08_Text_", text);
    UTILS_BREAK_IF(item == nullptr);

    if (!notMove)
    {
      moveTextButton(item);
    }
    addButton(item, callback);

    result = item;
  } while (false);

  return result;
}

TextToggle* BasicMenu::addSmallToggleTextButton(const std::string& text, const ccMenuCallback& callback)
{
  TextToggle* result = nullptr;

  do
  {
    const auto item = TextToggle::create("02_joystick_empty_0", text);

    addButton(item, callback);

    result = item;
  } while (false);

  return result;
}

TextToggle* BasicMenu::addToggleImageButton(const std::string& image, const ccMenuCallback& callback)
{
  TextToggle* result = nullptr;

  do
  {
    const auto item = TextToggle::create(image, "");

    addButton(item, callback);

    result = item;
  } while (false);

  return result;
}

SliderObject* BasicMenu::addSlider(MenuItem* attachTo, const FloatCallback& callback)
{
  SliderObject* result = nullptr;

  do
  {
    const auto slider = SliderObject::create("15_slider_empty.png", "15_slider_full.png", callback);
    UTILS_BREAK_IF(slider == nullptr);

    slider->setColor(Color3B(0, 255, 255));

    const auto sliderPosition =
      Vec2(attachTo->getPosition() +
           Vec2(attachTo->getContentSize().width / 2 + slider->getContentSize().width / 2 + 110.f, 0));

    slider->setPosition(sliderPosition);

    addButton(slider, nullptr);

    result = slider;
  } while (false);

  return result;
}

MenuItem* BasicMenu::addRowLabel(const std::string& text, MenuItem* attachTo, const float leftSpace)
{
  MenuItem* result = nullptr;

  do
  {
    const auto label = Label::createWithTTF(text, "fonts/tahoma.ttf", 120);
    UTILS_BREAK_IF(label == nullptr);

    label->setTextColor(Color4B(255, 255, 255, 255));
    label->enableOutline(Color4B(0, 0, 0, 255), 5);

    auto labelItem = MenuItemLabel::create(label);
    UTILS_BREAK_IF(labelItem == nullptr);

    labelItem->setPosition(attachTo->getPosition() - Vec2(attachTo->getContentSize().width / 2 -
                                                            label->getContentSize().width / 2 + leftSpace,
                                                          -label->getContentSize().height / 4));

    addButton(labelItem, nullptr);

    result = labelItem;
  } while (false);

  return result;
}

void BasicMenu::onMovementEnd()
{
  _moving = false;
}

void BasicMenu::moveSelection(const CompareFunction& compareFun, const DistanceFunction& distanceFun)
{
  if (_selectedMenuItem == nullptr)
  {
    return;
  }

  MenuItem* newItem = nullptr;
  auto minDistance = 1000000.f;
  for (const auto child : _menu->getChildren())
  {
    const auto label = dynamic_cast<MenuItemLabel*>(child);
    if (label != nullptr)
    {
      continue;
    }
    const auto item = dynamic_cast<MenuItem*>(child);
    if (item != nullptr)
    {
      if (item != _selectedMenuItem)
      {
        if (item->isEnabled())
        {
          if (compareFun(item))
          {
            const auto distance = distanceFun(item);
            if (distance < minDistance)
            {
              minDistance = distance;
              newItem = item;
            }
          }
        }
      }
    }
  }

  if (newItem != nullptr)
  {
    selectMenuItem(newItem);
  }
}

void BasicMenu::onMenuBack() const
{
  if (_defaultMenuItem != nullptr)
  {
    _defaultMenuItem->activate();
  }
}

void BasicMenu::drawSelection(DrawNode* draw, MenuItem* item, const Color4F& color) const
{
  draw->clear();

  const auto& itemSize = item->getContentSize();
  const auto origin = item->getPosition() - (itemSize / 2) - Vec2(0, getContentSize().height / 2);
  const auto destination = origin + item->getContentSize();
  const auto gap = Vec2(10, 10);

  draw->drawRect(origin - gap, destination + gap, color);
}

void BasicMenu::animateSelection(DrawNode* draw) const
{
  const auto fadeOut = FadeTo::create(0.5, 127);
  const auto fadeIn = FadeTo::create(0.5, 255);
  const auto sequence = Sequence::create(fadeOut, fadeIn, nullptr);
  const auto repeat = RepeatForever::create(sequence);

  draw->runAction(repeat);
}

void BasicMenu::stopSelectionAnimation(DrawNode* draw)
{
  draw->stopAllActions();
  draw->setOpacity(255);
}

bool BasicMenu::isSelectedItemSlider() const
{
  return dynamic_cast<SliderObject*>(_selectedMenuItem) != nullptr;
}

void BasicMenu::changeSliderValue(const float increase) const
{
  const auto slider = dynamic_cast<SliderObject*>(_selectedMenuItem);
  const auto value = slider->getPercentage();
  slider->setPercentage(value + increase);
}
