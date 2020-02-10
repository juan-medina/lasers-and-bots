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

#include "OnOffObject.h"

OnOffObject::OnOffObject() : _on(false), _activated(false), _spot(nullptr) {}

bool OnOffObject::init(PhysicsShapeCache* physicsShapeCache, const std::string& shape,
                       const std::string& spriteFrameName, const std::string& type, const Vec2& spotPos,
                       const string& target /*= ""*/)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init(physicsShapeCache, shape, spriteFrameName, type));

    _spot = createWithSpriteFrameName("10_spot.png");

    UTILS_BREAK_IF(_spot == nullptr);

    _spot->setPosition(spotPos.x, getContentSize().height - spotPos.y);

    _spot->setVisible(true);
    _spot->setColor(Color3B::RED);
    _spot->setOpacity(255);
    _spot->setBlendFunc(BlendFunc::ADDITIVE);

    const auto fadeDown = FadeTo::create(0.5, 90);
    const auto fadeUp = FadeTo::create(0.5, 255);
    const auto blink = Sequence::create(fadeDown, fadeUp, nullptr);
    const auto repeat = RepeatForever::create(blink);

    _spot->runAction(repeat);

    addChild(_spot);

    _target = target;

    ret = true;
  } while (false);

  return ret;
}

bool OnOffObject::on()
{
  if (isOff())
  {
    _on = true;
    changeSpotColor(Color3B::GREEN);
    return true;
  }

  return false;
}

bool OnOffObject::activate()
{
  if (isUnactivated())
  {
    _activated = true;
    changeSpotColor(Color3B::YELLOW);
    return true;
  }

  return false;
}

void OnOffObject::changeSpotColor(const Color3B& color) const
{
  _spot->setColor(color);
}
