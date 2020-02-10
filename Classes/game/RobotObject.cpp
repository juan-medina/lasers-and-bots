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

#include "RobotObject.h"

#include "../ui/VirtualJoyStick.h"
#include "../utils/audio/AudioHelper.h"

const Vec2 RobotObject::NORMAL_MOVEMENT = Vec2(1000.0f, 2600.f);
const int RobotObject::BLINK_ON_DAMAGE_ACTION_TAG = 0xF0F0F;

RobotObject::RobotObject()
  : _toLeft(false)
  , _toRight(false)
  , _jumping(false)
  , _walkSound(-1)
  , _currentShield(0)
  , _maxShield(0)
  , _periodicDamage(0)
  , _feetTouchAnything(false)
  , _feetTouchingCount(0)
  , _currentState(e_idle)
  , _virtualJoyStick(nullptr)
  , _audioHelper(nullptr)
  , _numberOfJumps(0)
{
}

RobotObject* RobotObject::create(PhysicsShapeCache* physicsShapeCache, AudioHelper* audioHelper,
                                 VirtualJoyStick* virtualJoyStick, const int maxShield)
{
  RobotObject* ret = nullptr;

  do
  {
    auto object = new RobotObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physicsShapeCache, audioHelper, virtualJoyStick, maxShield))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  } while (false);

  return ret;
}

bool RobotObject::init(PhysicsShapeCache* physicsShapeCache, AudioHelper* audioHelper,
                       VirtualJoyStick* virtualJoyStick, const int maxShield)
{
  auto ret = false;

  do
  {
    // cache
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot_0.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot_1.plist");

    UTILS_BREAK_IF(!BaseClass::init(physicsShapeCache, "Robot", "Idle_01.png", "robot"));

    setAnchorPoint(Vec2(0.5f, 0.0f));

    auto body = getPhysicsBody();
    body->setMass(1.0f);
    body->setMoment(PHYSICS_INFINITY);

    UTILS_BREAK_IF(!createAnim("Idle_%02d.png", 10, 0.05f, "idle"));
    UTILS_BREAK_IF(!createAnim("Run_%02d.png", 8, 0.15f, "run"));
    UTILS_BREAK_IF(!createAnim("Jump_%02d.png", 10, 0.15f, "jump", 1));

    _audioHelper = audioHelper;

    _audioHelper->preLoadEffect("sounds/metal_footsteps.mp3");
    _audioHelper->preLoadEffect("sounds/jump.mp3");
    _audioHelper->preLoadEffect("sounds/land.mp3");

    changeAnim("idle");

    _virtualJoyStick = virtualJoyStick;
    _currentShield = maxShield;
    _maxShield = maxShield;

    ret = true;
  } while (false);

  return ret;
}

void RobotObject::update(float delta)
{
  moveToLeft(_virtualJoyStick->left());
  moveToRight(_virtualJoyStick->right());
  jump(_virtualJoyStick->jump());

  changeState(decideState());

  if (_periodicDamage != 0)
  {
    damageShield(_periodicDamage);
  }
}

void RobotObject::onLandOnBlock()
{
  _audioHelper->playEffect("sounds/land.mp3", false, 0.35f);
}

void RobotObject::feetTouchWalkObjectStart()
{
  _feetTouchingCount++;
  if (_feetTouchingCount > 0)
  {
    _feetTouchAnything = true;

    if (_jumping)
    {
      onLandOnBlock();
      _jumping = false;
      _numberOfJumps = 0;
    }
  }
}

void RobotObject::feetTouchWalkObjectEnd()
{
  _feetTouchingCount--;
  if (_feetTouchingCount == 0)
  {
    _feetTouchAnything = false;
  }
}

float RobotObject::getShieldPercentage() const
{
  const auto ratio = static_cast<float>(_currentShield) / _maxShield;
  const auto amount = ratio * 100.f;
  return amount;
}

RobotObject::State RobotObject::decideState() const
{
  auto wantedState = e_idle;

  if (_jumping)
  {
    wantedState = e_jumping;
  }
  else if (_toLeft || _toRight)
  {
    wantedState = e_running;
  }

  return wantedState;
}

void RobotObject::changeState(State wantedState)
{
  if (wantedState != _currentState)
  {
    _currentState = wantedState;
    switch (_currentState)
    {
      case e_jumping:
        changeAnim("jump");
        walkSound(false);
        break;
      case e_idle:
        changeAnim("idle");
        walkSound(false);
        break;
      case e_running:
        changeAnim("run");
        walkSound(true);
        break;
      default:
        break;
    }
  }
}

void RobotObject::moveRobot() const
{
  const auto moveX = ((_toLeft ? 0.0f : 1.0f) - (_toRight ? 0.0f : 1.0f)) * NORMAL_MOVEMENT.x;
  const auto moveY = getPhysicsBody()->getVelocity().y;
  getPhysicsBody()->setVelocity(Vec2(moveX, moveY));
}

void RobotObject::damageShield(const int amount)
{
  if (amount > 0)
  {
    _currentShield -= amount;
    if (_currentShield < 0)
    {
      _currentShield = 0;
    }

    if (getActionByTag(BLINK_ON_DAMAGE_ACTION_TAG) == nullptr)
    {
      const auto inkToDamaged = TintTo::create(0.1f, Color3B::RED);
      const auto inkToNormal = TintTo::create(0.1f, Color3B::WHITE);
      const auto sequence = Sequence::create(inkToDamaged, inkToNormal, nullptr);
      const auto repeat = Repeat::create(sequence, 5);
      repeat->setTag(BLINK_ON_DAMAGE_ACTION_TAG);
      runAction(repeat);
    }
  }
}

void RobotObject::startPeriodicDamage(const int amount)
{
  damageShield(amount);
  _periodicDamage = amount;
}

void RobotObject::stopPeriodicDamage(const int amount)
{
  _periodicDamage = 0;
}

void RobotObject::pause()
{
  BaseClass::pause();
  if (_walkSound != -1)
  {
    _audioHelper->stopSound(_walkSound);
    _walkSound = -1;
  }
}

void RobotObject::resume()
{
  BaseClass::resume();
}

void RobotObject::moveToLeft(const bool toLeft)
{
  if (toLeft)
  {
    setFlippedX(true);
    getPhysicsBody()->setPositionOffset(Vec2(40.0f, 0.0f));
  }

  _toLeft = toLeft;
  moveRobot();
}

void RobotObject::moveToRight(const bool toRight)
{
  if (toRight)
  {
    setFlippedX(false);
    getPhysicsBody()->setPositionOffset(Vec2::ZERO);
  }

  _toRight = toRight;
  moveRobot();
}

void RobotObject::jump(const bool toJump)
{
  static auto jumpPressed = false;
  if (toJump)
  {
    if (!jumpPressed)
    {
      auto doJump = false;

      if ((!_jumping) && (_feetTouchAnything))
      {
        _numberOfJumps = 1;
        doJump = true;
      }
      else if ((_jumping) && (_numberOfJumps == 1))
      {
        _numberOfJumps = 2;
        doJump = true;
      }

      if (doJump)
      {
        _audioHelper->playEffect("sounds/jump.mp3", false, 0.35f);
        getPhysicsBody()->applyImpulse(Vec2(0.0f, NORMAL_MOVEMENT.y));
        _jumping = true;
      }

      jumpPressed = true;
    }
  }
  else
  {
    jumpPressed = false;
  }
}

void RobotObject::walkSound(const bool active)
{
  if (active)
  {
    if (_walkSound == -1)
    {
      _walkSound = _audioHelper->playEffect("sounds/metal_footsteps.mp3", true);
    }
    else
    {
      _audioHelper->resumeSound(_walkSound);
    }
  }
  else
  {
    if (_walkSound != -1)
    {
      _audioHelper->pauseSound(_walkSound);
    }
  }
}
