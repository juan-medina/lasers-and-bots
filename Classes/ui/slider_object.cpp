/****************************************************************************
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

#include "slider_object.h"

slider_object::slider_object():
  progress_(nullptr),
  enabled_(true)
{
}

slider_object* slider_object::create(const std::string& background, const std::string& progress)
{
  slider_object* ret = nullptr;

  do
  {
    auto object = new slider_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(background, progress))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  }
  while (false);

  return ret;
}

bool slider_object::init(const std::string& background, const std::string& progress)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::initWithSpriteFrameName(background));

    const auto progress_sprite = Sprite::createWithSpriteFrameName(progress);
    UTILS_BREAK_IF(progress_sprite == nullptr);

    progress_ = ProgressTimer::create(progress_sprite);
    UTILS_BREAK_IF(progress_ == nullptr);

    progress_->setType(ProgressTimer::Type::BAR);
    progress_->setMidpoint(Vec2(0, 0));
    progress_->setBarChangeRate(Vec2(1, 0));

    progress_->setPosition(getContentSize().width / 2, getContentSize().height / 2);

    addChild(progress_);

    setCascadeOpacityEnabled(true);
    setCascadeColorEnabled(true);

    ret = true;
  }
  while (false);

  return ret;
}

void slider_object::set_percentage(const float percentage) const
{
  progress_->setPercentage(percentage);
}

float slider_object::get_percentage() const
{
  return progress_->getPercentage();
}

void slider_object::enable(const bool enabled)
{
  enabled_ = enabled;
  const auto opacity = static_cast<GLubyte>(enabled ? 255 : 127);
  setOpacity(opacity);
  progress_->setOpacity(opacity);
}
