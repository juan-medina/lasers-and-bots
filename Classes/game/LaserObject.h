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

#ifndef __LASEROBJECT_H__
#define __LASEROBJECT_H__

#include "HarmObject.h"

// forward declarations
class AudioHelper;
class CustomDrawNode;

class LaserObject final : public HarmObject
{
public:
  using BaseClass = HarmObject;

  LaserObject();

  static LaserObject* create(AudioHelper* audioHelper, float initialAngle, float rotationAngle,
                             float speedFactor, int damage);

  bool init(AudioHelper* audioHelper, float initialAngle, float rotationAngle, float speedFactor, int damage);

  void update(float delta) override;

  void pause() override;

  void resume() override;

private:
  void updateSpark(const Vec2& point);

  float _angle;
  float _initialAngle;
  float _finalAngle;
  float _direction;

  // laser draw node
  CustomDrawNode* _draw;

  // the physics world
  PhysicsWorld* _physicsWorld;

  static constexpr float MAX_LASER_LENGTH = 10000.0f;

  ParticleSystemQuad* _spark;

  static int LOOP_SOUND;

  float _speedFactor;

  AudioHelper* _audioHelper;
};

#endif //__LASEROBJECT_H__
