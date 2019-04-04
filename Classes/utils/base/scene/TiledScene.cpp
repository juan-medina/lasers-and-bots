#include "TiledScene.h"

TiledScene* TiledScene::create(const char* tmxFile)
{
  // create the game
  auto scene = new TiledScene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init(tmxFile))
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

Scene* TiledScene::scene(const char* tmxFile)
{
  // create the grid
  auto scene = new TiledScene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init(tmxFile))
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
bool TiledScene::init(const char* tmxFile)
{
  bool ret = false;

  do
  {
    auto map = experimental::TMXTiledMap::create(tmxFile);
    ret = (map != nullptr);
    UTILS_BREAK_IF(!ret);

    // store the map
    _tiledMap = map;

    auto tileSize = map->getTileSize();
    auto size = map->getMapSize();

    // init with a grid
    ret = parent::init(size, tileSize);
    UTILS_BREAK_IF(!ret);

    // add the title map to the scene
    this->addChild(map);

  } while (0);

  return ret;
}
