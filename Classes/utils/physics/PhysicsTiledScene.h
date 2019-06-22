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

#ifndef __PHYSICS_TILED_SCENE_H__
#define __PHYSICS_TILED_SCENE_H__

#include "../base/scene/TiledScene.h"

// forward declarations
class PhysicsShapeCache;

class PhysicsTiledScene : public TiledScene
{
public:
  using BaseClass = TiledScene;

  PhysicsTiledScene();
  ~PhysicsTiledScene();

  static PhysicsTiledScene* create(BasicApp* application, const std::string& tmxFile, const float gravity,
                                   const bool debugPhysics);

  static Scene* scene(BasicApp* application, const std::string& tmxFile, const float gravity,
                      const bool debugPhysics);

  bool init(BasicApp* application, const std::string& tmxFile, const float gravity, const bool debugPhysics);

  PhysicsShapeCache* get_physics_shape_cache() const { return _physicsShapeCache; }

protected:
  bool addBodyToNode(Node* node, const string& shape) const;
  virtual Node* providePhysicsNode(const int gid);
  string getShapeFromTileGid(const int gid);

private:
  virtual void initPhysics(const bool debugPhysics) const;
  bool addPhysicsToMap();

  // create a dummy node to hold physic body
  Node* createDummyNode(experimental::TMXLayer* layer, const Vec2& tilePos, const int gid);

  float _gravity = 0.0f;
  std::map<int, string> _gidToShapes;

  PhysicsShapeCache* _physicsShapeCache;
};

#endif // __PHYSICS_TILED_SCENE_H__
