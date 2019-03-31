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
  Scene* result = nullptr;

  do
  {
    // 'layer' is an autorelease object
    TiledScene* layer = TiledScene::create(tmxFile, minimumInchPerBlock);
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
