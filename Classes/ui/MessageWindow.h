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
#ifndef __MESSAGEWINDOW_H__
#define __MESSAGEWINDOW_H__

#include "../menu/BasicMenu.h"
#include "../utils/utils.h"

// forward declarations
class AudioHelper;

class MessageWindow final : public BasicMenu
{
public:
  using BaseClass = BasicMenu;

  MessageWindow();

  static MessageWindow* create(AudioHelper* audioHelper);

  bool init(AudioHelper* audioHelper);

  void display(const std::string& message, const std::string& subMessage, const ccMenuCallback& callback);

  void hide() override;

protected:
  bool createMenuItems() override;

private:
  AudioHelper* _audioHelper;
  TextButton* _continueItem;
  Label* _subLabel;
};

#endif //__MESSAGEWINDOW_H__
