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

#include "DoorObject.h"

#include "../utils/audio/AudioHelper.h"

DoorObject::DoorObject() : _audioHelper(nullptr) {}

DoorObject* DoorObject::create(PhysicsShapeCache* physicsShapeCache, AudioHelper* audioHelper)
{
  DoorObject* ret = nullptr;

  do
  {
    auto object = new DoorObject();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(physicsShapeCache, audioHelper))
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

bool DoorObject::init(PhysicsShapeCache* physicsShapeCache, AudioHelper* audioHelper)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(
      !BaseClass::init(physicsShapeCache, "04_Door", "04_DoorLocked.png", "door", Vec2(140, 14)));

    _audioHelper = audioHelper;

    _audioHelper->preLoadEffect("sounds/metal_click.mp3");
    _audioHelper->preLoadEffect("sounds/slide.mp3");

    ret = true;
  } while (false);

  return ret;
}

bool DoorObject::on()
{
  if (BaseClass::on())
  {
    changeFrame("05_DoorOpen.png");
    _audioHelper->playEffect("sounds/slide.mp3");
    return true;
  }

  return false;
}

bool DoorObject::activate()
{
  if (BaseClass::activate())
  {
    changeFrame("06_DoorUnlocked.png");
    _audioHelper->playEffect("sounds/metal_click.mp3");
    return true;
  }

  return false;
}
