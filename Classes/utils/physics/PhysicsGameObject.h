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
#ifndef __PHYSICSGAMEOBJECT_H__
#define __PHYSICSGAMEOBJECT_H__

#include "../base/sprite/GameObject.h"

// forward declarations
class PhysicsShapeCache;

class PhysicsGameObject : public GameObject
{
public:
  using BaseClass = GameObject;

  static PhysicsGameObject* create(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
                                   const std::string& spriteFrameName, const std::string& type);
  static PhysicsGameObject* create(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
                                   const std::string& type);
  static PhysicsGameObject* create(const std::string& type);

  virtual bool init(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
                    const std::string& spriteFrameName, const std::string& type);
  virtual bool init(PhysicsShapeCache* physicsShapeCache, const std::string& shape, const std::string& type);
  bool init(const std::string& type) override;

private:
  bool setShape(PhysicsShapeCache* physicsShapeCache, const std::string& shapeName);
};

#endif //__PHYSICSGAMEOBJECT_H__
