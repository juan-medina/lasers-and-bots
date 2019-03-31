#include "MainScene.h"

MainScene::MainScene()
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

    // cache
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot.plist");

    //create bot
    UTILS_BREAK_IF(!this->createBot());

    //init physics
    initPhysics();

    // add physics to map
    UTILS_BREAK_IF(!addPhysicsToMap());

    // init keyboard
    UTILS_BREAK_IF(!createKeybordListener());

    //get updates
    scheduleUpdate();

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

bool MainScene::createKeybordListener()
{
  bool result = false;

  do
  {
    auto listener = EventListenerKeyboard::create();
    UTILS_BREAK_IF(listener == nullptr);

    listener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    result = true;
  } while (0);

  return result;
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
  switch (keyCode)
  {
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
  case EventKeyboard::KeyCode::KEY_W:
    break;
  case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
  case EventKeyboard::KeyCode::KEY_S:
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    break;
  default:
    break;
  }
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
  switch (keyCode)
  {
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
  case EventKeyboard::KeyCode::KEY_W:
    bot->getPhysicsBody()->applyImpulse(Vec2(0.0f, 1000.0f));
    break;
  case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
  case EventKeyboard::KeyCode::KEY_S:
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    bot->getPhysicsBody()->applyImpulse(Vec2(-300.0f, 0.0f));
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    bot->getPhysicsBody()->applyImpulse(Vec2(300.0f, 0.0f));
    break;
  default:
    break;
  }
}

void MainScene::update(float delta)
{
  getPhysicsWorld()->step(delta);
  this->moveTo(bot->getPosition());

}

void MainScene::initPhysics()
{
  auto edge = PhysicsBody::createEdgeBox(this->_totalSize);
  this->getTiledMap()->addComponent(edge);

  //getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
  getPhysicsWorld()->setGravity(Vec2(0.0f, -1000.0f));
  getPhysicsWorld()->setAutoStep(false);
}

bool MainScene::createBot()
{
  bool result = false;

  do
  {
    bot = Sprite::createWithSpriteFrameName("Idle_01.png");
    UTILS_BREAK_IF(bot == nullptr);
    this->addChild(bot);

    bot->setAnchorPoint(Vec2(0.5f, 0.0f));
    auto startRow = (_blocks.width / 2.0f) - 1;
    auto startCol = (_blocks.height / 2.0f) - 1;
    auto pos = this->getblockPossition(startRow, startCol);
    auto robotPos = Vec2(pos.getMidX(), pos.getMinY());
    auto width = bot->getContentSize().width;
    auto height = bot->getContentSize().height;

    auto botSize = bot->getContentSize();

    botSize.width *= 0.4;
    botSize.height *= 0.85;

    auto body = PhysicsBody::createBox(botSize, PhysicsMaterial(0.1f, 0.0f, 0.5f));
    UTILS_BREAK_IF(body == nullptr);

    body->setDynamic(true);
    body->setRotationEnable(false);
    body->setMass(1.0);
    body->setMoment(PHYSICS_INFINITY);
    bot->setPhysicsBody(body);

    bot->setPosition(robotPos);
    this->moveTo(robotPos);

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
    auto body = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.5f));
    UTILS_BREAK_IF(body == nullptr);

    body->setDynamic(false);
    sprite->setPhysicsBody(body);

    result = true;
  } while (0);

  return result;
}
