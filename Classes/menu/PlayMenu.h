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

#ifndef __PLAY_MENU_CLASS__
#define __PLAY_MENU_CLASS__

#include "../utils/utils.h"
#include "BasicMenu.h"

// forward declarations
class AudioHelper;
class LevelManager;
class TextButton;
class TextToggle;

class PlayMenu final : public BasicMenu
{
public:
  PlayMenu();

  using BaseClass = BasicMenu;

  static PlayMenu* create(AudioHelper* audioHelper, const unsigned short int selectedLevel);

  bool init(AudioHelper* audioHelper, const unsigned short int selectedLevel);

  void display() override;

protected:
  bool createMenuItems() override;

private:
  Label* addLabels(const std::string& labelText, const std::string& text, const Vec2& pos,
                   const float separation);
  void onBack();
  void onLevelSelect(Ref* sender, const unsigned short int level);
  void onPlay();
  void selectLevel(const unsigned short int level);
  LevelManager* getLevelManager();

  std::map<unsigned short int, TextToggle*> _levelButtons;
  static constexpr auto STAR_TAG = 0xFF0F;

  TextButton* _backItem;
  TextButton* _playItem;
  Label* _levelNameLabel;
  Label* _levelTimeLimitLabel;
  Label* _levelTimeRecordLabel;
  Label* _level3StarsTimeRecordLabel;
  Label* _levelStarsLabel;
  unsigned short int _selectedLevel;
};

#endif // __PLAY_MENU_CLASS__
