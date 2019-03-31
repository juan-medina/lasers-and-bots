#include "GridScene.h"

GridScene::GridScene()
{
}

GridScene::~GridScene()
{
  // if we a listener
  if (_listener != nullptr)
  {
    // remove the listener and release it
    _eventDispatcher->removeEventListener(_listener);
    _listener->release();
    _listener = nullptr;
  }
}

GridScene* GridScene::create(Size blocks, Size blockSize, float minimumInchPerBlock)
{
  // create the grid
  auto scene = new GridScene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init(blocks, blockSize, minimumInchPerBlock))
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

Scene* GridScene::scene(Size blocks, Size blockSize, float minimumInchPerBlock)
{
  // the scene
  Scene* result = nullptr;

  do
  {
    // 'layer' is an autorelease object
    GridScene* layer = GridScene::create(blocks, blockSize, minimumInchPerBlock);
    UTILS_BREAK_IF(layer == nullptr);

    // 'scene' is an autorelease object
    auto scene = parent::createScene(layer);
    UTILS_BREAK_IF(scene == nullptr);

    result = scene;
  } while (0);

  // return the scene
  return result;
}

// on "init" you need to initialize your instance
bool GridScene::init(Size blocks, Size blockSize, float minimumInchPerBlock/* = 0.0f*/)
{
  bool ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first

    ret = parent::init();

    UTILS_BREAK_IF(!ret);

    // init vars
    _listener = nullptr;
    _enableTocuh = false;
    this->setBlocks(blocks);
    this->setBlockSize(blockSize);
    this->moveAction = nullptr;
    _numberOfTaps = 0;

    // set the anchor
    this->setAnchorPoint(Point::ZERO);

    // calcualte total size
    _totalSize = Size(_blockSize.width * _blocks.width, _blockSize.height * _blocks.height);

    // calculate the maximum point that we could move
    _maxPos = Vec2(_screenSize.width, _screenSize.height) - _totalSize;
    _minPos = Point::ZERO;

    // calculate scale the minium inchs per block if we need to
    if (minimumInchPerBlock != 0.0f) {
      this->minimunInchsPerBlock(minimumInchPerBlock);
    }

    auto size = _totalSize * this->getScale();

    // if the size is bigger than the clip
    if ((_screenSize.width < size.width) || (_screenSize.height < size.height))
    {
      _canScroll = true;
    }
    else
    {
      _canScroll = false;
      this->center();
    }

    // enable touch events
    this->setEnableTouch(true);

    ret = true;
  } while (0);

  return ret;
}

void GridScene::onEnter()
{
  // call parent
  parent::onEnter();
}

void GridScene::onExit()
{
  // call parent
  parent::onExit();

  // process touch messages
  this->setEnableTouch(false);

  // we don't need anything else
  this->removeAllChildrenWithCleanup(true);
}

bool GridScene::getEnableTouch() const
{
  // just return the value
  return _enableTocuh;
}

void GridScene::setEnableTouch(bool enable)
{
  // if not change exit
  if (enable == _enableTocuh)
  {
    return;
  }

  // stop!
  this->stopMovement();

  if (enable)
  {
    // store that we processing touchs
    _enableTocuh = true;

    // Register Touch Event
    _listener = EventListenerTouchOneByOne::create();
    _listener->retain();
    _listener->setSwallowTouches(false);

    _listener->onTouchBegan = CC_CALLBACK_2(GridScene::onTouchBegan, this);
    _listener->onTouchEnded = CC_CALLBACK_2(GridScene::onTouchEnded, this);
    _listener->onTouchMoved = CC_CALLBACK_2(GridScene::onTouchMoved, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
  }

  else
  {
    // we are not processing touchs
    _enableTocuh = false;

    // remove listener
    _eventDispatcher->removeEventListener(_listener);
    _listener->release();
    _listener = nullptr;
  }
}

bool GridScene::createDebugGrid(const char* fontName)
{
  bool ret = false;

  do
  {
    // create a draw noe
    auto draw = DrawNode::create();
    UTILS_BREAK_IF(draw == nullptr);

    // vars for drawing lines
    Point from, to;
    Point verts[4];

    // everything ok, so far
    bool allOK = true;

    // for each row and col and if we stil ok
    for (int row = 0; ((row < _blocks.height) && allOK); row++)
    {
      for (int col = 0; ((col < _blocks.width) && allOK); col++)
      {
        // get the block position and calculate quad
        auto position = this->getblockPossition(col, row);

        from = position.origin;
        to = from + position.size;

        verts[0] = from;
        verts[1] = Vec2(from.x, to.y);
        verts[2] = to;
        verts[3] = Vec2(to.x, from.y);

        // draw the quad
        draw->drawPolygon(&verts[0], 4, Color4F(0.0f, 0.0f, 0.0f, 0.25f), 2.0f, Color4F(1.0f, 1.0f, 1.0f, 0.25f));

        // create the label for this quad
        auto label = Label::createWithTTF(string_format("%d-%d", col, row).c_str(), fontName, _blockSize.width / 4);
        allOK = (label != nullptr);

        // if all its ok positio and add the label
        if (allOK)
        {
          label->setPosition(this->getBlockCenter(col, row));
          this->addChild(label);
        }
      }
    }

    UTILS_BREAK_IF(!allOK);

    // add the debug grid
    this->addChild(draw);

    ret = true;
  } while (0);

  return ret;
}

void GridScene::minimunInchsPerBlock(float minimun)
{
  // get screen dpi
  int dpi = Device::getDPI();

  // get the real screen scale
  Point realScreenScale = Vec2(Director::getInstance()->getOpenGLView()->getScaleX(), Director::getInstance()->getOpenGLView()->getScaleY());

  // calculate real pixels per block
  float realPixels = realScreenScale.getLength() * _blockSize.width;

  // calculate inches per block
  float inchsPerBlock = realPixels / dpi;

  // default scale, none
  float scale = 1.0f;

  // if the block is smaller thant half inch
  if (inchsPerBlock < minimun)
  {
    // scale to half inch
    scale /= (inchsPerBlock / minimun);
  }

  // scale it
  this->setScale(scale);

  // recalculate max pos
  _maxPos = Vec2(_screenSize.width, _screenSize.height) - (_totalSize * this->getScale());
}

Vec2 GridScene::getBlockAtLocation(Point location)
{
  // calculate row and col
  int col = (int)(location.x / _blockSize.width);
  int row = (int)(location.y / _blockSize.height);

  return Vec2(col, row);
}

bool GridScene::onTouchBegan(Touch* touch, Event* unused_event)
{
  // if touch is not enable return
  if (!_enableTocuh)
  {
    return false;
  }

  // get the location in node space
  Point pointOne = Director::getInstance()->convertToUI(touch->getLocationInView());
  Point location = this->convertToNodeSpace(pointOne);

  // store the location and the node that was touched
  _firstTapLocation = location;
  _blockTouched = this->getBlockAtLocation(location);

  return true;
}

void GridScene::onTouchMoved(Touch* touch, Event* unused_event)
{
  // do we have any touch?
  if (_numberOfTaps == 0)
  {
    // get the location in node space
    Point pointOne = Director::getInstance()->convertToUI(touch->getLocationInView());
    Point location = this->convertToNodeSpace(pointOne);

    // calculate how much we move
    Point moveDistance = location - _firstTapLocation;

    // if we move at least 50.0 points move it
    if ((moveDistance.getLength() > 50.0) && (_canScroll))
    {
      moveDistance = moveDistance * this->getScale();

      float time = moveDistance.getLength() / (1000.0f * 1);

      this->moveBy(moveDistance, time);
    }
  }
}

void GridScene::onTouchEnded(Touch* touch, Event* unused_event)
{

  // if touch is not enable return
  if (!_enableTocuh)
  {
    return;
  }

  // get the location ni node space
  Point pointOne = Director::getInstance()->convertToUI(touch->getLocationInView());
  Point location = this->convertToNodeSpace(pointOne);

  // get the block
  auto block = this->getBlockAtLocation(location);

  // if we are in the same block trigger the tap handler
  if (block == _blockTouched)
  {
    if (_numberOfTaps == 0)
    {
      this->schedule(CC_SCHEDULE_SELECTOR(GridScene::tapHandler), 0.20f, 1, 0);
    }

    _numberOfTaps++;
  }
}

void GridScene::moveBy(Point distance, float time)
{
  // stop all movement
  this->stopMovement();

  // calculate the final post that we like to move
  Point finalPos = this->getPosition() + distance;

  // clamp the point from origin to the max position
  finalPos = finalPos.getClampPoint(_minPos, _maxPos);

  //if we need to move now
  if (time == 0.0f)
  {
    this->setPosition(finalPos);
  }
  else
  {
    // calculate how we need to move now
    Point toMove = finalPos - this->getPosition();

    // if we need to move anything
    if (!((toMove.x == 0.0f) && (toMove.y == 0.0f)))
    {

      // realtime will be caculate from the original time acording the new distance
      float retalTime = (toMove.getLength() / distance.getLength()) * time;

      // create the action
      this->moveAction = MoveBy::create(retalTime, toMove);

      // tagit so we could search for it if we like to stop it
      this->moveAction->setTag(0xF00F);

      // run the action
      this->runAction(moveAction);
    }
  }
}

void GridScene::moveTo(Point to, float time/* = 0.0f*/)
{
  // get the scaled grid size
  Vec2 scaledSize = Vec2(_totalSize) * this->getScale();

  // get a ratio from a total scrolls to just our point
  float pX = to.x / _totalSize.width;
  float pY = to.y / _totalSize.height;

  // scale the distance that we want to scroll acording our ratio, its absolute from (0,0)
  Vec2 distance = Vec2(scaledSize.x * pX, scaledSize.y * pY);

  // center the distance in the screen
  distance = distance - (Vec2(_screenSize) / 2);

  // add the current position
  distance += this->getPosition();

  // move in
  this->moveBy(-distance, time);
}

void GridScene::center(float time /* = 0.0f*/)
{
  // just move to the center of our size
  this->moveTo(this->gettotalSize() / 2, time);
}

void GridScene::stopMovement()
{
  // if the action is created
  if (this->moveAction != nullptr)
  {
    // check if is attached to the object
    if (this->getActionByTag(0xF00F))
    {
      // remove it
      this->stopAction(this->moveAction);
    }
    this->moveAction = nullptr;
  }
}

void GridScene::tapHandler(float dt)
{
  // we dont want more events
  this->unschedule(CC_SCHEDULE_SELECTOR(GridScene::tapHandler));

  // single & dobule tap, launch event
  if ((_numberOfTaps == 1) || (_numberOfTaps == 2))
  {
    if (((_blockTouched.x > 0) && (_blockTouched.y > 0)) && ((_blockTouched.x < _blocks.width) && (_blockTouched.y < _blocks.height)))
    {
      this->onBlockTap((int)_blockTouched.x, (int)_blockTouched.y, _numberOfTaps);
    }
  }

  // stop counting taps
  _numberOfTaps = 0;
}
