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

#include "GameScene.h"

#include "../misc/LevelManager.h"
#include "../ui/VirtualJoyStick.h"
#include "../utils/audio/AudioHelper.h"
#include "../utils/base/nodes/CustomDrawNode.h"
#include "../utils/physics/PhysicsShapeCache.h"
#include "LaserAndBotsApp.h"
#include "game/BackgroundLayer.h"
#include "game/BarrelObject.h"
#include "game/BoxObject.h"
#include "game/DoorObject.h"
#include "game/HarmObject.h"
#include "game/LaserObject.h"
#include "game/RobotFragment.h"
#include "game/RobotObject.h"
#include "game/SawObject.h"
#include "game/SwitchObject.h"
#include "ui/GameUi.h"

GameScene::GameScene() noexcept
  : _robot(nullptr)
  , _gameUi(nullptr)
  , _lastRobotPosition(Vec2::ZERO)
  , _lastCameraPosition(Vec2::ZERO)
  , _minCameraPos(Vec2::ZERO)
  , _maxCameraPos(Vec2::ZERO)
  , _paused(false)
  , _doingFinalAnim(false)
  , _doingDelayStart(false)
  , _closing(false)
  , _totalTime(0.f)
  , _timeLimit(0)
  , _levelName("")
  , _barrelCount(0)
  , _level(-1)
  , _levelManager(nullptr)
  , _musicFileName("")
  , _background(nullptr)
  , _lightsNode(nullptr)
{
}

Scene* GameScene::scene(BasicApp* application, const bool debugGrid, const bool debugPhysics, const int level)
{
  auto scene = new GameScene();

  if (scene)
  {
    if (scene->init(application, debugGrid, debugPhysics, level))
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }
  }

  return scene;
}

GameScene* GameScene::create(BasicApp* application, const bool debugGrid, const bool debugPhysics,
                             const int level)
{
  GameScene* ret = nullptr;

  do
  {
    auto object = new GameScene();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(application, debugGrid, debugPhysics, level))
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

bool GameScene::init(BasicApp* application, const bool debugGrid, const bool debugPhysics,
                     unsigned short level)
{
  auto ret = false;

  do
  {
    _level = level;

    _levelManager = dynamic_cast<LaserAndBotsApp*>(application)->getLevelManager();
    const auto levelFileMap = _levelManager->getLevelMap(_level);

    _background = BackgroundLayer::create();
    _background->setPosition(0.f, 0.f);
    addChild(_background);

    // load the map
    UTILS_BREAK_IF(!BaseClass::init(application, levelFileMap, GRAVITY, debugPhysics));

    calculateCameraBounds();

    cacheObjectsTextures();

    UTILS_BREAK_IF(!createGameUi());

    UTILS_BREAK_IF(!addObjectsToGame());

    UTILS_BREAK_IF(!cacheRobotExplosion());

    getPhysicsShapeCache()->removeAllShapes();

    if (debugGrid)
    {
      UTILS_BREAK_IF(!createDebugGrid("fonts/tahoma.ttf"));
    }

    UTILS_BREAK_IF(!createPhysicsContactsCallback());

    setMapBoundsContactsSettings();

    getLevelSettings();

    preLoadSounds();

    checkRobotMovement(0.f);

    // we start with delay
    _doingDelayStart = true;

    // start game loop
    scheduleUpdate();

    ret = true;
  } while (false);

  return ret;
}

bool GameScene::createGameUi()
{
  auto ret = false;

  do
  {
    _gameUi = GameUi::create(getAudioHelper(), getInputController(), _levelManager, _level);
    UTILS_BREAK_IF(_gameUi == nullptr);

    _gameUi->setAnchorPoint(Vec2(0.f, 0.f));

    addChild(_gameUi);

    ret = true;
  } while (false);

  return ret;
}

void GameScene::calculateCameraBounds()
{
  _minCameraPos = Vec2(_screenSize.width / 2, _screenSize.height / 2);
  _maxCameraPos = Vec2(_totalSize.width - _minCameraPos.x, _totalSize.height - _minCameraPos.y);
}

bool GameScene::createPhysicsContactsCallback()
{
  auto ret = false;

  do
  {
    auto contactListener = EventListenerPhysicsContact::create();
    UTILS_BREAK_IF(contactListener == nullptr);

    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactSeparate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    ret = true;
  } while (false);

  return ret;
}

void GameScene::setMapBoundsContactsSettings() const
{
  const auto map = getTiledMap();
  const auto edge = map->getPhysicsBody();
  edge->setCategoryBitmask(static_cast<unsigned short>(Categories::world));
  edge->setContactTestBitmask(static_cast<unsigned short>(Categories::feet));
}

void GameScene::preLoadSounds() const
{
  getAudioHelper()->preLoadMusic(_musicFileName);
  getAudioHelper()->preLoadEffect("sounds/fail.mp3");
  getAudioHelper()->preLoadEffect("sounds/victory.mp3");
  getAudioHelper()->preLoadEffect("sounds/countdown.mp3");
  getAudioHelper()->preLoadEffect("sounds/explosion.mp3");
}

void GameScene::getLevelSettings()
{
  _timeLimit = _levelManager->getLevelTimeLimit(_level);
  _levelName = _levelManager->getLevelName(_level);
  _musicFileName = _levelManager->getLevelMusic(_level);
}

void GameScene::cacheObjectsTextures()
{
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile("objects/objects.plist");
}

void GameScene::willEnterForeground()
{
  if (_paused)
  {
    _gameUi->displayPauseWindow();
  }
}

Node* GameScene::providePhysicsNode(const int gid)
{
  const auto map = getTiledMap();
  const auto gidProperties = map->getPropertiesForGID(gid);
  const auto shape = getShapeFromTileGid(gid);
  if (gidProperties.getType() == Value::Type::MAP)
  {
    const auto& valueMap = gidProperties.asValueMap();
    if (valueMap.count("damage") == 1)
    {
      const auto damage = valueMap.at("damage").asInt();
      return HarmObject::create(getPhysicsShapeCache(), shape, "dummy", damage);
    }
  }

  return PhysicsGameObject::create(getPhysicsShapeCache(), shape, "dummy");
}

void GameScene::updateGameTime(const float delta)
{
  _totalTime += delta;
  _gameUi->updateTime(_totalTime, _timeLimit);
}

bool GameScene::updateRobotShieldAndCheckIfDepleted() const
{
  const auto shieldPercentage = _robot->getShieldPercentage();
  _gameUi->setShieldPercentage(shieldPercentage);

  return shieldPercentage != 0.0f;
}

void GameScene::checkRobotMovement(const float delta)
{
  const auto newPosition = _robot->getPosition();
  if (_lastRobotPosition != newPosition)
  {
    cameraFollowRobot(newPosition, delta);

    _lastRobotPosition = newPosition;
  }
}

void GameScene::update(float delta)
{
  _gameUi->update(delta);

  if (doWeNeedGameUpdates())
  {
    updateGameTime(delta);

    if (updateRobotShieldAndCheckIfDepleted())
    {
      _robot->update(delta);

      checkRobotMovement(delta);
    }
    else
    {
      explodeRobot();
    }
  }
}

bool GameScene::addLaser(const ValueMap& values, Node* layer)
{
  auto ret = false;

  do
  {
    const auto name = values.at("name").asString();
    const auto rotation = values.at("rotation").asFloat();
    const auto rotationAngle = values.at("rotation_angle").asFloat();
    const auto position = getObjectCenterPosition(values);
    const auto damage = values.at("damage").asInt();
    const auto speed = values.at("speed").asFloat();

    auto laser = LaserObject::create(getAudioHelper(), rotation, rotationAngle, speed, damage);
    UTILS_BREAK_IF(laser == nullptr);

    laser->setPosition(position);

    layer->addChild(laser);

    _gameObjects[name] = laser;

    ret = true;
  } while (false);

  return ret;
}

bool GameScene::addRobot(const ValueMap& values, Node* layer)
{
  auto ret = false;
  do
  {
    const auto shield = values.at("shield").asInt();
    _robot =
      RobotObject::create(getPhysicsShapeCache(), getAudioHelper(), _gameUi->getVirtualJoyStick(), shield);
    UTILS_BREAK_IF(_robot == nullptr);

    auto position = getObjectCenterPosition(values);
    position.y -= (_robot->getContentSize().height) / 2;

    _robot->setPosition(position);

    layer->addChild(_robot);

    ret = true;
  } while (false);

  return ret;
}

bool GameScene::addSwitch(const ValueMap& values, Node* layer)
{
  auto ret = false;

  do
  {
    const auto name = values.at("name").asString();
    const auto target = values.at("target").asString();
    const auto isActivated = values.count("activated") == 0 ? false : values.at("activated").asBool();

    auto switchGameObject = SwitchObject::create(getPhysicsShapeCache(), target);
    UTILS_BREAK_IF(switchGameObject == nullptr);

    if (isActivated)
    {
      switchGameObject->activate();
    }

    switchGameObject->setAnchorPoint(Vec2(0.5f, 0.f));

    auto position = getObjectPosition(values);
    position.y += (values.at("height").asFloat() / 2);

    switchGameObject->setPosition(position);

    layer->addChild(switchGameObject);

    _gameObjects[name] = switchGameObject;

    ret = true;
  } while (false);

  return ret;
}

bool GameScene::addDoor(const ValueMap& values, Node* layer)
{
  auto ret = false;

  do
  {
    const auto name = values.at("name").asString();
    auto doorGameObject = DoorObject::create(getPhysicsShapeCache(), getAudioHelper());

    UTILS_BREAK_IF(doorGameObject == nullptr);

    doorGameObject->setAnchorPoint(Vec2(0.5f, 0.f));

    auto position = getObjectPosition(values);
    position.y += (values.at("height").asFloat() / 2);

    doorGameObject->setPosition(position);

    layer->addChild(doorGameObject);

    _gameObjects[name] = doorGameObject;

    ret = true;
  } while (false);

  return ret;
}

bool GameScene::addBarrel(const ValueMap& values, Node* layer)
{
  auto ret = false;

  do
  {
    _barrelCount++;

    const auto name = values.at("name").asString();
    const auto image = values.at("image").asString();
    const auto shape = values.at("shape").asString();
    const auto rotation = values.at("rotation").asFloat();

    auto barrel = BarrelObject::create(getPhysicsShapeCache(), _barrelCount, image, shape);
    UTILS_BREAK_IF(barrel == nullptr);

    const auto position = getObjectCenterPosition(values);

    barrel->setPosition(position);
    barrel->setRotation(rotation);

    layer->addChild(barrel);

    _gameObjects[name] = barrel;

    ret = true;
  } while (false);

  return ret;
}

bool GameScene::addSaw(const ValueMap& values, Node* layer)
{
  auto ret = false;

  do
  {
    const auto name = values.at("name").asString();
    const auto image = values.at("image").asString();
    const auto shape = values.at("shape").asString();
    const auto damage = values.at("damage").asInt();
    const auto rotationTime = values.at("rotation_time").asFloat();
    const auto movement = values.at("movement").asFloat();
    const auto movementTime = values.at("movement_time").asFloat();
    const auto stopTime = values.at("stop_time").asFloat();

    auto saw = SawObject::create(getPhysicsShapeCache(), image, shape, damage, rotationTime, movement,
                                 movementTime, stopTime);
    UTILS_BREAK_IF(saw == nullptr);

    const auto position = getObjectCenterPosition(values);

    saw->setPosition(position);
    saw->setAnchorPoint(Vec2(0.5f, 0.5f));

    layer->addChild(saw);

    _gameObjects[name] = saw;

    ret = true;
  } while (false);

  return ret;
}

bool GameScene::addBox(const ValueMap& values, Node* layer)
{
  auto ret = false;

  do
  {
    const auto name = values.at("name").asString();
    const auto image = values.at("image").asString();
    const auto shape = values.at("shape").asString();
    const auto rotation = values.at("rotation").asFloat();

    auto box = BoxObject::create(getPhysicsShapeCache(), image, shape);
    UTILS_BREAK_IF(box == nullptr);

    const auto position = getObjectCenterPosition(values);

    box->setPosition(position);
    box->setRotation(rotation);

    layer->addChild(box);

    _gameObjects[name] = box;

    ret = true;
  } while (false);

  return ret;
}

bool GameScene::addObject(const vector<Value>::value_type& object)
{
  auto ret = false;
  do
  {
    const auto map = getTiledMap();
    UTILS_BREAK_IF(map == nullptr);

    const auto layerWalk = map->getLayer("walk");
    UTILS_BREAK_IF(layerWalk == nullptr);

    const auto layerWalkBack = map->getLayer("walk_back");
    UTILS_BREAK_IF(layerWalkBack == nullptr);

    const auto layerFront = map->getLayer("front");
    UTILS_BREAK_IF(layerFront == nullptr);

    const auto& values = object.asValueMap();
    const auto type = values.at("type").asString();

    if (type == "laser")
    {
      UTILS_BREAK_IF(!addLaser(values, layerWalk));
    }
    else if (type == "robot")
    {
      UTILS_BREAK_IF(!addRobot(values, layerWalk));
    }
    else if (type == "saw")
    {
      UTILS_BREAK_IF(!addSaw(values, layerWalkBack));
    }
    else if (type == "barrel")
    {
      UTILS_BREAK_IF(!addBarrel(values, layerWalkBack));
    }
    else if (type == "switch")
    {
      UTILS_BREAK_IF(!addSwitch(values, layerFront));
    }
    else if (type == "door")
    {
      UTILS_BREAK_IF(!addDoor(values, layerWalkBack));
    }
    else if (type == "box")
    {
      UTILS_BREAK_IF(!addBox(values, layerWalk));
    }
    else if (type == "light")
    {
      if (_lightsNode == nullptr)
      {
        _lightsNode = CustomDrawNode::create();
        UTILS_BREAK_IF(_lightsNode == nullptr);

        _lightsNode->setBlendFunc(BlendFunc::ADDITIVE);

        layerFront->addChild(_lightsNode);
      }
      UTILS_BREAK_IF(!addLight(values));
    }

    ret = true;
  } while (false);
  return ret;
}

bool GameScene::addObjectsToGame()
{
  auto result = false;

  do
  {
    const auto map = getTiledMap();
    const auto objects = map->getObjectGroup("objects");

    for (const auto& object : objects->getObjects())
    {
      UTILS_BREAK_IF(!addObject(object));
    }

    result = true;
  } while (false);

  return result;
}

unsigned short int GameScene::calculateStars() const
{
  unsigned short int stars = 1;

  if (_totalTime <= _timeLimit)
  {
    stars = 2;
    if (_robot->getShieldPercentage() == 100.f)
    {
      stars = 3;
    }
  }

  return stars;
}

void GameScene::moveFragmentsToRobot()
{
  const auto pos = _robot->getPosition();
  const auto position = Vec2(pos.x, pos.y + _robot->getContentSize().height / 2.5);

  _robotFragments[5]->setPosition(position);
  _robotFragments[0]->setPosition(Vec2(position.x, position.y +
                                                     _robotFragments[5]->getContentSize().height / 2 +
                                                     (_robotFragments[0]->getContentSize().height / 2)));
  _robotFragments[1]->setPosition(
    Vec2(position.x - (_robotFragments[5]->getContentSize().width / 2), _robotFragments[5]->getPosition().y));
  _robotFragments[2]->setPosition(
    Vec2(position.x + (_robotFragments[5]->getContentSize().width / 2), _robotFragments[5]->getPosition().y));
  _robotFragments[3]->setPosition(
    Vec2(position.x - (_robotFragments[5]->getContentSize().width / 2),
         _robotFragments[5]->getPosition().y - (_robotFragments[5]->getContentSize().height / 2)));
  _robotFragments[4]->setPosition(
    Vec2(position.x + (_robotFragments[5]->getContentSize().width / 2),
         _robotFragments[5]->getPosition().y - (_robotFragments[5]->getContentSize().height / 2)));
}

bool GameScene::cacheRobotExplosion()
{
  auto ret = false;
  do
  {
    for (auto fragmentNumber = 1; fragmentNumber <= 6; ++fragmentNumber)
    {
      const auto robotFragment = RobotFragment::create(getPhysicsShapeCache(), fragmentNumber);
      UTILS_BREAK_IF(robotFragment == nullptr);

      _robotFragments.push_back(robotFragment);
      getTiledMap()->getLayer("walk")->addChild(robotFragment);
      robotFragment->setPosition(-10000, 0);
    }

    ret = true;
  } while (false);

  return ret;
}

void GameScene::explodeRobot()
{
  getAudioHelper()->playEffect("sounds/explosion.mp3");

  _gameUi->disableButtons(true);
  _doingFinalAnim = true;

  moveFragmentsToRobot();

  for (auto robotFragment : _robotFragments)
  {
    robotFragment->explode(_robot->getPhysicsBody()->getVelocity());
  }

  _robot->removeFromParent();
  _robot = nullptr;

  auto const delay = DelayTime::create(5.0f);
  auto const gameOverCallback = CallFunc::create(CC_CALLBACK_0(GameScene::gameOver, this, false));
  auto const delayCall = Sequence::create(delay, gameOverCallback, nullptr);

  runAction(delayCall);
}

void GameScene::gameOver(const bool win)
{
  do
  {
    pause();
    _gameUi->disableButtons(true);

    if (win)
    {
      const auto stars = calculateStars();
      const auto completion = _levelManager->setLevelCompleted(_level, stars, _totalTime);

      getAudioHelper()->playEffect("sounds/victory.mp3");
      _gameUi->displayLevelCompleted(_level, _totalTime, stars, completion,
                                     CC_CALLBACK_0(GameScene::continueButton, this));
    }
    else
    {
      getAudioHelper()->playEffect("sounds/fail.mp3");
      _gameUi->displayMessage("Game Over", "\n\n\n\n\nOops, we are going to\nneed a new robot.",
                              CC_CALLBACK_0(GameScene::reload, this));
    }
  } while (false);
}

void GameScene::delayStart()
{
  pause();
  _gameUi->disableButtons(true);

  const auto start = CallFunc::create(CC_CALLBACK_0(GameScene::start, this));
  const auto delay = DelayTime::create(4.6f);

  const auto delayStartSequence = Sequence::create(delay, start, nullptr);

  const auto count3 = CallFuncN::create(CC_CALLBACK_1(GameScene::setCountdownNumberInUi, this, 3));
  const auto count2 = CallFuncN::create(CC_CALLBACK_1(GameScene::setCountdownNumberInUi, this, 2));
  const auto count1 = CallFuncN::create(CC_CALLBACK_1(GameScene::setCountdownNumberInUi, this, 1));
  const auto count0 = CallFuncN::create(CC_CALLBACK_1(GameScene::setCountdownNumberInUi, this, 0));
  const auto countGo = CallFuncN::create(CC_CALLBACK_1(GameScene::setCountdownNumberInUi, this, -1));

  const auto countSequence =
    Sequence::create(count3, DelayTime::create(1.f), count2, DelayTime::create(1.f), count1,
                     DelayTime::create(1.f), count0, DelayTime::create(1.f), countGo, nullptr);

  getAudioHelper()->playEffect("sounds/countdown.mp3");

  runAction(delayStartSequence);
  runAction(countSequence);
}

void GameScene::setCountdownNumberInUi(Ref* sender, const int value) const
{
  _gameUi->updateCountdown(value);
}

void GameScene::start()
{
  getAudioHelper()->playMusic(_musicFileName);

  resume();
  _gameUi->disableButtons(false);
}

void GameScene::close()
{
  if (_closing)
  {
    return;
  }

  _closing = true;
  pause();
  const auto application = dynamic_cast<LaserAndBotsApp*>(getApplication());
  application->toMainMenu();
}

void GameScene::pause()
{
  _paused = true;
  BaseClass::pause();

  if (_closing)
  {
    return;
  }

  _gameUi->changePauseButton();

  getPhysicsWorld()->setAutoStep(false);

  if (_robot != nullptr)
  {
    _robot->pause();
  }

  for (const auto& gameObject : _gameObjects)
  {
    gameObject.second->pause();
  }

  getAudioHelper()->pauseMusic();

  _gameUi->getVirtualJoyStick()->pause();

  if (_doingFinalAnim)
  {
    for (auto robotFragment : _robotFragments)
    {
      robotFragment->pause();
    }
  }
}

void GameScene::resume()
{
  BaseClass::resume();

  getPhysicsWorld()->setAutoStep(true);

  if (_robot != nullptr)
  {
    _robot->resume();
  }

  for (const auto& gameObject : _gameObjects)
  {
    gameObject.second->resume();
  }

  getAudioHelper()->resumeMusic();

  _gameUi->getVirtualJoyStick()->resume();

  _paused = false;
}

void GameScene::togglePause()
{
  if (_paused)
  {
    resume();
  }
  else
  {
    pause();
  }
}

void GameScene::reload()
{
  _closing = true;

  pause();

  _gameUi->disableButtons(true);

  auto app = dynamic_cast<LaserAndBotsApp*>(_application);
  app->toGame(_level);
}

void GameScene::continueButton()
{
  _closing = true;

  pause();

  _gameUi->disableButtons(true);

  auto app = dynamic_cast<LaserAndBotsApp*>(_application);
  app->toPlayMenu(_levelManager->getNextLevel(_level));
}

void GameScene::onEnter()
{
  BaseClass::onEnter();

  if (_doingDelayStart)
  {
    delayStart();

    _doingDelayStart = false;
  }
}

void GameScene::updateUiPosition(const Vec2& finalPos) const
{
  const auto uiPos = Vec2(finalPos.x - (_screenSize.width / 2), finalPos.y - (_screenSize.height / 2));
  _gameUi->setPosition(uiPos);
}

void GameScene::updateBackgroundPosition(const Vec2& finalPos) const
{
  _background->updateScroll(finalPos);
}

void GameScene::cameraFollowRobot(const Vec2& robotPosition, const float delta)
{
  // move the camera to the clamped position
  const auto clampY = clampf(robotPosition.y, _minCameraPos.y, _maxCameraPos.y);
  const auto finalPos = Vec2(robotPosition.x, clampY);

  if (finalPos != _lastCameraPosition)
  {
    getDefaultCamera()->setPosition(finalPos);
    getDefaultCamera()->update(delta);

    updateUiPosition(finalPos);
    updateBackgroundPosition(finalPos);

    _lastCameraPosition = finalPos;
  }
}

void GameScene::switchActivateDoor(DoorObject* door)
{
  if (door->isUnactivated())
  {
    door->activate();
  }
}

void GameScene::switchActivateSwitch(SwitchObject* switchObject)
{
  if (switchObject->isUnactivated())
  {
    getAudioHelper()->playEffect("sounds/metal_click.mp3");
    switchObject->activate();
  }
}

void GameScene::switchActivateTarget(GameObject* target)
{
  const auto type = target->getType();
  if (type == "switch")
  {
    switchActivateSwitch(dynamic_cast<SwitchObject*>(target));
  }
  else if (type == "door")
  {
    switchActivateDoor(dynamic_cast<DoorObject*>(target));
  }
}

bool GameScene::isSwitchTargetingASwitch(SwitchObject* switchObject)
{
  const auto target = switchObject->getTarget();
  if (_gameObjects.count(target) == 1)
  {
    return _gameObjects.at(target)->getType() == "switch";
  }
  return false;
}

void GameScene::robotTouchSwitch(SwitchObject* switchObject)
{
  if (switchObject->isActivated())
  {
    if (switchObject->isOff())
    {
      switchObject->on();
      if (isSwitchTargetingASwitch(switchObject))
      {
        getAudioHelper()->playEffect("sounds/metal_click.mp3");
      }

      const auto target = switchObject->getTarget();
      if (_gameObjects.count(target) == 1)
      {
        const auto targetObject = _gameObjects.at(target);
        switchActivateTarget(targetObject);
      }
    }
  }
}

void GameScene::robotTouchDoor(DoorObject* doorGameObject)
{
  if (doorGameObject->isActivated())
  {
    if (doorGameObject->isOff())
    {
      doorGameObject->on();
    }
    else
    {
      gameOver(true);
    }
  }
}

void GameScene::robotTouchHarmObjectStart(HarmObject* harmObject) const
{
  _robot->startPeriodicDamage(harmObject->getDamage());
}

void GameScene::robotTouchHarmObjectEnd(HarmObject* harmObject) const
{
  _robot->stopPeriodicDamage(harmObject->getDamage());
}

void GameScene::robotTouchObjectStart(const PhysicsContact& contact)
{
  const auto door = getObjectFromContact<DoorObject>(contact, Categories::door);
  if (door != nullptr)
  {
    robotTouchDoor(door);
  }
  else
  {
    const auto switchGameObject = getObjectFromContact<SwitchObject>(contact, Categories::switches);
    if (switchGameObject != nullptr)
    {
      robotTouchSwitch(switchGameObject);
    }
    else
    {
      const auto harmGameObject = getObjectFromContact<HarmObject>(contact, Categories::harm);
      if (harmGameObject != nullptr)
      {
        robotTouchHarmObjectStart(harmGameObject);
      }
    }
  }
}

void GameScene::robotTouchObjectEnd(const PhysicsContact& contact) const
{
  const auto harmGameObject = getObjectFromContact<HarmObject>(contact, Categories::harm);
  if (harmGameObject != nullptr)
  {
    robotTouchHarmObjectEnd(harmGameObject);
  }
}

void GameScene::feetTouchObjectStart(const PhysicsContact& contact) const
{
  const auto blockGameObject = getObjectFromContact<GameObject>(contact, Categories::walkOn);
  if (blockGameObject != nullptr)
  {
    _robot->feetTouchWalkObjectStart();
  }
  else
  {
    const auto map = getObjectFromContact<experimental::TMXTiledMap>(contact, Categories::world);
    if (map != nullptr)
    {
      _robot->feetTouchWalkObjectStart();
    }
  }
}

void GameScene::feetTouchObjectEnd(const PhysicsContact& contact) const
{
  const auto blockGameObject = getObjectFromContact<GameObject>(contact, Categories::walkOn);
  if (blockGameObject != nullptr)
  {
    _robot->feetTouchWalkObjectEnd();
  }
  else
  {
    const auto map = getObjectFromContact<experimental::TMXTiledMap>(contact, Categories::world);
    if (map != nullptr)
    {
      _robot->feetTouchWalkObjectEnd();
    }
  }
}

bool GameScene::onContactBegin(PhysicsContact& contact)
{
  if (doWeNeedGameUpdates())
  {
    const auto robot = getObjectFromContact<RobotObject>(contact, Categories::robot);
    if (robot != nullptr)
    {
      robotTouchObjectStart(contact);
    }
    else
    {
      const auto feetNode = getObjectFromContact<RobotObject>(contact, Categories::feet);
      if (feetNode != nullptr)
      {
        feetTouchObjectStart(contact);
      }
    }
  }

  return true;
}

void GameScene::onContactSeparate(PhysicsContact& contact) const
{
  if (doWeNeedGameUpdates())
  {
    const auto robot = getObjectFromContact<RobotObject>(contact, Categories::robot);
    if (robot != nullptr)
    {
      robotTouchObjectEnd(contact);
    }
    else
    {
      const auto feetNode = getObjectFromContact<RobotObject>(contact, Categories::feet);
      if (feetNode != nullptr)
      {
        feetTouchObjectEnd(contact);
      }
    }
  }
}

template <class Type>
Type* GameScene::getObjectFromContact(const PhysicsContact& contact, const Categories category)
{
  Type* object = nullptr;
  const auto shortCategory = static_cast<unsigned short>(category);
  if (contact.getShapeA()->getCategoryBitmask() & shortCategory)
  {
    object = dynamic_cast<Type*>(contact.getShapeA()->getBody()->getNode());
    CCASSERT(object != nullptr, "contact dynamic_cast fail");
  }
  else if (contact.getShapeB()->getCategoryBitmask() & shortCategory)
  {
    object = dynamic_cast<Type*>(contact.getShapeB()->getBody()->getNode());
    CCASSERT(object != nullptr, "contact dynamic_cast fail");
  }

  return object;
}

bool GameScene::addLight(const ValueMap& values) const
{
  auto ret = false;
  do
  {
    const auto width = values.at("width").asFloat();
    const auto height = values.at("height").asFloat();
    const auto x = values.at("x").asFloat();
    const auto y = values.at("y").asFloat() + height;

    Vec2 points[6];
    Color4F colors[6];

    points[0] = Vec2(x, y);
    points[1] = Vec2(x + width, y);
    points[2] = Vec2(0.5f, -0.5f) * LIGHT_DISTANCE + points[0];
    points[3] = Vec2(0.5f, -0.5f) * LIGHT_DISTANCE + points[1];
    points[4] = Vec2(x + width, y + height);
    points[5] = Vec2(0.5f, -0.5f) * LIGHT_DISTANCE + points[4];
    points[5] = Vec2::getIntersectPoint(points[3], points[3] + Vec2(LIGHT_DISTANCE, 0), points[4], points[5]);

    colors[0] = Color4F(1.f, 1.f, 1.f, 0.65f);
    colors[1] = Color4F(1.f, 1.f, 1.f, 0.5f);
    colors[2] = Color4F(1.f, 1.f, 1.f, 0.f);
    colors[3] = Color4F(1.f, 1.f, 1.f, 0.f);
    colors[4] = Color4F(1.f, 1.f, 1.f, 0.5f);
    colors[5] = Color4F(1.f, 1.f, 1.f, 0.f);

    const Vec2 poly1Vertex[] = {points[0], points[1], points[3], points[2]};
    const Color4F poly1VertexColors[] = {colors[0], colors[1], colors[3], colors[2]};

    _lightsNode->drawColorQuad(&poly1Vertex[0], &poly1VertexColors[0]);

    const Vec2 poly2Vertex[] = {points[1], points[4], points[5], points[3]};
    const Color4F poly2VertexColors[] = {colors[1], colors[4], colors[5], colors[3]};

    _lightsNode->drawColorQuad(&poly2Vertex[0], &poly2VertexColors[0]);

    ret = true;
  } while (false);

  return ret;
}
