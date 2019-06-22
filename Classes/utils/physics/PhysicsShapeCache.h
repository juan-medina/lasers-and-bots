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

#ifndef __PHYSICS_SHAPE_CACHE_CLASS__
#define __PHYSICS_SHAPE_CACHE_CLASS__

#include "../utils.h"

class PhysicsShapeCache
{
public:
  PhysicsShapeCache();

  ~PhysicsShapeCache();

  bool addShapesWithFile(const std::string& plist);

  bool addShapesWithFile(const std::string& plist, float scaleFactor);

  void removeShapesWithFile(const std::string& plist);

  void removeAllShapes();

  PhysicsBody* createBodyWithName(const std::string& name);

private:
  typedef enum
  {
    FixturePolygon,
    FixtureCircle
  } FixtureType;

  class Polygon
  {
  public:
    Point* _vertex;
    int _numVertex;
  };

  class FixtureData
  {
  public:
    FixtureType _fixtureType;

    float _density;
    float _restitution;
    float _friction;

    int _tag;
    int _group;
    int _categoryMask;
    int _collisionMask;
    int _contactTestMask;

    // for circles
    Point _center;
    float _radius;

    std::vector<Polygon*> _polygons;
  };

  class BodyDef
  {
  public:
    Point _anchorPoint;
    std::vector<FixtureData*> _fixtures;

    bool _isDynamic;
    bool _affectedByGravity;
    bool _allowsRotation;

    float _linearDamping;
    float _angularDamping;
    float _velocityLimit;
    float _angularVelocityLimit;
  };

  static void safeDeleteBodyDef(BodyDef* bodyDef);
  BodyDef* getBodyDef(const std::string& name);
  static void setBodyProperties(PhysicsBody* body, BodyDef* bd);
  static void setShapeProperties(PhysicsShape* shape, FixtureData* fd);

  std::map<std::string, BodyDef*> _bodyDefs;
  std::map<std::string, std::vector<BodyDef*>> _bodiesInFile;
};

#endif // __PHYSICS_SHAPE_CACHE_CLASS__
