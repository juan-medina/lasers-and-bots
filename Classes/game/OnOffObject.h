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
#ifndef __ONOFFOBJECT_H__
#define __ONOFFOBJECT_H__

#include "../utils/physics/PhysicsGameObject.h"

class OnOffObject : public PhysicsGameObject
{
public:
  using BaseClass = PhysicsGameObject;

  OnOffObject();

  bool init(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
            const std::string& spriteFrameName, const std::string& type, const Vec2& spotPos,
            const string& target = "");

  bool isOn() const { return _on; }

  bool isOff() const { return !isOn(); }

  bool isActivated() const { return _activated; }

  bool isUnactivated() const { return !isActivated(); }

  virtual bool on();
  virtual bool activate();

  string getTarget() const { return _target; }

protected:
  void changeSpotColor(const Color3B& color) const;

private:
  bool _on;
  bool _activated;
  string _target;
  Sprite* _spot;
};

#endif //__ONOFFOBJECT_H__
