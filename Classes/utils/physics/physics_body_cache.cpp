/****************************************************************************
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

#include "physics_body_cache.h"
#include "rapidjson/document.h"

physics_body_cache::physics_body_cache() :
  initiated_(false)
{
  // init object
  init();
}

physics_body_cache::~physics_body_cache()
{
  // if we are init we need to end
  if (initiated_)
  {
    end();
  }
}

bool physics_body_cache::init()
{
  // object is init
  initiated_ = true;

  return true;
}

void physics_body_cache::end()
{
  // we are not init
  initiated_ = false;

  clear();
}

void physics_body_cache::clear()
{
  for (auto it = bodies_.begin(); it != bodies_.end(); ++it)
  {
    delete it->second;
  }
  bodies_.clear();
}

bool physics_body_cache::load(const string& physics_editor_json_file)
{
  auto ret = false;

  do
  {
    auto document = rapidjson::Document();

    const auto full_path = FileUtils::getInstance()->fullPathForFilename(physics_editor_json_file);
    const auto json = FileUtils::getInstance()->getStringFromFile(full_path);

    document.Parse(json.c_str());
    if (!document.HasParseError())
    {
      if (document.HasMember("rigidBodies"))
      {
        if (document["rigidBodies"].IsArray())
        {
          const auto bodies = document["rigidBodies"].GetArray();
          for (rapidjson::Value::ConstValueIterator bodies_iterator = bodies.Begin(); bodies_iterator != bodies.End();
               ++bodies_iterator)
          {
            if (bodies_iterator->HasMember("name"))
            {
              const auto name = (*bodies_iterator)["name"].GetString();
              const auto width = (*bodies_iterator)["width"].GetInt();
              const auto height = (*bodies_iterator)["height"].GetInt();
              const auto scale_x = width - 1;
              const auto scale_y = height - 1;

              auto saved_body = new physics_body_cache::saved_body(width, height);

              if (bodies_iterator->HasMember("polygons"))
              {
                if ((*bodies_iterator)["polygons"].IsArray())
                {
                  const auto polygons = (*bodies_iterator)["polygons"].GetArray();
                  for (rapidjson::Value::ConstValueIterator polygon_iterator = polygons.Begin(); polygon_iterator !=
                       polygons.End(); ++polygon_iterator)
                  {
                    if ((*polygon_iterator).IsArray())
                    {
                      const auto vertex_def = (*polygon_iterator).GetArray();
                      const auto size = vertex_def.Size();
                      const auto saved_shape = new physics_body_cache::saved_shape(size);
                      const auto vertex = saved_shape->vertex_;
                      for (rapidjson::SizeType i = 0; i < size; i++)
                      {
                        vertex[i].x = 0.f;
                        vertex[i].y = 0.f;
                        if (vertex_def[i].HasMember("x"))
                        {
                          if (vertex_def[i]["x"].IsFloat())
                          {
                            vertex[i].x = vertex_def[i]["x"].GetFloat() * scale_x;
                          }
                        }
                        if (vertex_def[i].HasMember("y"))
                        {
                          if (vertex_def[i]["y"].IsFloat())
                          {
                            vertex[i].y = vertex_def[i]["y"].GetFloat() * scale_y;
                          }
                        }
                      }
                      saved_body->shapes_.push_back(saved_shape);
                    }
                  }
                }
              }
              if (!saved_body->shapes_.empty())
              {
                bodies_[name] = saved_body;
              }
              else
              {
                delete saved_body;
              }
            }
          }
        }
      }
    }

    ret = true;
  }
  while (false);

  return ret;
}

PhysicsBody* physics_body_cache::get_body(const string& name, const PhysicsMaterial& mat)
{
  PhysicsBody* body = nullptr;

  if (bodies_.count(name) != 0)
  {
    auto saved_body = bodies_[name];
    const auto offset = Vec2(-saved_body->body_width_ / 2, -saved_body->body_height_ / 2);

    body = PhysicsBody::create();

    for (auto shape : saved_body->shapes_)
    {
      const auto polygon_shape = PhysicsShapePolygon::create(shape->vertex_, shape->num_vertex_, mat, offset);
      body->addShape(polygon_shape);
    }
  }
  return body;
}

physics_body_cache::saved_shape::saved_shape(const unsigned short int num_vertex)
{
  num_vertex_ = num_vertex;
  vertex_ = new Vec2[num_vertex_];
}

physics_body_cache::saved_shape::~saved_shape()
{
  delete[] vertex_;
}

physics_body_cache::saved_body::saved_body(float _body_with, float _body_height)
{
  body_width_ = _body_with;
  body_height_ = _body_height;
}

physics_body_cache::saved_body::~saved_body()
{
  for (auto shape : shapes_)
  {
    delete shape;
  }
  shapes_.clear();
}
