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
#ifndef __ROBOTOBJECT_H__
#define __ROBOTOBJECT_H__

#include "../utils/physics/PhysicsGameObject.h"

// forward declarations
class VirtualJoyStick;
class AudioHelper;

class RobotObject final : public PhysicsGameObject
{
public:
  using BaseClass = PhysicsGameObject;

  RobotObject();

  static RobotObject* create(PhysicsShapeCache* physicsShapeCache, AudioHelper* audioHelper,
                             VirtualJoyStick* virtualJoyStick, int maxShield);

  bool init(PhysicsShapeCache* physicsShapeCache, AudioHelper* audioHelper, VirtualJoyStick* virtualJoyStick,
            int maxShield);

  void update(float delta) override;

  void onLandOnBlock();

  void feetTouchWalkObjectStart();
  void feetTouchWalkObjectEnd();

  float getShieldPercentage() const;

  void damageShield(int amount);

  void startPeriodicDamage(int amount);
  void stopPeriodicDamage(int amount);

  void pause() override;
  void resume() override;

private:
  enum State
  {
    e_falling,
    e_jumping,
    e_idle,
    e_running
  };

  void moveToLeft(bool toLeft);
  void moveToRight(bool toRight);
  void jump(bool toJump);

  void walkSound(bool active);

  State decideState() const;
  void changeState(State wantedState);

  void moveRobot() const;

  // our normal movement
  static const Vec2 NORMAL_MOVEMENT;
  static const int BLINK_ON_DAMAGE_ACTION_TAG;

  bool _toLeft;
  bool _toRight;
  bool _jumping;

  int _walkSound;

  int _currentShield;
  int _maxShield;

  int _periodicDamage;

  bool _feetTouchAnything;
  int _feetTouchingCount;

  State _currentState;
  VirtualJoyStick* _virtualJoyStick;
  AudioHelper* _audioHelper;

  int _numberOfJumps;
};

#endif //__ROBOTOBJECT_H__
