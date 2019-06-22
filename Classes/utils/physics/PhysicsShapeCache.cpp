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

#include "PhysicsShapeCache.h"

PhysicsShapeCache::PhysicsShapeCache() = default;

PhysicsShapeCache::~PhysicsShapeCache()
{
  removeAllShapes();
}

bool PhysicsShapeCache::addShapesWithFile(const std::string& plist)
{
  const auto scaleFactor = Director::getInstance()->getContentScaleFactor();
  return addShapesWithFile(plist, scaleFactor);
}

bool PhysicsShapeCache::addShapesWithFile(const std::string& plist, const float scaleFactor)
{
  CCASSERT(bodies_in_file_.find(plist) == bodies_in_file_.end(), "file already loaded");

  auto dict = FileUtils::getInstance()->getValueMapFromFile(plist);
  if (dict.empty())
  {
    // plist file not found
    return false;
  }

  auto& metadata = dict["metadata"].asValueMap();
  const auto format = metadata["format"].asInt();
  if (format != 1)
  {
    CCASSERT(format == 1, "format not supported!");
    return false;
  }

  auto& bodyDict = dict.at("bodies").asValueMap();

  std::vector<BodyDef*> bodies(bodyDict.size());
  auto num = 0;

  for (auto iterator = bodyDict.cbegin(); iterator != bodyDict.cend(); ++iterator)
  {
    const auto& bodyData = iterator->second.asValueMap();
    auto bodyName = iterator->first;
    auto bodyDefObject = new BodyDef();
    bodies[num++] = bodyDefObject;
    _bodyDefs.insert(std::make_pair(bodyName, bodyDefObject));
    bodyDefObject->_anchorPoint = PointFromString(bodyData.at("anchorpoint").asString());
    bodyDefObject->_isDynamic = bodyData.at("is_dynamic").asBool();
    bodyDefObject->_affectedByGravity = bodyData.at("affected_by_gravity").asBool();
    bodyDefObject->_allowsRotation = bodyData.at("allows_rotation").asBool();
    bodyDefObject->_linearDamping = bodyData.at("linear_damping").asFloat();
    bodyDefObject->_angularDamping = bodyData.at("angular_damping").asFloat();
    bodyDefObject->_velocityLimit = bodyData.at("velocity_limit").asFloat();
    bodyDefObject->_angularVelocityLimit = bodyData.at("angular_velocity_limit").asFloat();

    const auto& fixtureList = bodyData.at("fixtures").asValueVector();
    for (auto& fixtureItem : fixtureList)
    {
      auto fd = new FixtureData();
      bodyDefObject->_fixtures.push_back(fd);
      auto& fixtureData = fixtureItem.asValueMap();
      fd->_density = fixtureData.at("density").asFloat();
      fd->_restitution = fixtureData.at("restitution").asFloat();
      fd->_friction = fixtureData.at("friction").asFloat();
      fd->_tag = fixtureData.at("tag").asInt();
      fd->_group = fixtureData.at("group").asInt();
      fd->_categoryMask = fixtureData.at("category_mask").asInt();
      fd->_collisionMask = fixtureData.at("collision_mask").asInt();
      fd->_contactTestMask = fixtureData.at("contact_test_mask").asInt();

      auto fixtureType = fixtureData.at("fixture_type").asString();
      if (fixtureType == "POLYGON")
      {
        fd->_fixtureType = FixturePolygon;
        const auto& polygonsArray = fixtureData.at("polygons").asValueVector();
        for (auto& polygonItem : polygonsArray)
        {
          auto poly = new Polygon();
          fd->_polygons.push_back(poly);
          auto& polygon_array = polygonItem.asValueVector();
          poly->_numVertex = static_cast<int>(polygon_array.size());
          auto* vertices = poly->_vertex = new cocos2d::Point[poly->_numVertex];
          auto v_index = 0;
          for (auto& point_string : polygon_array)
          {
            const auto offset = PointFromString(point_string.asString());
            vertices[v_index].x = offset.x / scaleFactor;
            vertices[v_index].y = offset.y / scaleFactor;
            v_index++;
          }
        }
      }
      else if (fixtureType == "CIRCLE")
      {
        fd->_fixtureType = FixtureCircle;
        const auto& circleData = fixtureData.at("circle").asValueMap();
        fd->_radius = circleData.at("radius").asFloat() / scaleFactor;
        fd->_center = PointFromString(circleData.at("position").asString()) / scaleFactor;
      }
      else
      {
        // unknown type
        return false;
      }
    }
  }

  _bodiesInFile[plist] = bodies;

  return true;
}

PhysicsShapeCache::BodyDef* PhysicsShapeCache::getBodyDef(const std::string& name)
{
  try
  {
    return _bodyDefs.at(name);
  }
  catch (std::out_of_range&)
  {
  }

  try
  {
    return _bodyDefs.at(name.substr(0, name.rfind('.'))); // remove file suffix and try again...
  }
  catch (std::out_of_range&)
  {
  }

  return nullptr;
}

void PhysicsShapeCache::setBodyProperties(PhysicsBody* body, BodyDef* bd)
{
  body->setGravityEnable(bd->_affectedByGravity);
  body->setDynamic(bd->_isDynamic);
  body->setRotationEnable(bd->_allowsRotation);
  body->setLinearDamping(bd->_linearDamping);
  body->setAngularDamping(bd->_angularDamping);
  body->setVelocityLimit(bd->_velocityLimit);
  body->setAngularVelocityLimit(bd->_angularVelocityLimit);
}

void PhysicsShapeCache::setShapeProperties(PhysicsShape* shape, FixtureData* fd)
{
  shape->setGroup(fd->_group);
  shape->setCategoryBitmask(fd->_categoryMask);
  shape->setCollisionBitmask(fd->_collisionMask);
  shape->setContactTestBitmask(fd->_contactTestMask);
  shape->setTag(fd->_tag);
}

PhysicsBody* PhysicsShapeCache::createBodyWithName(const std::string& name)
{
  auto bd = getBodyDef(name);
  if (!bd)
  {
    CCLOG("WARNING: PhysicsBody with name \"%s\", not found!", name.c_str());
    return nullptr; // body not found
  }
  auto body = PhysicsBody::create();
  setBodyProperties(body, bd);

  for (auto fd : bd->_fixtures)
  {
    PhysicsMaterial material(fd->_density, fd->_restitution, fd->_friction);
    if (fd->_fixtureType == FixtureCircle)
    {
      const auto shape = PhysicsShapeCircle::create(fd->_radius, material, fd->_center);
      setShapeProperties(shape, fd);
      body->addShape(shape);
    }
    else if (fd->_fixtureType == FixturePolygon)
    {
      for (auto polygon : fd->_polygons)
      {
        const auto shape =
          PhysicsShapePolygon::create(polygon->_vertex, polygon->_numVertex, material, fd->_center);
        setShapeProperties(shape, fd);
        body->addShape(shape);
      }
    }
  }
  return body;
}

void PhysicsShapeCache::removeShapesWithFile(const std::string& plist)
{
  auto bodies = _bodiesInFile.at(plist);

  for (auto iterator = bodies.begin(); iterator != bodies.end(); ++iterator)
  {
    safeDeleteBodyDef(*iterator);
  }

  _bodiesInFile.erase(plist);

  return;
}

void PhysicsShapeCache::removeAllShapes()
{
  for (auto iterator = _bodyDefs.cbegin(); iterator != _bodyDefs.cend(); ++iterator)
  {
    safeDeleteBodyDef(iterator->second);
  }
  _bodyDefs.clear();
  _bodiesInFile.clear();
}

void PhysicsShapeCache::safeDeleteBodyDef(BodyDef* bodyDef)
{
  for (auto fixtureData : bodyDef->_fixtures)
  {
    for (auto polygon : fixtureData->_polygons)
    {
      CC_SAFE_DELETE_ARRAY(polygon->_vertex);
      CC_SAFE_DELETE(polygon);
    }
    fixtureData->_polygons.clear();
    CC_SAFE_DELETE(fixtureData);
  }
  bodyDef->_fixtures.clear();
  CC_SAFE_DELETE(bodyDef);
}
