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

#ifndef __BASIC_SCENE_H__
#define __BASIC_SCENE_H__

#include "../../utils.h"

class BasicScene : public Layer
{
public:
  // parent
  typedef Layer parent;

  // create the scene
  static Scene* createScene(BasicScene* layer);

  // init this scene
  virtual bool init();

  // foreground layer
  CC_SYNTHESIZE_READONLY(Layer*, _foregroundLayer, FgLayer);

protected:
  // init the foreground layer
  virtual bool initFgLayer();

  // store the screen size
  Size _screenSize;
};

#endif // __BASIC_SCENE_H__
