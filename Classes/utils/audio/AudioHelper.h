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

#ifndef __AUDIO_HELPER_H__
#define __AUDIO_HELPER_H__

#include "../utils.h"

// class that represent a audio engine
class AudioHelper : public Ref
{
public:
  // parent
  typedef Ref parent;

  // constructor & destructor
  AudioHelper();
  ~AudioHelper();

  // play an effect
  unsigned int playEffect(const char* fileName);

  // play music
  void playMusic(const char* fileName);

  // preload an effect
  void preLoadEffect(const char* fileName);

  // preload music
  void preLoadMusic(const char* fileName);

  // toggle sound
  void toggleSound();

  // toggle music
  void toggleMusic();

  // app is going to background
  void appToBG();

  // app is going to foreground
  void appToFG();

  // the application is going to exit
  void appExit();

  // get default audioo helper
  static AudioHelper* getInstance();

  // stops all sounds
  void stopAllSounds();

  // is music muted
  CC_SYNTHESIZE(bool, _musicMuted, MusicMuted);

  // is effects muted
  CC_SYNTHESIZE(bool, _effectsMuted, EffectsMuted);

  // last music requested to play
  CC_SYNTHESIZE_READONLY(string, _lastMusic, LastMusic);

  // unload and effect
  void unloadEffect(const char* file);

protected:
  // save values in userconfig
  void saveValues();

  // is the object initialized ?
  bool _inited;

  // init helper
  bool init();

  // end helper
  void end();
};

#endif // __BUTTON_H__
