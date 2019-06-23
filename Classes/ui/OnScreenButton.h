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
#ifndef __ON_SCREEN_BUTTON_CLASS__
#define __ON_SCREEN_BUTTON_CLASS__

#include "../utils/utils.h"

enum class ButtonType
{
  None,
  Up,
  Down,
  Left,
  Right,
  ButtonA,
  ButtonB
};

class OnScreenButton final : public Node
{
public:
  using BaseClass = Node;

  OnScreenButton();

  static OnScreenButton* create(const ButtonType& type, const std::string& spriteFrameName,
                                const std::string& label = "");

  bool init(const ButtonType& type, const std::string& spriteFrameName, const std::string& label);

  const cocos2d::Size& getContentSize() const override;

  void pushed(const bool pushed);

  inline constexpr bool isPushed() const noexcept { return _pushed; }

  void disabled(const bool disabled);

  const ButtonType& type() const { return _type; }

private:
  void onStatusChange() const;

  bool isTouchedByLocation(const Vec2& location) const;

  void onTouchesBegan(const std::vector<Touch*>& touches, Event* unusedEvent);
  void onTouchesMoved(const std::vector<Touch*>& touches, Event* unusedEvent);
  void onTouchesEnded(const std::vector<Touch*>& touches, Event* unusedEvent);
  void onTouchesCancel(const std::vector<Touch*>& touches, Event* unusedEvent);

  bool enableTouch(const bool enabled);

  Sprite* _normalSprite;
  Sprite* _pushedSprite;
  Sprite* _disabledSprite;

  Label* _labelButton;

  bool _pushed;
  bool _disabled;
  int _savedTouchId;

  EventListenerTouchAllAtOnce* _touchListener;

  ButtonType _type;
};

#endif // __ON_SCREEN_BUTTON_CLASS__
