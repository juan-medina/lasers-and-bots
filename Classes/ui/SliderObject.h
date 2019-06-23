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
#ifndef __SLIDER_OBJECT_CLASS__
#define __SLIDER_OBJECT_CLASS__

#include "../utils/utils.h"

class SliderObject final : public MenuItemSprite
{
public:
  using BaseClass = MenuItemSprite;
  using SliderChangeCallback = std::function<void(const float)>;

  SliderObject();

  static SliderObject* create(const std::string& background, const std::string& progress,
                              const SliderChangeCallback& callback);

  bool init(const std::string& background, const std::string& progress, const SliderChangeCallback& callback);

  void setPercentage(const float percentage);
  float getPercentage() const;

  bool isEnabled() const override { return _enabled; }

  void enable(const bool enabled);
  void setColor(const Color3B& color) override;

private:
  ProgressTimer* _progress;
  Label* _label;
  bool _enabled;

  bool enableTouch(const bool enabled);

  bool onTouchBegan(Touch* touch, Event* unusedEvent);
  void onTouchMoved(Touch* touch, Event* unusedEvent);
  void onTouchEnded(Touch* touch, Event* unusedEvent);
  void onTouchCancel(Touch* touch, Event* unusedEvent);

  bool isTouchedByLocation(Node* node, const Vec2& location) const;
  void updateLocation(const Vec2& location);

  EventListenerTouchOneByOne* _touchListener;
  Vec2 _touchBeginAt;
  float _percentBeginTouch;
  Sprite* _arrow;
  float _lastPercentage;

  void onPercentageChange(const float percentage) const;
  std::function<void(const float)> _callback;
};

#endif // __SLIDER_OBJECT_CLASS__
