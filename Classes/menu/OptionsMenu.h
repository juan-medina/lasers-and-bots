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

#ifndef __OPTIONSMENU_H__
#define __OPTIONSMENU_H__

#include "../utils/utils.h"
#include "BasicMenu.h"

// forward declarations
class AudioHelper;
class SliderObject;
class TextToggle;
class TextButton;

class OptionsMenu final : public BasicMenu
{
public:
  using BaseClass = BasicMenu;

  OptionsMenu();

  static OptionsMenu* create(AudioHelper* audioHelper, bool isDesktopApplication);

  bool init(AudioHelper* audioHelper, bool isDesktopApplication);

  void display() override;

protected:
  bool createMenuItems() override;

private:
  bool _desktopApplication;

  TextButton* _backItem;
  TextToggle* _soundToggle;
  TextToggle* _musicToggle;
  SliderObject* _soundSlider;
  SliderObject* _musicSlider;
  TextToggle* _fullScreenToggle;
  TextToggle* _windowedToggle;
  TextToggle* _debugGridToggle;
  TextToggle* _debugPhysicsToggle;

  void onBack();
  void onMusic();
  void onSound();
  void onMusicSliderChange(float percentage);
  void onSoundSliderChange(float percentage);
  void onFullScreen();
  void onWindowed();
  void onDebugGrid();
  void onDebugPhysics();
  void updateLabels();
};

#endif //__OPTIONSMENU_H__