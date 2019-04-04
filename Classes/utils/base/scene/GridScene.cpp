#include "GridScene.h"

GridScene::GridScene()
{
}

GridScene::~GridScene()
{
}

GridScene* GridScene::create(const Size blocks, const Size blockSize)
{
  // create the grid
  auto scene = new GridScene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init(blocks, blockSize))
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

Scene* GridScene::scene(const Size blocks, const Size blockSize)
{
  // create the grid
  auto scene = new GridScene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init(blocks, blockSize))
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

// on "init" you need to initialize your instance
bool GridScene::init(const Size blocks, const Size blockSize)
{
  bool ret = false;

  do
  {
    //////////////////////////////
    // 1. super init first

    ret = parent::init();

    UTILS_BREAK_IF(!ret);

    // init vars
    this->_blocks = blocks;
    this->_blockSize= blockSize;

    // set the anchor
    this->setAnchorPoint(Point::ZERO);

    // calcualte total size
    _totalSize = Size(_blockSize.width * _blocks.width, _blockSize.height * _blocks.height);

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

  // we don't need anything else
  this->removeAllChildrenWithCleanup(true);
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

        // if all its ok set position and add the label
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
