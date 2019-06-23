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

#ifndef __GAME_UI__CLASS__
#define __GAME_UI__CLASS__

#include "../utils/utils.h"

// forward declarations
class MessageWindow;
class LevelCompleted;
class PauseWindow;
class VirtualJoyStick;
class AudioHelper;
class LevelManager;
class InputController;
enum class CompletedResult;

class GameUI final : public Node
{
public:
  using BaseClass = Node;

  GameUI();

  static GameUI* create(AudioHelper* audioHelper, InputController* inputController,
                        LevelManager* levelManager, const unsigned short int level);

  bool init(AudioHelper* audioHelper, InputController* inputController, LevelManager* levelManager,
            const unsigned short int level);

  void onPause(Ref* sender) const;
  void onClose(Ref* sender) const;
  void onReload(Ref* sender) const;

  inline VirtualJoyStick* getVirtualJoyStick() const { return _virtualJoyStick; }

  inline void setShieldPercentage(const float amount) const
  {
    _shieldBar->setPercentage(amount);
    const auto intValue = static_cast<int>(std::round(amount));
    _shieldLabel->setString(StringFormat("%3d %%", intValue));
  }

  void changePauseButton() const;
  void disableButtons(const bool disabled) const;

  void updateTime(const float time, const unsigned int timeLimit);

  static string timeMessage(const float time);

  void displayMessage(const std::string& message, const std::string& subMessage,
                      const ccMenuCallback& callback);

  void displayLevelCompleted(const unsigned short int level, const float time, const unsigned short int stars,
                             const CompletedResult completion, const ccMenuCallback& callback);

  void updateCountdown(const int value) const;

  void displayPauseWindow() const;

  void update(float delta) override;

private:
  void onContinue();

  VirtualJoyStick* _virtualJoyStick;
  ProgressTimer* _shieldBar;
  Label* _shieldLabel;
  MenuItemToggle* _pauseItem;
  Label* _timeLabel;
  Label* _subTimeLabel;
  Label* _countdownLabel;
  Label* _levelNameLabel;
  unsigned int _timeLimit;
  ccMenuCallback _continueCallback;
  AudioHelper* _audioHelper;
  MessageWindow* _messageWindow;
  LevelCompleted* _levelCompleted;
  PauseWindow* _pauseWindow;
  LevelManager* _levelManager;
  unsigned short int _level;
  InputController* _inputController;
};

#endif // __GAME_UI__CLASS__
