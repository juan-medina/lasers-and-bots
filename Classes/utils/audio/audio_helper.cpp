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

#include "audio_helper.h"
#include "audio/include/AudioEngine.h"

using namespace experimental;

audio_helper::audio_helper() :
  initiated_(false),
  music_muted_(false),
  effects_muted_(false),
  last_music_{AudioEngine::INVALID_AUDIO_ID, "", 1.f}
{
  init();
}

audio_helper::~audio_helper()
{
  if (initiated_)
  {
    end();
  }
}

bool audio_helper::init()
{
  if (!initiated_)
  {
    AudioEngine::lazyInit();
  }

  initiated_ = true;

  return true;
}

void audio_helper::end()
{
  if (initiated_)
  {
    AudioEngine::stopAll();
    AudioEngine::uncacheAll();
    AudioEngine::end();
  }

  initiated_ = false;
}

int audio_helper::play_effect(const std::string& file_name, const bool loop /*= false*/,
                              const float volume /*= 1.0f*/) const
{
  if (get_effects_muted())
  {
    return AudioEngine::INVALID_AUDIO_ID;
  }
  return AudioEngine::play2d(file_name, loop, volume);
}

void audio_helper::play_music(const std::string& file_name, const float volume /*= 1.0f*/)
{
  last_music_.filename = std::string(file_name);
  last_music_.volume = volume;
  if (!get_music_muted())
  {
    last_music_.id = AudioEngine::play2d(file_name, true, volume);
  }
}

void audio_helper::pre_load_effect(const std::string& file_name) const
{
  AudioEngine::preload(file_name);
}

void audio_helper::pre_load_music(const std::string& file_name) const
{
  AudioEngine::preload(file_name);
}

void audio_helper::unload_effect(const std::string& file_name) const
{
  AudioEngine::uncache(file_name);
}

void audio_helper::pause_music() const
{
  if (last_music_.id != AudioEngine::INVALID_AUDIO_ID)
  {
    if (!get_music_muted())
    {
      AudioEngine::pause(last_music_.id);
    }
  }
}

void audio_helper::resume_music()
{
  if (last_music_.id != AudioEngine::INVALID_AUDIO_ID)
  {
    if (!get_music_muted())
    {
      AudioEngine::resume(last_music_.id);
    }
  }
  else
  {
    if (!get_music_muted())
    {
      if (!last_music_.filename.empty())
      {
        play_music(last_music_.filename, last_music_.volume);
      }
    }
  }
}

void audio_helper::app_exit()
{
  end();
}

void audio_helper::stop_all_sounds()
{
  AudioEngine::stopAll();
}

void audio_helper::stop_sound(const int sound)
{
  AudioEngine::stop(sound);
}

void audio_helper::pause_sound(const int sound)
{
  if (!get_effects_muted())
  {
    AudioEngine::pause(sound);
  }
}

void audio_helper::resume_sound(const int sound)
{
  if (!get_effects_muted())
  {
    AudioEngine::resume(sound);
  }
}
