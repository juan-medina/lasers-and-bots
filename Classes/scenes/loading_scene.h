#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "../utils/base/scene/basic_scene.h"

class loading_scene final : public basic_scene
{
public:  
  // base_class
  using base_class = basic_scene;

  // constructor
  loading_scene();

  // destructor
  ~loading_scene();

  // create a loading screen that go to the game
  static Scene* game();

private:

  enum load_to_type
  {
    to_game
  };

  // for node creation
  bool init(load_to_type type);


  // go to the scene
  void go_to_scene() const;

  load_to_type type_;
};

#endif // __LOADING_SCENE_H__
