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
