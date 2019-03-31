#ifndef __TILED_SCENE_H__
#define __TILED_SCENE_H__

#include "GridScene.h"

class TiledScene : public GridScene
{
public:
  // parent
  typedef GridScene parent;

  // create with size
  static TiledScene* create(const char* tmxFile, float minimumInchPerBlock);

  // create the scene
  static Scene* scene(const char* tmxFile, float minimumInchPerBlock);

  bool init(const char* tmxFile, float minimumInchPerBlock = 0.0f);

  CC_SYNTHESIZE_READONLY(TMXTiledMap*, _tiledMap, TiledMap);
};

#endif // __TILED_SCENE_H__