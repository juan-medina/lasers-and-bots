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

    moveLeft = false;
    moveRight = false;
    moveUp = false;
    moveDown = false;

    ret = createKeybordListener();
    scheduleUpdate();

  } while (0);

  return ret;
}

void MainScene::onEnter()
{
  parent::onEnter();

  // cache
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot.plist");

  // map
  do
  {
    auto map = TMXTiledMap::create("maps/map.tmx");
    UTILS_BREAK_IF(map == nullptr);
    map->setPosition(0.0f, 0.0f);
    this->addChild(map);

    bot = Sprite::createWithSpriteFrameName("Idle_01.png");
    UTILS_BREAK_IF(bot == nullptr);
    this->addChild(bot);

    bot->setAnchorPoint(Vec2(0.5f, 0.0f));
    auto pos = this->getblockPossition(0, 2);
    auto robotPos = Vec2(pos.getMidX(), pos.getMinY());
    auto width = bot->getContentSize().width;
    auto height = bot->getContentSize().height;

    auto body = PhysicsBody::createBox(bot->getContentSize(), PhysicsMaterial(0.1, 0.0, 0.5));
    UTILS_BREAK_IF(body == nullptr);

    body->setDynamic(true);    
    body->setRotationEnable(false);
    body->setMass(1.0);
    body->setMoment(PHYSICS_INFINITY);
    bot->setPhysicsBody(body);

    bot->setPosition(robotPos);
    this->moveTo(robotPos);

    auto edge = PhysicsBody::createEdgeBox(map->getContentSize());
    map->addComponent(edge);

    initPhysics();
  } while (0);

}

Scene* MainScene::scene()
{
  Scene* result = nullptr;

  do
  {
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    UTILS_BREAK_IF(layer == nullptr);

    // 'scene' is an autorelease object
    auto scene = parent::createScene(layer);
    UTILS_BREAK_IF(scene == nullptr);

    result = scene;
  } while (0);

  // return the scene
  return result;
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
    moveUp = true;
    break;
  case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
  case EventKeyboard::KeyCode::KEY_S:
    moveDown = true;
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    moveLeft = true;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    moveRight = true;
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
    moveUp = false;
    bot->getPhysicsBody()->applyImpulse(Vec2(0.0f, 300.0f));
    break;
  case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
  case EventKeyboard::KeyCode::KEY_S:
    moveDown = false;
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    moveLeft = false;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    moveRight = false;
    break;
  default:
    break;
  }
}

void MainScene::update(float delta)
{
  /*
  auto savedPos = bot->getPosition();
  auto pos = savedPos;

  float moveX = (moveLeft ? 0.0f : 1.0f) - (moveRight ? 0.0f : 1.0f);
  float moveY = (moveDown ? 0.0f : 1.0f) - (moveUp ? 0.0f : 1.0f);
  auto move = Vec2(moveX, moveY) * (delta * 1000.f);

  pos += move;

  if (pos != savedPos)
  {
    bot->setPosition(pos);

  }*/

  auto scene = Director::getInstance()->getRunningScene();
  scene->getPhysicsWorld()->step(delta);
  this->moveTo(bot->getPosition());

}

void MainScene::initPhysics()
{
  auto scene = Director::getInstance()->getRunningScene();

  scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -100.0f));
  scene->getPhysicsWorld()->setAutoStep(false);
}
