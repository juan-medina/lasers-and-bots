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

#ifndef __BASICSCENE_H__
#define __BASICSCENE_H__

#include "../../utils.h"

// forward declarations
class BasicApp;
class AudioHelper;
class InputController;

class BasicScene : public Scene
{
public:
  using BaseClass = Scene;

  BasicScene();

  static BaseClass* createScene(BasicApp* application);

  bool init(BasicApp* application);

  Size getScreenSize() const noexcept { return _screenSize; }

  BasicApp* getApplication() const { return _application; }

  AudioHelper* getAudioHelper() const;
  InputController* getInputController() const;

  virtual void didEnterBackground();
  virtual void willEnterForeground();

protected:
  void onExit() override;

  Size _screenSize;

  BasicApp* _application;
};

#endif //__BASICSCENE_H__
