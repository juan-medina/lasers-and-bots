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

#include "loading_scene.h"
#include "game_scene.h"

Scene* loading_scene::game()
{
  loading_scene* ret = nullptr;

  do
  {
    auto object = new loading_scene();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(to_game))
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

  // return the object
  return ret;
}

loading_scene::loading_scene():
  type_(to_game)
{
}

loading_scene::~loading_scene()
{
  base_class::removeAllChildrenWithCleanup(true);
}

bool loading_scene::init(const load_to_type type)
{
  auto ret = false;

  do
  {
    type_ = type;

    //////////////////////////////////////////////////////////////////////////
    // Call parent
    //////////////////////////////////////////////////////////////////////////

    UTILS_BREAK_IF(!base_class::init());

    //////////////////////////////////////////////////////////////////////////
    // Background & Title
    //////////////////////////////////////////////////////////////////////////

    // Get window size and place the label upper.
    const auto size = Director::getInstance()->getWinSize();


    // add the background
    const auto background = LayerGradient::create(Color4B(0, 0, 0, 255), Color4B(0, 0, 127, 255));
    UTILS_BREAK_IF(background==nullptr);

    // Add the sprite to Menu layer as a child layer.
    addChild(background, 0);

    // create the text for the label
    auto label = Label::createWithTTF("Loading...", "fonts/tahoma.ttf", 500);
    UTILS_BREAK_IF(label == nullptr);

    // position menu item
    label->setPosition(Vec2(size.width / 2, size.height / 2));
    label->setTextColor(Color4B(0, 255, 255, 255));

    label->enableGlow(Color4B(0, 127, 127, 127));
    label->enableShadow(Color4B(255, 255, 255, 127), Size(10, -10));
    label->enableOutline(Color4B(255, 255, 255, 255), 10);

    addChild(label, 0);

    const auto delay_exit = DelayTime::create(1.f);
    UTILS_BREAK_IF(delay_exit == nullptr);

    // function call in the event chain to go to the menu
    const auto func = CallFunc::create(CC_CALLBACK_0(loading_scene::go_to_scene, this));
    UTILS_BREAK_IF(func == nullptr);

    // create the sequence of effects and go to the menu
    const auto sequence = Sequence::create(delay_exit, func, NULL);
    UTILS_BREAK_IF(sequence == nullptr);

    // run effects
    runAction(sequence);


    ret = true;
  }
  while (false);

  return ret;
}


void loading_scene::go_to_scene() const
{
  do
  {
    Scene* scene = nullptr;

    switch (type_)
    {
    case to_game:
      scene = game_scene::scene();
      break;

    default:
      break;
    }

    UTILS_BREAK_IF(scene==nullptr);

    Director::getInstance()->replaceScene(scene);
  }
  while (false);
}
