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

#include "MainScene.h"
#include "../game/Robot.h"
#include "../game/Laser.h"

MainScene::MainScene() :
  _robot(nullptr)
{
}

MainScene::~MainScene()
{
}

MainScene* MainScene::create()
{
  MainScene* ret = nullptr;

  do
  {
    auto object = new MainScene();
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
bool MainScene::init()
{
  bool ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first
    UTILS_BREAK_IF(!parent::init("maps/map.tmx"));

    //create bot
    UTILS_BREAK_IF(!this->createBot());

    //init physics
    initPhysics();

    // add physics to map
    UTILS_BREAK_IF(!addPhysicsToMap());

    // add a laser
    UTILS_BREAK_IF(!addLaserAtBlock(getBlocks().width / 2, getBlocks().height / 2));

    //get updates
    scheduleUpdate();

    auto debugGrid = UserDefault::getInstance()->getBoolForKey("debugGrid", false);
    if (debugGrid)
    {
      UTILS_BREAK_IF(!createDebugGrid("fonts/Marker Felt.ttf"));
    }

    ret = true;

  } while (0);

  return ret;
}

Scene* MainScene::scene()
{
  // create the grid
  auto scene = new MainScene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init())
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }
  }

  // return the scene
  return scene;
}

void MainScene::update(float delta)
{
  _robot->update(delta);


  updateCamera();
}

void MainScene::initPhysics()
{
  auto edge = PhysicsBody::createEdgeBox(this->_totalSize, PhysicsMaterial(0.1f, 0.0f, 0.5f), 5);
  this->getTiledMap()->addComponent(edge);

  auto debugPhysics = UserDefault::getInstance()->getBoolForKey("debugPhysics", false);
  if (debugPhysics)
  {
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
  }

  getPhysicsWorld()->setGravity(Vec2(0.0f, _Gravity));
  getPhysicsWorld()->setSubsteps(4);
}

bool MainScene::createBot()
{
  bool result = false;

  do
  {
    _robot = Robot::create();
    UTILS_BREAK_IF(_robot == nullptr);
    this->addChild(_robot);

    auto startRow = getTiledMap()->getProperty("startX").asInt();
    auto startCol = getTiledMap()->getProperty("startY").asInt();
    auto pos = this->getblockPossition(startCol + 1, startRow);
    auto blockSize = getBlockSize();
    auto robotPos = Vec2(pos.getMinX() - (blockSize.width / 2), pos.getMinY() - (blockSize.height / 4));
    _robot->setPosition(robotPos);

    result = true;

  } while (0);

  return result;
}

bool MainScene::addPhysicsToMap()
{
  bool result = false;

  do
  {
    auto map = this->getTiledMap();

    auto layer = map->getLayer("walk");
    UTILS_BREAK_IF(layer == nullptr);

    for (int col = 0; col < this->_blocks.height; col++)
    {
      for (int row = 0; row < this->_blocks.width; row++)
      {
        auto sprite = layer->getTileAt(Vec2(row, col));
        if (sprite != nullptr) {
          addBodyToSprite(sprite);
        }
      }
    }


    result = true;
  } while (0);

  return result;
}

bool MainScene::addBodyToSprite(Sprite* sprite)
{
  bool result = false;

  do
  {
    auto mat = PhysicsMaterial(0.0f, 0.0f, 0.0f);
    auto body = PhysicsBody::createBox(sprite->getContentSize(), mat);
    UTILS_BREAK_IF(body == nullptr);

    body->setDynamic(false);
    sprite->setPhysicsBody(body);

    result = true;
  } while (0);

  return result;
}

void MainScene::updateCamera()
{
  // calculate the maximum position that we could move  
  auto minPos = Vec2(_screenSize.width / 2, _screenSize.height / 2);
  auto maxPos = Vec2(_totalSize.width - minPos.x, _totalSize.height - minPos.y);

  // move the camara to the clamped position
  auto finalPos = _robot->getPosition().getClampPoint(minPos, maxPos);
  this->getDefaultCamera()->setPosition(finalPos);
}


bool MainScene::addLaserAtBlock(const int col, const int row)
{
  bool ret = false;

  do
  {
    auto pos = getblockPossition(col, row);

    auto laserOrigin = Vec2(pos.getMinX() + getBlockSize().width / 2, pos.getMinY() + +getBlockSize().height / 2);

    auto laser = Laser::create();
    UTILS_BREAK_IF(laser == nullptr);
    laser->setPosition(laserOrigin);

    addChild(laser);
    
    ret = true;

  } while (0);

  return ret;
}
