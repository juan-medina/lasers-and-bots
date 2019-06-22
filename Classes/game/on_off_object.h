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
#ifndef __ON_OFF_OBJECT_CLASS__
#define __ON_OFF_OBJECT_CLASS__

#include "../utils/physics/physics_game_object.h"

class on_off_object : public physics_game_object
{
public:
  using base_class = physics_game_object;

  on_off_object();

  bool init(PhysicsShapeCache* physics_shape_cache, const std::string& shape,
            const std::string& sprite_frame_name, const std::string& type, const Vec2& spot_pos,
            const string& target = "");

  bool is_on() const { return on_; }

  bool is_off() const { return !is_on(); }

  bool is_activated() const { return activated_; }

  bool is_unactivated() const { return !is_activated(); }

  virtual bool on();
  virtual bool activate();

  string get_target() const { return target_; }

protected:
  void change_spot_color(const Color3B& color) const;

private:
  bool on_;
  bool activated_;
  string target_;
  Sprite* spot_;
};

#endif // __ON_OFF_OBJECT_CLASS__
