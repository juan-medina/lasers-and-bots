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
  static GridScene* create(const Size blocks, const Size blockSize);

  // create the scene
  static Scene* scene(const Size blocks, const Size blockSize);

  // init this class
  bool init(const Size blocks, const Size blockSize);

  // get the position of a giving block in a col/row
  inline Rect getblockPossition(const int col, const int row)
  {
    return Rect(col * _blockSize.width, row * _blockSize.height, _blockSize.width, _blockSize.height);
  }

  // get the block center for a giving block in a col/row
  inline Vec2 getBlockCenter(const int col, const int row)
  {
    Rect possition = this->getblockPossition(col, row);

    return Vec2(possition.getMidX(), possition.getMidY());
  }

  // get a block in a giving location
  inline Vec2 GridScene::getBlockAtLocation(const Point location)
  {
    // calculate row and col
    int col = (int)(location.x / _blockSize.width);
    int row = (int)(location.y / _blockSize.height);

    return Vec2(col, row);
  }


  // enter scene
  virtual void onEnter() override;

  // exit scene
  virtual void onExit() override;

  // number of blocks
  CC_SYNTHESIZE_READONLY(Size, _blocks, Blocks);

  // size of a block
  CC_SYNTHESIZE_READONLY(Size, _blockSize, BlockSize);

  // total size of the area
  CC_SYNTHESIZE_READONLY(Size, _totalSize, totalSize);

protected:

  // create a debug grid with a label text in the center
  bool createDebugGrid(const char* fontName);

};

#endif // __GRID_SCENE_H__
