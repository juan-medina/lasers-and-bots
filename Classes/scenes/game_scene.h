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
#ifndef __MAIN_SCENE__
#define __MAIN_SCENE__

#include "../utils/utils.h"
#include "../utils/base/scene/physics_tiled_scene.h"

//foward declarations
class robot_object;
class game_ui;

class game_scene final : public physics_tiled_scene
{
public:
  // base_class
  using base_class = physics_tiled_scene;

  // constructor
  game_scene();

  // create the object
  static game_scene* create();

  // create the scene
  static Scene* scene();

  // init this object
  bool init() override;

private:

  // update our game
  void update(float delta) override;

  // move the camera following the robot clamping on the map
  void update_camera() const;

  //our robot
  robot_object* robot_;

  // get the object center position
  static Vec2 get_object_center_position(const ValueMap& values);

  // get the object position
  static Vec2 get_object_position(const ValueMap& values);


  // add lasers to the game
  bool add_objects_to_game();

  // our game gravity
  static constexpr float gravity = -1000.0f;

  game_ui* game_ui_;
};

#endif // __MAIN_SCENE__
