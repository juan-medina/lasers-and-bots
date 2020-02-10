/****************************************************************************
 Copyright (c) 2019 Juan Medina

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "PhysicsTiledScene.h"

#include "PhysicsShapeCache.h"

PhysicsTiledScene::PhysicsTiledScene() : _physicsShapeCache(nullptr) {}

PhysicsTiledScene::~PhysicsTiledScene()
{
  _physicsShapeCache->removeAllShapes();
  delete _physicsShapeCache;
  _physicsShapeCache = nullptr;
}

PhysicsTiledScene* PhysicsTiledScene::create(BasicApp* application, const std::string& tmxFile,
                                             const float gravity, const bool debugPhysics)
{
  auto scene = new PhysicsTiledScene();

  if (scene != nullptr)
  {
    if (scene->init(application, tmxFile, gravity, debugPhysics))
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }
  }

  return scene;
}

Scene* PhysicsTiledScene::scene(BasicApp* application, const std::string& tmxFile, const float gravity,
                                const bool debugPhysics)
{
  auto scene = new PhysicsTiledScene();

  if (scene != nullptr)
  {
    if (scene->init(application, tmxFile, gravity, debugPhysics))
    {
      scene->autorelease();
    }
    else
    {
      delete scene;
      scene = nullptr;
    }
  }

  return scene;
}

bool PhysicsTiledScene::init(BasicApp* application, const std::string& tmxFile, const float gravity,
                             const bool debugPhysics)
{
  auto ret = false;

  do
  {
    ret = BaseClass::init(application, tmxFile);
    UTILS_BREAK_IF(!ret);

    _gravity = gravity;

    initPhysics(debugPhysics);

    UTILS_BREAK_IF(!addPhysicsToMap());
  } while (false);

  return ret;
}

void PhysicsTiledScene::initPhysics(const bool debugPhysics) const
{
  const auto edge = PhysicsBody::createEdgeBox(_totalSize, PhysicsMaterial(0.1f, 0.0f, 0.5f), 5);
  getTiledMap()->addComponent(edge);

  if (debugPhysics)
  {
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
  }

  getPhysicsWorld()->setGravity(Vec2(0.0f, _gravity));
  getPhysicsWorld()->setSubsteps(4);
}

string PhysicsTiledScene::getShapeFromTileGid(const int gid)
{
  if (_gidToShapes.count(gid) == 0)
  {
    string shape;
    const auto map = getTiledMap();
    if (map->getPropertiesForGID(gid).getType() == Value::Type::MAP)
    {
      const auto properties = map->getPropertiesForGID(gid).asValueMap();
      if (properties.count("shape") == 1)
      {
        shape = properties.at("shape").asString();
      }
    }
    _gidToShapes[gid] = shape;
    return shape;
  }

  return _gidToShapes[gid];
}

Node* PhysicsTiledScene::providePhysicsNode(const int gid)
{
  const auto node = Node::create();
  const auto shape = getShapeFromTileGid(gid);
  if (!shape.empty())
  {
    addBodyToNode(node, shape);
  }
  return node;
}

bool PhysicsTiledScene::addBodyToNode(Node* node, const string& shape) const
{
  auto result = false;

  do
  {
    PhysicsBody* body = nullptr;
    if (!shape.empty())
    {
      body = _physicsShapeCache->createBodyWithName(shape);
    }

    if (body != nullptr)
    {
      node->setPhysicsBody(body);
    }

    result = true;
  } while (false);

  return result;
}

Node* PhysicsTiledScene::createDummyNode(experimental::TMXLayer* const layer, const Vec2& tilePos,
                                         const int gid)
{
  const auto node = providePhysicsNode(gid);
  node->setAnchorPoint(Vec2(0, 0));
  node->setContentSize(_blockSize);
  node->setPosition(layer->getPositionAt(tilePos) + (_blockSize / 2));
  node->setVisible(false);
  layer->addChild(node);

  return node;
}

bool PhysicsTiledScene::addPhysicsToMap()
{
  auto result = false;

  do
  {
    const auto map = getTiledMap();

    const auto shapes = map->getProperty("shapes").asString();

    _physicsShapeCache = new PhysicsShapeCache();
    UTILS_BREAK_IF(_physicsShapeCache == nullptr);

    UTILS_BREAK_IF(!_physicsShapeCache->addShapesWithFile(shapes));

    for (auto& child : map->getChildren())
    {
      const auto layer = dynamic_cast<experimental::TMXLayer*>(child);
      if (layer != nullptr)
      {
        auto physics = layer->getProperty("physics");
        if (physics.asBool())
        {
          for (auto col = 0; col < _blocks.height; col++)
          {
            for (auto row = 0; row < _blocks.width; row++)
            {
              const auto tilePos = Vec2(row, col);
              const auto gid = layer->getTileGIDAt(tilePos);
              if (gid != 0)
              {
                createDummyNode(layer, tilePos, gid);
              }
            }
          }
        }
      }
    }

    result = true;
  } while (false);

  return result;
}
