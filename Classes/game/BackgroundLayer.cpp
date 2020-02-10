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

#include "BackgroundLayer.h"
BackgroundLayer::BackgroundLayer() : _bottomLayer(nullptr), _screenSize(cocos2d::Size::ZERO) {}

BackgroundLayer *BackgroundLayer::create()
{
  BackgroundLayer *ret = nullptr;

  do
  {
    auto object = new BackgroundLayer();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init())
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

bool BackgroundLayer::init()
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!BaseClass::init());
    _bottomLayer = LayerGradient::create(Color4B(0, 255, 255, 255), Color4B(0, 127, 127, 255));
    UTILS_BREAK_IF(_bottomLayer == nullptr);

    addChild(_bottomLayer);

    _screenSize = Director::getInstance()->getWinSize();

    _bottomLayer->setPosition(-_screenSize.width / 2, 0.f);

    auto spriteFrameCache = SpriteFrameCache::getInstance();

    spriteFrameCache->addSpriteFramesWithFile("backgrounds/backgrounds-1.plist");
    spriteFrameCache->addSpriteFramesWithFile("backgrounds/backgrounds-2.plist");
    spriteFrameCache->addSpriteFramesWithFile("backgrounds/backgrounds-3.plist");

    for (int layer = 4; layer >= 0; --layer)
    {
      const auto fileName = StringFormat("layer_%d.png", layer);
      auto sprite = Sprite::createWithSpriteFrameName(fileName);
      UTILS_BREAK_IF(sprite == nullptr);

      sprite->setScale(LAYER_SCALE);

      addChild(sprite);

      const auto spriteMirror = Sprite::createWithSpriteFrameName(fileName);
      UTILS_BREAK_IF(spriteMirror == nullptr);
      sprite->addChild(spriteMirror);
      spriteMirror->setPosition(sprite->getContentSize().width + (spriteMirror->getContentSize().width / 2),
                                spriteMirror->getContentSize().height / 2);

      _layers.push_back(sprite);
    }

    UTILS_BREAK_IF(_layers.size() != 5);

    ret = true;
  } while (false);

  return ret;
}

void BackgroundLayer::updateScroll(const Vec2 &position)
{
  const auto newPos = Vec2(position.x, position.y - (_screenSize.height / 2));

  setPosition(newPos);

  for (unsigned long layer = 0; layer < _layers.size(); ++layer)
  {
    const auto sprite = _layers.at(layer);
    const auto layerRelative = _layers.size() - layer;
    const auto layerPos = (Vec2(0, sprite->getContentSize().height / 3.5f) * LAYER_SCALE) -
                          ((position / LAYER_SCALE) / (layerRelative));
    sprite->setPosition(layerPos);
  }
}
