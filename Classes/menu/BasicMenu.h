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

#ifndef __BASICMENU_H__
#define __BASICMENU_H__

#include "../ui/ResizableWindow.h"
#include "../utils/utils.h"

// forward declarations
class AudioHelper;
class SliderObject;
class TextButton;
class TextToggle;

class BasicMenu : public ResizableWindow
{
public:
  enum class AnimationType
  {
    Slide,
    Fade
  };

  using BaseClass = ResizableWindow;
  using FloatCallback = std::function<void(const float)>;

  BasicMenu();

  bool init(const std::string& name, AudioHelper* audioHelper, float width, float height,
            AnimationType animationType = AnimationType::Slide);

  virtual void display();

  virtual void hide();

  AudioHelper* getAudioHelper() const { return _audioHelper; }

  void setDefaultMenuItem(MenuItem* item);
  void selectMenuItem(MenuItem* item);

  MenuItem* getSelectedMenuItem() const { return _selectedMenuItem; }

  void moveSelectionLeft();
  void moveSelectionRight();
  void moveSelectionUp();
  void moveSelectionDown();

  void activateSelection();
  void selectionBack();

protected:
  virtual bool createMenuItems() = 0;

  void moveTextButton(MenuItem* item);
  void addButton(MenuItem* item, const ccMenuCallback& callback);

  TextButton* addTextButton(const std::string& text, const ccMenuCallback& callback);
  TextToggle* addToggleTextButton(const std::string& text, const ccMenuCallback& callback,
                                  bool notMove = false);
  TextToggle* addSmallToggleTextButton(const std::string& text, const ccMenuCallback& callback);
  TextToggle* addToggleImageButton(const std::string& image, const ccMenuCallback& callback);
  SliderObject* addSlider(MenuItem* attachTo, const FloatCallback& callback);
  MenuItem* addRowLabel(const std::string& text, MenuItem* attachTo, float leftSpace);

private:
  void onMovementEnd();

  AudioHelper* _audioHelper;
  Vector<MenuItem*> _buttons;
  float _currentTextButtonY;
  float _currentImageButtonX;
  float _currentImageButtonY;
  float _imageButtonStartX;

  bool _moving;

  DrawNode* _selection;
  DrawNode* _previousSelection;

  MenuItem* _selectedMenuItem;
  MenuItem* _defaultMenuItem;
  bool _selectionLocked;

  Menu* _menu;

  using CompareFunction = std::function<bool(MenuItem*)>;
  using DistanceFunction = std::function<float(MenuItem*)>;

  const CompareFunction _compareUp = [&](MenuItem* item) {
    return item->getPosition().y > this->_selectedMenuItem->getPosition().y;
  };

  const CompareFunction _compareDown = [&](MenuItem* item) {
    return item->getPosition().y < this->_selectedMenuItem->getPosition().y;
  };

  const CompareFunction _compareRight = [&](MenuItem* item) {
    return item->getPosition().x > this->_selectedMenuItem->getPosition().x;
  };

  const CompareFunction _compareLeft = [&](MenuItem* item) {
    return item->getPosition().x < this->_selectedMenuItem->getPosition().x;
  };

  const DistanceFunction _distanceUp = [&](MenuItem* item) {
    const auto pos1 = item->getPosition() + Vec2(0, item->getContentSize().height / 2);
    const auto pos2 =
      this->_selectedMenuItem->getPosition() - Vec2(0, this->_selectedMenuItem->getContentSize().width / 2);
    return pos1.distance(pos2);
  };

  const DistanceFunction _distanceDown = [&](MenuItem* item) {
    const auto pos1 = item->getPosition() - Vec2(0, item->getContentSize().height / 2);
    const auto pos2 =
      this->_selectedMenuItem->getPosition() + Vec2(0, this->_selectedMenuItem->getContentSize().width / 2);
    return pos1.distance(pos2);
  };

  const DistanceFunction _distanceRight = [&](MenuItem* item) {
    const auto pos1 = item->getPosition() - Vec2(item->getContentSize().width / 2, 0);
    const auto pos2 =
      this->_selectedMenuItem->getPosition() + Vec2(this->_selectedMenuItem->getContentSize().width / 2, 0);
    return pos1.distance(pos2);
  };

  const DistanceFunction _distanceLeft = [&](MenuItem* item) {
    const auto pos1 = item->getPosition() + Vec2(item->getContentSize().width / 2, 0);
    const auto pos2 =
      this->_selectedMenuItem->getPosition() - Vec2(this->_selectedMenuItem->getContentSize().width / 2, 0);
    return pos1.distance(pos2);
  };

  void moveSelection(const CompareFunction& compareFun, const DistanceFunction& distanceFun);

  void onMenuBack() const;

  void drawSelection(DrawNode* draw, MenuItem* item, const Color4F& color) const;
  void animateSelection(DrawNode* draw) const;
  static void stopSelectionAnimation(DrawNode* draw);

  bool isSelectedItemSlider() const;

  void changeSliderValue(float increase) const;

  AnimationType _animationType;
};

#endif //__BASICMENU_H__
