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
#ifndef __LEVEL_COMPLETED_CLASS__
#define __LEVEL_COMPLETED_CLASS__

#include "../menu/BasicMenu.h"
#include "../utils/utils.h"

// forward declarations
class AudioHelper;
class LevelManager;
enum class CompletedResult;
class TextButton;

class LevelCompleted final : public BasicMenu
{
public:
  using BaseClass = BasicMenu;

  LevelCompleted();

  static LevelCompleted* create(AudioHelper* audioHelper, LevelManager* levelManager);

  bool init(AudioHelper* audioHelper, LevelManager* levelManager);

  void display(const unsigned short int level, const float time, const unsigned short int stars,
               const CompletedResult completion, const ccMenuCallback& callback);

  void hide() override;

private:
  Label* addLabels(const std::string& labelText, const std::string& text, const Vec2& pos,
                   const float separation);

  void starSound() const;

  void animateLabel(Label* label) const;

protected:
  bool createMenuItems() override;

private:
  AudioHelper* _audioHelper;
  TextButton* _continueItem;
  Label* _levelNameLabel;

  std::vector<Sprite*> _grayStars;
  std::vector<Sprite*> _goldStars;
  std::vector<Label*> _labelStars;

  LevelManager* _levelManager;
  Label* _levelTotalTimeLabel;
  Label* _levelTimeLimitLabel;
  Label* _levelTimeRecordLabel;
  Label* _level3StarsRecordLabel;
};

#endif // __LEVEL_COMPLETED_CLASS__
