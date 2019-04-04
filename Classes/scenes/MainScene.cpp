#include "MainScene.h"

MainScene::MainScene() :
  _angle(0.0f),
  _draw(nullptr),
  _toLeft(false),
  _toRight(false),
  _animation(nullptr),
  _currentState(eIdle)
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
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot1.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("robot/robot2.plist");

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

bool MainScene::fuzzyEquals(const float a, const float b, float var/*=5.0f*/) const
{
  if (a - var <= b && b <= a + var)
    return true;
  return false;
}
void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
  auto y = bot->getPhysicsBody()->getVelocity().y;
  switch (keyCode)
  {
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
  case EventKeyboard::KeyCode::KEY_W:
    if (fuzzyEquals(y, 0.0f)) {
      bot->getPhysicsBody()->applyImpulse(Vec2(0.0f, _NormalMovement.y));
    }
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    bot->setFlippedX(true);
    _toLeft = true;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    bot->setFlippedX(false);
    _toRight = true;
    break;
  default:
    break;
  }
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
  switch (keyCode)
  {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
    _toLeft = false;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
    _toRight = false;
    break;
  default:
    break;
  }
}

void MainScene::changeAnim(const char* name)
{
  if (_animation != nullptr)
  {
    bot->stopAction(_animation);
    _animation = nullptr;
  }
  _animation = Animate::create(AnimationCache::getInstance()->getAnimation(name));
  bot->runAction(_animation);
}

void MainScene::update(float delta)
{
  float moveX = (_toLeft ? 0.0f : 1.0f) - (_toRight ? 0.0f : 1.0f);
  auto move = moveX * _NormalMovement.x;

  auto yVelocity = bot->getPhysicsBody()->getVelocity().y;
  auto xVelocity = bot->getPhysicsBody()->getVelocity().x;
  bot->getPhysicsBody()->setVelocity(Vec2(move, yVelocity));

  State wantedState = _currentState;


  if (fuzzyEquals(yVelocity, 0.0f) && fuzzyEquals(xVelocity, 0.0f))
  {
    wantedState = eIdle;
  }
  else
  {
    if ((fabs(yVelocity) > 10.0f) || (fabs(xVelocity) > 10.0f))
    {
      wantedState = eRunning;
      if (yVelocity > 10.0f)
      {
        wantedState = eJumping;
      }
    }
  }
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

  // calculate the maximum point that we could move  
  auto minPos = Vec2(_screenSize.width / 2, _screenSize.height / 2);
  auto maxPos = Vec2(_totalSize.width - minPos.x, _totalSize.height - minPos.y);

  // clamp to our position
  auto finalPos = bot->getPosition().getClampPoint(minPos, maxPos);

  this->getDefaultCamera()->setPosition(finalPos);
}

void MainScene::initPhysics()
{
  auto edge = PhysicsBody::createEdgeBox(this->_totalSize, PhysicsMaterial(0.1f, 0.0f, 0.5f), 5);
  this->getTiledMap()->addComponent(edge);

  //getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
  getPhysicsWorld()->setGravity(Vec2(0.0f, _Gravity));
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
    auto startRow = 2;
    auto startCol = 1;
    auto pos = this->getblockPossition(startRow, startCol);
    auto robotPos = Vec2(pos.getMinX(), pos.getMaxY()+1);
    bot->setPosition(robotPos);
    auto width = bot->getContentSize().width;
    auto height = bot->getContentSize().height;

    auto botSize = bot->getContentSize();

    botSize.width *= 0.4f;
    botSize.height *= 0.85f;

    auto body = PhysicsBody::createBox(botSize, PhysicsMaterial(0.1f, 0.0f, 0.5f));
    UTILS_BREAK_IF(body == nullptr);

    body->setDynamic(true);
    body->setRotationEnable(false);
    body->setMass(1.0);
    body->setMoment(PHYSICS_INFINITY);
    bot->setPhysicsBody(body);

    

    createAnim("Idle_%02d.png", 10, 0.05f, "idle");
    createAnim("Run_%02d.png", 8, 0.15f, "run");
    createAnim("Jump_%02d.png", 10, 0.15f, "jump", 1);

    result = true;

  } while (0);

  return result;
}

void MainScene::createAnim(const char* pattern, int maxFrame, float speed, const char* name, unsigned int loops/* = -1*/)
{
  auto cache = SpriteFrameCache::getInstance();
  Vector<SpriteFrame *> frames(maxFrame);
  for (unsigned short int num = 1; num <= maxFrame; num++)
  {
    char name[255];
    std::snprintf(name, 255, pattern, num);

    auto frame = cache->getSpriteFrameByName(name);
    frame->setAnchorPoint(Vec2(0.5f, 0.0f));
    frames.pushBack(frame);
  }

  auto anim = Animation::createWithSpriteFrames(frames);

  anim->setLoops(loops);
  anim->setDelayPerUnit(speed);

  AnimationCache::getInstance()->addAnimation(anim, name);
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
