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

#include "AudioHelper.h"
#include "audio/include/AudioEngine.h"

using namespace experimental;

AudioHelper::AudioHelper()
  : _initiated(false)
  , _musicMuted(false)
  , _effectsMuted(false)
  , _lastMusic{AudioEngine::INVALID_AUDIO_ID, AudioEngine::INVALID_AUDIO_ID, "", "", 1.f}
  , _musicVolume(1.f)
  , _soundVolume(1.f)
{
  init();
}

AudioHelper::~AudioHelper()
{
  if (_initiated)
  {
    end();
  }
}

bool AudioHelper::init()
{
  if (!_initiated)
  {
    AudioEngine::lazyInit();
  }

  _initiated = true;

  return true;
}

void AudioHelper::end()
{
  if (_initiated)
  {
    AudioEngine::stopAll();
    AudioEngine::uncacheAll();
    AudioEngine::end();
  }

  _initiated = false;
}

void AudioHelper::setMusicVolume(const float musicVolume) noexcept
{
  _musicVolume = musicVolume;

  if (!getMusicMuted())
  {
    if (_lastMusic.introId != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::setVolume(_lastMusic.introId, _lastMusic.volume * musicVolume);
    }
    else if (_lastMusic.loopId != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::setVolume(_lastMusic.loopId, _lastMusic.volume * musicVolume);
    }
  }
}

int AudioHelper::playEffect(const std::string& fileName, const bool loop /*= false*/,
                            const float volume /*= 1.0f*/) const
{
  if (getEffectsMuted())
  {
    return AudioEngine::INVALID_AUDIO_ID;
  }
  return AudioEngine::play2d(fileName, loop, volume * _soundVolume);
}

void AudioHelper::playMusic(const std::string& loopFileName, const float volume /*= 1.0f*/)
{
  _lastMusic.introId = AudioEngine::INVALID_AUDIO_ID;
  _lastMusic.introFileName = "";
  _lastMusic.loopFileName = std::string(loopFileName);
  _lastMusic.volume = volume;
  if (!getMusicMuted())
  {
    _lastMusic.loopId = AudioEngine::play2d(loopFileName, true, volume * _musicVolume);
  }
}

void AudioHelper::onMusicIntroEnds(const int id, const std::string& filePath)
{
  if ((id == _lastMusic.introId) && (filePath == _lastMusic.introFileName))
  {
    playMusic(_lastMusic.loopFileName, _lastMusic.volume);
  }
}

void AudioHelper::playMusic(const std::string& introFileName, const std::string& loopFileName,
                            const float volume /*= 1.0f*/)
{
  _lastMusic.loopId = AudioEngine::INVALID_AUDIO_ID;
  _lastMusic.introFileName = std::string(introFileName);
  _lastMusic.loopFileName = std::string(loopFileName);
  _lastMusic.volume = volume;
  if (!getMusicMuted())
  {
    _lastMusic.introId = AudioEngine::play2d(introFileName, false, volume * _musicVolume);
    AudioEngine::setFinishCallback(_lastMusic.introId, CC_CALLBACK_2(AudioHelper::onMusicIntroEnds, this));
  }
}

void AudioHelper::preLoadEffect(const std::string& fileName) const
{
  AudioEngine::preload(fileName);
}

void AudioHelper::preLoadMusic(const std::string& loopFileName) const
{
  AudioEngine::preload(loopFileName);
}

void AudioHelper::preLoadMusic(const std::string& introFileName, const std::string& loopFileName) const
{
  AudioEngine::preload(introFileName);
  AudioEngine::preload(loopFileName);
}

void AudioHelper::unloadEffect(const std::string& fileName) const
{
  AudioEngine::uncache(fileName);
}

void AudioHelper::pauseMusic() const
{
  if (!getMusicMuted())
  {
    if (_lastMusic.introId != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::pause(_lastMusic.introId);
    }
    else if (_lastMusic.loopId != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::pause(_lastMusic.loopId);
    }
  }
}

void AudioHelper::resumeMusic()
{
  if (!getMusicMuted())
  {
    if (_lastMusic.introId != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::resume(_lastMusic.introId);
      AudioEngine::setVolume(_lastMusic.introId, _lastMusic.volume * _musicVolume);
    }
    else if (_lastMusic.loopId != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::resume(_lastMusic.loopId);
      AudioEngine::setVolume(_lastMusic.loopId, _lastMusic.volume * _musicVolume);
    }
    else
    {
      if (!_lastMusic.introFileName.empty())
      {
        playMusic(_lastMusic.introFileName, _lastMusic.loopFileName, _lastMusic.volume);
      }
      else if (!_lastMusic.loopFileName.empty())
      {
        playMusic(_lastMusic.loopFileName, _lastMusic.volume);
      }
    }
  }
}

void AudioHelper::appExit()
{
  end();
}

void AudioHelper::stopAllSounds()
{
  AudioEngine::stopAll();
  _lastMusic = {AudioEngine::INVALID_AUDIO_ID, AudioEngine::INVALID_AUDIO_ID, "", "", 1.f};
}

void AudioHelper::unloadAllSounds()
{
  AudioEngine::uncacheAll();
  _lastMusic = {AudioEngine::INVALID_AUDIO_ID, AudioEngine::INVALID_AUDIO_ID, "", "", 1.f};
}

void AudioHelper::stopSound(const int sound)
{
  AudioEngine::stop(sound);
}

void AudioHelper::pauseSound(const int sound)
{
  if (!getEffectsMuted())
  {
    AudioEngine::pause(sound);
  }
}

void AudioHelper::resumeSound(const int sound)
{
  if (!getEffectsMuted())
  {
    AudioEngine::resume(sound);
  }
}
