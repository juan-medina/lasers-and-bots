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

#include "physics_shape_cache.h"


physics_shape_cache::physics_shape_cache()
= default;


physics_shape_cache::~physics_shape_cache()
{
  remove_all_shapes();
}

bool physics_shape_cache::add_shapes_with_file(const std::string& plist)
{
  const auto scale_factor = Director::getInstance()->getContentScaleFactor();
  return add_shapes_with_file(plist, scale_factor);
}


bool physics_shape_cache::add_shapes_with_file(const std::string& plist, const float scale_factor)
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

  auto& body_dict = dict.at("bodies").asValueMap();

  std::vector<body_def*> bodies(body_dict.size());
  int num = 0;

  for (auto iterator = body_dict.cbegin(); iterator != body_dict.cend(); ++iterator)
  {
    const auto& body_data = iterator->second.asValueMap();
    auto body_name = iterator->first;
    auto body_def_object = new body_def();
    bodies[num++] = body_def_object;
    body_defs_.insert(std::make_pair(body_name, body_def_object));
    body_def_object->anchor_point = PointFromString(body_data.at("anchorpoint").asString());
    body_def_object->is_dynamic = body_data.at("is_dynamic").asBool();
    body_def_object->affected_by_gravity = body_data.at("affected_by_gravity").asBool();
    body_def_object->allows_rotation = body_data.at("allows_rotation").asBool();
    body_def_object->linear_damping = body_data.at("linear_damping").asFloat();
    body_def_object->angular_damping = body_data.at("angular_damping").asFloat();
    body_def_object->velocity_limit = body_data.at("velocity_limit").asFloat();
    body_def_object->angular_velocity_limit = body_data.at("angular_velocity_limit").asFloat();

    const auto& fixture_list = body_data.at("fixtures").asValueVector();
    for (auto& fixture_item : fixture_list)
    {
      auto fd = new fixture_data();
      body_def_object->fixtures.push_back(fd);
      auto& fixture_data = fixture_item.asValueMap();
      fd->density = fixture_data.at("density").asFloat();
      fd->restitution = fixture_data.at("restitution").asFloat();
      fd->friction = fixture_data.at("friction").asFloat();
      fd->tag = fixture_data.at("tag").asInt();
      fd->group = fixture_data.at("group").asInt();
      fd->category_mask = fixture_data.at("category_mask").asInt();
      fd->collision_mask = fixture_data.at("collision_mask").asInt();
      fd->contact_test_mask = fixture_data.at("contact_test_mask").asInt();

      auto fixture_type = fixture_data.at("fixture_type").asString();
      if (fixture_type == "POLYGON")
      {
        fd->fixture_type = fixture_polygon;
        const auto& polygons_array = fixture_data.at("polygons").asValueVector();
        for (auto& polygon_item : polygons_array)
        {
          auto poly = new polygon();
          fd->polygons.push_back(poly);
          auto& polygon_array = polygon_item.asValueVector();
          poly->num_vertices = static_cast<int>(polygon_array.size());
          auto* vertices = poly->vertices = new cocos2d::Point[poly->num_vertices];
          auto v_index = 0;
          for (auto& point_string : polygon_array)
          {
            const auto offset = PointFromString(point_string.asString());
            vertices[v_index].x = offset.x / scale_factor;
            vertices[v_index].y = offset.y / scale_factor;
            v_index++;
          }
        }
      }
      else if (fixture_type == "CIRCLE")
      {
        fd->fixture_type = fixture_circle;
        const auto& circle_data = fixture_data.at("circle").asValueMap();
        fd->radius = circle_data.at("radius").asFloat() / scale_factor;
        fd->center = PointFromString(circle_data.at("position").asString()) / scale_factor;
      }
      else
      {
        // unknown type
        return false;
      }
    }
  }

  bodies_in_file_[plist] = bodies;

  return true;
}


physics_shape_cache::body_def* physics_shape_cache::get_body_def(const std::string& name)
{
  try
  {
    return body_defs_.at(name);
  }
  catch (std::out_of_range&)
  {
  }

  try
  {
    return body_defs_.at(name.substr(0, name.rfind('.'))); // remove file suffix and try again...
  }
  catch (std::out_of_range&)
  {
  }

  return nullptr;
}


void physics_shape_cache::set_body_properties(PhysicsBody* body, body_def* bd)
{
  body->setGravityEnable(bd->affected_by_gravity);
  body->setDynamic(bd->is_dynamic);
  body->setRotationEnable(bd->allows_rotation);
  body->setLinearDamping(bd->linear_damping);
  body->setAngularDamping(bd->angular_damping);
  body->setVelocityLimit(bd->velocity_limit);
  body->setAngularVelocityLimit(bd->angular_velocity_limit);
}


void physics_shape_cache::set_shape_properties(PhysicsShape* shape, fixture_data* fd)
{
  shape->setGroup(fd->group);
  shape->setCategoryBitmask(fd->category_mask);
  shape->setCollisionBitmask(fd->collision_mask);
  shape->setContactTestBitmask(fd->contact_test_mask);
  shape->setTag(fd->tag);
}


PhysicsBody* physics_shape_cache::create_body_with_name(const std::string& name)
{
  auto bd = get_body_def(name);
  if (!bd)
  {
    CCLOG("WARNING: PhysicsBody with name \"%s\", not found!", name.c_str());
    return nullptr; // body not found
  }
  auto body = PhysicsBody::create();
  set_body_properties(body, bd);

  for (auto fd : bd->fixtures)
  {
    PhysicsMaterial material(fd->density, fd->restitution, fd->friction);
    if (fd->fixture_type == fixture_circle)
    {
      const auto shape = PhysicsShapeCircle::create(fd->radius, material, fd->center);
      set_shape_properties(shape, fd);
      body->addShape(shape);
    }
    else if (fd->fixture_type == fixture_polygon)
    {
      for (auto polygon : fd->polygons)
      {
        const auto shape = PhysicsShapePolygon::create(polygon->vertices, polygon->num_vertices, material, fd->center);
        set_shape_properties(shape, fd);
        body->addShape(shape);
      }
    }
  }
  return body;
}

void physics_shape_cache::remove_shapes_with_file(const std::string& plist)
{
  auto bodies = bodies_in_file_.at(plist);

  for (auto iterator = bodies.begin(); iterator != bodies.end(); ++iterator)
  {
    safe_delete_body_def(*iterator);
  }

  bodies_in_file_.erase(plist);

  return;
}


void physics_shape_cache::remove_all_shapes()
{
  for (auto iterator = body_defs_.cbegin(); iterator != body_defs_.cend(); ++iterator)
  {
    safe_delete_body_def(iterator->second);
  }
  body_defs_.clear();
  bodies_in_file_.clear();
}


void physics_shape_cache::safe_delete_body_def(body_def* body_def)
{
  for (auto fixture_data : body_def->fixtures)
  {
    for (auto polygon : fixture_data->polygons)
    {
      CC_SAFE_DELETE_ARRAY(polygon->vertices);
      CC_SAFE_DELETE(polygon);
    }
    fixture_data->polygons.clear();
    CC_SAFE_DELETE(fixture_data);
  }
  body_def->fixtures.clear();
  CC_SAFE_DELETE(body_def);
}
