#ifndef __GRID_SCENE_H__
#define __GRID_SCENE_H__

#include "BasicScene.h"

class GridScene : public BasicScene
{
public:
  // constructor
  GridScene();

  // destructor
  virtual ~GridScene();

  // parent
  typedef BasicScene parent;

  // create with size
  static GridScene* create(Size blocks, Size blockSize, float minimumInchPerBlock);

  // create the scene
  static Scene* scene(Size blocks, Size blockSize, float minimumInchPerBlock);

  // init this class
  bool init(Size blocks, Size blockSize, float minimumInchPerBlock = 0.0f);

  // get the position of a giving block in a col/row
  inline Rect getblockPossition(int col, int row)
  {
    return Rect(col * _blockSize.width, row * _blockSize.height, _blockSize.width, _blockSize.height);
  }

  // get the block center for a giving block in a col/row
  inline Vec2 getBlockCenter(int col, int row)
  {
    Rect possition = this->getblockPossition(col, row);

    return Vec2(possition.getMidX(), possition.getMidY());
  }

  // enter scene
  virtual void onEnter();

  // exit scene
  virtual void onExit();

  // process tocuh events
  CC_PROPERTY(bool, _enableTocuh, EnableTouch);

  // number of blocks
  CC_SYNTHESIZE(Size, _blocks, Blocks);

  // size of a block
  CC_SYNTHESIZE(Size, _blockSize, BlockSize);

  // total size of the area
  CC_SYNTHESIZE(Size, _totalSize, totalSize);

  // can we scroll?
  CC_SYNTHESIZE(bool, _canScroll, CanScroll);

protected:
  // center the grid
  void center(float time = 0.0f);

  // move by a distance
  void moveBy(Point distance, float time);

  // move to a point
  void moveTo(Point to, float time = 0.0f);

  // move to a block in a col/row
  inline void moveToBlock(int col, int row, float time)
  {
    this->moveTo(this->getBlockCenter(col, row), time);
  }

  // block tap event
  virtual void onBlockTap(int col, int row, int numTaps) {};

  // create a debug grid with a label text in the center
  bool createDebugGrid(const char* fontName);

  // Min position to move, used to clip movement
  Point _minPos;

  // Max position to move, used to clip movement
  Point _maxPos;
private:
  // set the minimum inchs per block
  void minimunInchsPerBlock(float minimun);

  // the col/row for the last block touched
  Vec2 _blockTouched;

  // get the block on a giving location
  Vec2 getBlockAtLocation(Point location);

  // location of first tap
  Point _firstTapLocation;

  // number of taps
  int _numberOfTaps;

  // begin touch event
  virtual bool onTouchBegan(Touch* touch, Event* unused_event);

  // touch moved event
  virtual void onTouchMoved(Touch* touch, Event* unused_event);

  // touch end event
  virtual void onTouchEnded(Touch* touch, Event* unused_event);

  // listener for touch events
  EventListenerTouchOneByOne* _listener;

  // stop current movement
  void stopMovement();

  // action for movements
  Action* moveAction;

  // handle taps
  virtual void tapHandler(float dt);
};

#endif // __GRID_SCENE_H__
