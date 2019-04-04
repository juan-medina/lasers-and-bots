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

#include "Robot.h"

Robot::Robot() :
  _currentState(eIdle),
  _toLeft(false),
  _toRight(false)
{
}

Robot::~Robot()
{
}

Robot* Robot::create()
{
  Robot* ret = nullptr;

  do
  {
    auto object = new Robot();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init())
    {
      object->autorelease();
    }
    else
    {      
      delete object;
      object = nullptr;
    }

    ret = object;
  } while (0);

  // return the object
  return ret;
}

// on "init" you need to initialize your instance
bool Robot::init()
{
  bool ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first

    // cache
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot1.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot2.plist");

    UTILS_BREAK_IF(!parent::init("Idle_01.png"));

    setAnchorPoint(Vec2(0.5f, 0.0f));

    auto botSize = getContentSize();
    auto width = botSize.width;
    auto height = botSize.height;

    botSize.width *= 0.4f;
    botSize.height *= 0.85f;

    auto body = PhysicsBody::createBox(botSize, PhysicsMaterial(0.1f, 0.0f, 0.5f));
    UTILS_BREAK_IF(body == nullptr);

    body->setDynamic(true);
    body->setRotationEnable(false);
    body->setMass(1.0);
    body->setMoment(PHYSICS_INFINITY);
    setPhysicsBody(body);

    UTILS_BREAK_IF(!createAnim("Idle_%02d.png", 10, 0.05f, "idle"));
    UTILS_BREAK_IF(!createAnim("Run_%02d.png", 8, 0.15f, "run"));
    UTILS_BREAK_IF(!createAnim("Jump_%02d.png", 10, 0.15f, "jump", 1));

    UTILS_BREAK_IF(!createKeybordListener());

    ret = true;

  } while (0);

  return ret;
}

void Robot::update(float delta)
{
  auto moveX = ((_toLeft ? 0.0f : 1.0f) - (_toRight ? 0.0f : 1.0f)) * _NormalMovement.x;
  auto moveY = getPhysicsBody()->getVelocity().y;

  getPhysicsBody()->setVelocity(Vec2(moveX, moveY));

  changeState(decideState());
}

Robot::State Robot::decideState()
{
  auto velocity = getPhysicsBody()->getVelocity();

  auto wantedState = _currentState;

  if (fuzzyEquals(velocity.y, 0.0f) && fuzzyEquals(velocity.x, 0.0f))
  {
    wantedState = eIdle;
  }
  else
  {
    if ((fabs(velocity.y) > 10.0f) || (fabs(velocity.x) > 10.0f))
    {
      wantedState = eRunning;
      if (velocity.y > 10.0f)
      {
        wantedState = eJumping;
      }
    }
  }

  return wantedState;
}

void Robot::changeState(Robot::State wantedState)
{
  if (wantedState != _currentState)
  {
    _currentState = wantedState;
    switch (_currentState)
    {
    case eJumping:
      changeAnim("jump");
      break;
    case eIdle:
      changeAnim("idle");
      break;
    case eRunning:
      changeAnim("run");
      break;
    default:
      break;
    }
  }
}

void Robot::toLeft(bool toLeft)
{
  if(toLeft)
  {
    setFlippedX(true);
  }
  
  _toLeft = toLeft;
}

void Robot::toRight(bool toRight)
{
  if (toRight)
  {
    setFlippedX(false);
  }
  
  _toRight = toRight;
}

void Robot::jump()
{
  auto y = getPhysicsBody()->getVelocity().y;
  if (fuzzyEquals(y, 0.0f)) {
    getPhysicsBody()->applyImpulse(Vec2(0.0f, _NormalMovement.y));
  }
}

bool Robot::createKeybordListener()
{
  bool result = false;

  do
  {
    auto listener = EventListenerKeyboard::create();
    UTILS_BREAK_IF(listener == nullptr);

    listener->onKeyPressed = CC_CALLBACK_2(Robot::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Robot::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    result = true;
  } while (0);

  return result;
}

void Robot::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
  switch (keyCode)
  {
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
  case EventKeyboard::KeyCode::KEY_W:
    jump();
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    toLeft(true);
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    toRight(true);;
    break;
  default:
    break;
  }
}

void Robot::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
  switch (keyCode)
  {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    toLeft(false);
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    toRight(false);
    break;
  default:
    break;
  }
}
