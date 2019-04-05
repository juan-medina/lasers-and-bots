#include "MainScene.h"

MainScene::MainScene() :
  _angle(0.0f),
  _draw(nullptr)
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

  this->removeChild(_draw);
  _draw = DrawNode::create();
  _draw->setOpacity(180);

  float L = 10000.0f;
  Vec2 point1 = Vec2(_totalSize.width / 2, _totalSize.height / 2);
  Vec2 d(L * cosf(_angle), L * sinf(_angle));
  Vec2 point2 = point1 + d;

  Vec2 point3 = point2;

  PhysicsRayCastCallbackFunc func = [point1, &point3](PhysicsWorld& /*world*/, const PhysicsRayCastInfo& info, void* /*data*/)->bool
  {
    auto origin = Point(point1);
    auto newPoint = Point(info.contact);
    auto savedPoint = Point(point3);
    if (origin.distance(newPoint) < origin.distance(savedPoint))
    {
      point3 = info.contact;
    }
    return true;
  };


  getPhysicsWorld()->rayCast(func, point1, point2, &point3);
  _draw->drawSegment(point1, point3, 2, Color4F::RED);

  if ((point3.x != point2.x) & (point3.y != point2.y))
  {
    _draw->drawDot(point3, 10, Color4F::RED);
    this->createEmitter(point3);
  }

  this->addChild(_draw);

  _angle += 0.25f * (float)M_PI / 180.0f;

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
    auto pos = this->getblockPossition(startCol+1, startRow);
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


void MainScene::createEmitter(Vec2 point)
{
  auto emitter = ParticleFire::create();
  emitter->setDuration(1);
  emitter->setBlendFunc(BlendFunc::ADDITIVE);
  emitter->setColor(Color3B::RED);
  emitter->setOpacityModifyRGB(true);
  emitter->setOpacity(127);
  emitter->setPosition(point);
  emitter->setEmissionRate(10);
  emitter->setGravity(this->getPhysicsWorld()->getGravity());
  emitter->setAutoRemoveOnFinish(true);
  addChild(emitter);
}

// move the camera following the robot clamping on the map
void MainScene::updateCamera()
{
  // calculate the maximum position that we could move  
  auto minPos = Vec2(_screenSize.width / 2, _screenSize.height / 2);
  auto maxPos = Vec2(_totalSize.width - minPos.x, _totalSize.height - minPos.y);

  // move the camara to the clamped position
  auto finalPos = _robot->getPosition().getClampPoint(minPos, maxPos);
  this->getDefaultCamera()->setPosition(finalPos);
}
