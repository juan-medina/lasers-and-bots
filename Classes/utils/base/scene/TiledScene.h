#ifndef __TILED_SCENE_H__
#define __TILED_SCENE_H__

#include "GridScene.h"

class TiledScene : public GridScene
{
public:
  // parent
  typedef GridScene parent;

  // create with a tmx file
  static TiledScene* create(const char* tmxFile);

  // create the scene
  static Scene* scene(const char* tmxFile);

  // init the scene
  bool init(const char* tmxFile);

  CC_SYNTHESIZE_READONLY(experimental::TMXTiledMap*, _tiledMap, TiledMap);
};

#endif // __TILED_SCENE_H__
