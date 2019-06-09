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

#include "background_layer.h"
background_layer::background_layer() : bottom_layer_(nullptr), screen_size_(cocos2d::Size::ZERO) {}

background_layer *background_layer::create()
{
  background_layer *ret = nullptr;

  do
  {
    auto object = new background_layer();
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

bool background_layer::init()
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init());
    bottom_layer_ = LayerGradient::create(Color4B(0, 255, 255, 255), Color4B(0, 127, 127, 255));
    UTILS_BREAK_IF(bottom_layer_ == nullptr);

    addChild(bottom_layer_);

    screen_size_ = Director::getInstance()->getWinSize();

    bottom_layer_->setPosition(-screen_size_.width / 2, 0.f);

    auto sprite_frame_cache = SpriteFrameCache::getInstance();

    sprite_frame_cache->addSpriteFramesWithFile("backgrounds/backgrounds-1.plist");
    sprite_frame_cache->addSpriteFramesWithFile("backgrounds/backgrounds-2.plist");
    sprite_frame_cache->addSpriteFramesWithFile("backgrounds/backgrounds-3.plist");

    for (int layer = 4; layer >= 0; --layer)
    {
      const auto file_name = string_format("layer_%d.png", layer);
      auto sprite = Sprite::createWithSpriteFrameName(file_name);
      UTILS_BREAK_IF(sprite == nullptr);

      sprite->setScale(layer_scale);

      addChild(sprite);

      const auto sprite_mirror = Sprite::createWithSpriteFrameName(file_name);
      UTILS_BREAK_IF(sprite_mirror == nullptr);
      sprite->addChild(sprite_mirror);
      sprite_mirror->setPosition(sprite->getContentSize().width + (sprite_mirror->getContentSize().width/2),
                                 sprite_mirror->getContentSize().height / 2);

      layers_.push_back(sprite);
    }

    UTILS_BREAK_IF(layers_.size() != 5);

    ret = true;
  } while (false);

  return ret;
}

void background_layer::update_scroll(const Vec2 &position)
{
  const auto new_pos = Vec2(position.x, position.y - (screen_size_.height / 2));

  setPosition(new_pos);

  for (unsigned long layer = 0; layer < layers_.size(); ++layer)
  {
    const auto sprite = layers_.at(layer);
    const auto layer_relative = layers_.size() - layer;
    const auto layer_pos = (Vec2(0, sprite->getContentSize().height / 3.5f) * layer_scale) -
                           ((position / layer_scale) / (layer_relative));
    sprite->setPosition(layer_pos);
  }
}
