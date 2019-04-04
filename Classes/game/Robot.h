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
#ifndef __ROBOT_CLASS__
#define __ROBOT_CLASS__

#include "../utils/utils.h"

class Robot : public Sprite
{
public:
  // parent
  typedef Sprite parent;

  // constructor
  Robot();

  // destructor
  ~Robot();

  // create the object
  static Robot* create();

  // init this object
  virtual bool init();

  
  virtual void update(float delta);

private:

  // change robot anim
  void changeAnim(const char* name);

  // robot states
  enum State
  {
    eFalling,
    eJumping,
    eIdle,
    eRunning
  };

  // we need to move to the left
  void toLeft(bool toLeft);

  // we need to move to the right
  void toRight(bool toRight);

  // we jump
  void jump();

  //we like to move to left
  bool _toLeft;

  //we like to move to right
  bool _toRight;

  // our normal movement
  const Vec2 _NormalMovement = Vec2(700.0f, 1200.0f);

  // helper for fuzzy equals
  bool fuzzyEquals(const float a, const float b, const float var = 5.0f) const;

  // our current animation
  Action* _animation;

  // our current state
  State _currentState;

  // create a animation
  void createAnim(const char* pattern, const int maxFrame, const float speed, const char* name, unsigned const int loops = -1);

  // calculate our state
  State decideState();

  // change our state
  void changeState(State wantedState);
  
  // create a keyboard listener
  bool Robot::createKeybordListener();

  // on key press
  void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

  // on key released
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

};

#endif // __ROBOT_CLASS__

