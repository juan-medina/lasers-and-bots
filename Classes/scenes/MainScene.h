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
#ifndef _MAIN_SCENE__
#define _MAIN_SCENE__

#include "../utils/utils.h"
#include "../utils/base/scene/TiledScene.h"

class MainScene : public TiledScene
{
public:
  // parent
  typedef TiledScene parent;

  // constructor
  MainScene();

  // destructor
  ~MainScene();

  // create the object
  static MainScene* create();

  // create the scene
  static Scene* scene();

  // init this object
  virtual bool init();

protected:

private:

  bool createKeybordListener();
  void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

  virtual void update(float delta);

  Sprite *bot;

  void initPhysics();

  bool createBot();

  bool addPhysicsToMap();
  bool addBodyToSprite(Sprite* sprite);
  void createEmitter(Vec2 point);

  float _angle;
  DrawNode* _draw;

  bool _toLeft;
  bool _toRight;

  const float _Gravity = -1000.0f;
  const Vec2 _NormalMovement = Vec2(700.0f, 1200.0f);

  bool fuzzyEquals(const float a, const float b, const float var = 5.0f) const;

  Action* _animation;

  void createAnim(const char* pattern, int maxFrame, float speed, const char* name, unsigned int loops = -1);
  void changeAnim(const char* name);

  enum State
  {
    eFalling,
    eJumping,
    eIdle,
    eRunning
  };

  State _currentState;

};

#endif // _MAIN_SCENE__

