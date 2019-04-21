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

    addChild(label, 0);

    // create node grid for effects
    auto node_grid = NodeGrid::create();
    addChild(node_grid, 0);

    // background color layer will cover our logo until its fade out
    const auto color_layer = LayerColor::create(Color4B(0, 0, 0, 255));
    UTILS_BREAK_IF(!color_layer);

    node_grid->addChild(color_layer, 0);

    const auto tiles = FadeOutTRTiles::create(0.2f, Size(20, 20));
    UTILS_BREAK_IF(tiles==nullptr);

    const auto hide = Hide::create();
    UTILS_BREAK_IF(hide == nullptr);

    const auto delay_exit = DelayTime::create(1.0f);
    UTILS_BREAK_IF(delay_exit == nullptr);

    // function call in the event chain to go to the menu
    const auto func = CallFunc::create(CC_CALLBACK_0(loading_scene::go_to_scene, this));
    UTILS_BREAK_IF(func == nullptr);

    // create the sequence of effects and go to the menu
    const auto sequence = Sequence::create(tiles, hide, delay_exit, func, NULL);
    UTILS_BREAK_IF(sequence == nullptr);

    // run effects
    node_grid->runAction(sequence);


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

    const auto fade = TransitionFadeTR::create(1.0f, scene);
    Director::getInstance()->replaceScene(fade);
  }
  while (false);
}
