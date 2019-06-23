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
#ifndef __VIRTUAL_JOY_STICK_CLASS__
#define __VIRTUAL_JOY_STICK_CLASS__

#include "../utils/utils.h"

// forward declarations
class OnScreenButton;
enum class ButtonType;
class InputController;

class VirtualJoyStick final : public Node
{
public:
  using BaseClass = Node;

  VirtualJoyStick();

  static VirtualJoyStick* create(InputController* inputController);

  bool init(InputController* inputController);

  bool left() const;

  bool right() const;

  bool jump() const;

  void disabled(const bool disabled);

private:
  OnScreenButton* addOnScreenButton(const ButtonType& type, const std::string& spriteFrameName,
                                    const std::string& label = "");

  bool addOnScreenButtons();

  bool isOnScreenPushed(const ButtonType& type) const;

  std::vector<OnScreenButton*> _onScreenButtons;

  InputController* _inputController;
};

#endif // __VIRTUAL_JOY_STICK_CLASS__
