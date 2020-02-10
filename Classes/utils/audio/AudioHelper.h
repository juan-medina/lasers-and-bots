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

#ifndef __AUDIOHELPER_H__
#define __AUDIOHELPER_H__

#include "../utils.h"

// class that represent a audio engine
class AudioHelper final : public Ref
{
public:
  using BaseClass = Ref;

  AudioHelper();
  ~AudioHelper() override;

  int playEffect(const std::string& fileName, bool loop = false, float volume = 1.0f) const;
  void playMusic(const std::string& loopFileName, float volume = 1.0f);
  void playMusic(const std::string& introFileName, const std::string& loopFileName, float volume = 1.0f);

  void preLoadEffect(const std::string& fileName) const;
  void preLoadMusic(const std::string& loopFileName) const;
  void preLoadMusic(const std::string& introFileName, const std::string& loopFileName) const;
  void unloadEffect(const std::string& fileName) const;
  void stopAllSounds();
  void unloadAllSounds();

  void pauseMusic() const;
  void resumeMusic();

  void appExit();

  void stopSound(int sound);
  void pauseSound(int sound);
  void resumeSound(int sound);

  bool getMusicMuted() const noexcept { return _musicMuted; }

  void setMusicMuted(const bool musicMuted) noexcept { _musicMuted = musicMuted; };

  bool getEffectsMuted() const noexcept { return _effectsMuted; }

  void setEffectsMuted(const bool effectsMuted) noexcept { _effectsMuted = effectsMuted; };

  void end();

  float getMusicVolume() const noexcept { return _musicVolume; }

  void setMusicVolume(float musicVolume) noexcept;

  float getSoundVolume() const noexcept { return _soundVolume; }

  void setSoundVolume(const float soundVolume) noexcept { _soundVolume = soundVolume; }

private:
  void onMusicIntroEnds(int id, const std::string& filePath);
  bool init();

  bool _initiated;
  bool _musicMuted;
  bool _effectsMuted;

  struct LastMusic
  {
    int _loopId;
    int _introId;
    std::string _introFileName;
    std::string _loopFileName;
    float _volume;
  };

  LastMusic _lastMusic;

  float _musicVolume;
  float _soundVolume;
};

#endif //__AUDIOHELPER_H__
