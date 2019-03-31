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

#include "BasicScene.h"

Scene* BasicScene::createScene()
{
  Scene* ret = nullptr;

  do
  {
    auto scene = new BasicScene();
    UTILS_BREAK_IF(scene==nullptr);

    if (scene->init())
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }

    ret = scene;
  } while (0);

  // return the scene
  return ret;
}

// on "init" you need to initialize your instance
bool BasicScene::init()
{
  bool ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first

    ret = parent::initWithPhysics();

    UTILS_BREAK_IF(!ret);

    // store the screen size
    _screenSize = Director::getInstance()->getWinSize();

    ret = true;
  } while (0);

  return ret;
}
