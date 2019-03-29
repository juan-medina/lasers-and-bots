/****************************************************************************
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

#ifndef __TEXT_BUTTON_H__
#define __TEXT_BUTTON_H__

#include "BaseButton.h"

class TextButton : public BaseButton
{
public:
  // parent
  typedef BaseButton parent;

  // constructor
  TextButton();

  // destructor
  ~TextButton();

  // create the object
  static TextButton* create(Sprite* sprite, Label* label, const ccMenuCallback& selector);

  // create the object
  static TextButton* createWithSpriteFrameName(const string& spriteFrameName, Label* label, const ccMenuCallback& selector);

  // create the object
  static TextButton* createWithSpriteFrameNameAndBMFont(const string& spriteFrameName, const string& bmfontPath, const string& text, const TextHAlignment& hAlignment, const ccMenuCallback& selector);

  // init this object
  virtual bool init(Sprite* sprite, Label* label, const ccMenuCallback& selector);

  // enable / disable button
  virtual void setEnabled(bool value);

  // change the button text
  void setString(const string& string);

private:
  Label* _label;
};

#endif //__TEXT_BUTTON_H__
