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

#ifndef __LOADINGSCENE_H__
#define __LOADINGSCENE_H__

#include "../utils/base/scene/BasicScene.h"

// forward declarations
enum class MenuToDisplay;

class LoadingScene final : public BasicScene
{
public:
  using BaseClass = BasicScene;

  LoadingScene();

  ~LoadingScene() override;

  static Scene *game(BasicApp *application, bool debugGrid, bool debugPhysics, unsigned short int level);

  static Scene *menu(BasicApp *application, MenuToDisplay menu, unsigned short int selectedLevel);

private:
  enum class LoadTo
  {
    ToGame,
    ToMenu
  };

  bool init(BasicApp *application, const LoadTo &type, bool debugGrid, bool debugPhysics, MenuToDisplay menu,
            unsigned short int level);

  void goToScene() const;

  LoadTo _type;
  bool _debugGrid;
  bool _debugPhysics;
  MenuToDisplay _menu;
  unsigned short int _level;
};

#endif //__LOADINGSCENE_H__
