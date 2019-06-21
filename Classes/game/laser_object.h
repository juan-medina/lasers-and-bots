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

#ifndef __LASER_CLASS__
#define __LASER_CLASS__

#include "harm_object.h"

// forward declarations
class AudioHelper;
class custom_draw_node;

class laser_object final : public harm_object
{
public:
  using base_class = harm_object;

  laser_object();

  static laser_object* create(AudioHelper* audio_helper, const float initial_angle,
                              const float rotation_angle, const float speed_factor, const int damage);

  bool init(AudioHelper* audio_helper, const float initial_angle, const float rotation_angle,
            const float speed_factor, const int damage);

  void update(float delta) override;

  void pause() override;

  void resume() override;

private:
  void update_spark(const Vec2& point);

  float angle_;
  float initial_angle_;
  float final_angle_;
  float direction_;

  // laser draw node
  custom_draw_node* draw_;

  // the physics world
  PhysicsWorld* physics_world_;

  static constexpr float max_laser_length = 10000.0f;

  ParticleSystemQuad* spark_;

  static int loop_sound_;

  float speed_factor_;

  AudioHelper* audio_helper_;
};

#endif // __LASER_CLASS__
