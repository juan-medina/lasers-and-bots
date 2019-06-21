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

#include "door_object.h"
#include "../utils/audio/AudioHelper.h"

door_object::door_object() : audio_helper_(nullptr) {}

door_object* door_object::create(physics_shape_cache* physics_shape_cache, AudioHelper* audio_helper)
{
  door_object* ret = nullptr;

  do
  {
    auto object = new door_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physics_shape_cache, audio_helper))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  } while (false);

  return ret;
}

bool door_object::init(physics_shape_cache* physics_shape_cache, AudioHelper* audio_helper)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(
      !base_class::init(physics_shape_cache, "04_Door", "04_DoorLocked.png", "door", Vec2(140, 14)));

    audio_helper_ = audio_helper;

    audio_helper_->preLoadEffect("sounds/metal_click.mp3");
    audio_helper_->preLoadEffect("sounds/slide.mp3");

    ret = true;
  } while (false);

  return ret;
}

bool door_object::on()
{
  if (base_class::on())
  {
    change_frame("05_DoorOpen.png");
    audio_helper_->playEffect("sounds/slide.mp3");
    return true;
  }

  return false;
}

bool door_object::activate()
{
  if (base_class::activate())
  {
    change_frame("06_DoorUnlocked.png");
    audio_helper_->playEffect("sounds/metal_click.mp3");
    return true;
  }

  return false;
}
