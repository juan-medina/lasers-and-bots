#include "TiledScene.h"

TiledScene* TiledScene::create(const char* tmxFile, float minimumInchPerBlock)
{
  // create the game
  auto scene = new TiledScene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init(tmxFile, minimumInchPerBlock))
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

Scene* TiledScene::scene(const char* tmxFile, float minimumInchPerBlock)
{
  // create the grid
  auto scene = new TiledScene();

  // init the scene and auto release
  if (scene)
  {
    if (scene->init(tmxFile, minimumInchPerBlock))
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
bool TiledScene::init(const char* tmxFile, float minimumInchPerBlock/* = 0.0f*/)
{
  bool ret = false;

  do
  {
    auto map = TMXTiledMap::create(tmxFile);
    ret = (map != nullptr);
    UTILS_BREAK_IF(!ret);

    // store the map
    _tiledMap = map;

    auto tileSize = map->getTileSize();
    auto size = map->getMapSize();

    // init with a grid of 50/50 and 0.5f of mimium block inchs
    ret = parent::init(size, tileSize, minimumInchPerBlock);
    UTILS_BREAK_IF(!ret);

    // add the title map to the scene
    this->addChild(map);
  } while (0);

  return ret;
}
