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

#include "audio_helper.h"
#include "audio/include/AudioEngine.h"

using namespace experimental;

audio_helper::audio_helper() :
  initiated_(false),
  music_muted_(false),
  effects_muted_(false),
  last_music_
  {
    AudioEngine::INVALID_AUDIO_ID,
    AudioEngine::INVALID_AUDIO_ID,
    "",
    "",
    1.f
  },
  music_volume_(1.f),
  sound_volume_(1.f)
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

void audio_helper::set_music_volume(const float music_volume) noexcept
{
  music_volume_ = music_volume;

  if (!get_music_muted())
  {
    if (last_music_.intro_id != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::setVolume(last_music_.intro_id, last_music_.volume * music_volume);
    }
    else if (last_music_.loop_id != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::setVolume(last_music_.loop_id, last_music_.volume * music_volume);
    }
  }
}

int audio_helper::play_effect(const std::string& file_name, const bool loop /*= false*/,
                              const float volume /*= 1.0f*/) const
{
  if (get_effects_muted())
  {
    return AudioEngine::INVALID_AUDIO_ID;
  }
  return AudioEngine::play2d(file_name, loop, volume * sound_volume_);
}

void audio_helper::play_music(const std::string& loop_file_name, const float volume /*= 1.0f*/)
{
  last_music_.intro_id = AudioEngine::INVALID_AUDIO_ID;
  last_music_.intro_file_name = "";
  last_music_.loop_file_name = std::string(loop_file_name);
  last_music_.volume = volume;
  if (!get_music_muted())
  {
    last_music_.loop_id = AudioEngine::play2d(loop_file_name, true, volume * music_volume_);
  }
}

void audio_helper::on_music_intro_ends(const int id, const std::string& file_path)
{
  if ((id == last_music_.intro_id) && (file_path == last_music_.intro_file_name))
  {
    play_music(last_music_.loop_file_name, last_music_.volume);
  }
}

void audio_helper::play_music(const std::string& intro_file_name, const std::string& loop_file_name,
                              const float volume /*= 1.0f*/)
{
  last_music_.loop_id = AudioEngine::INVALID_AUDIO_ID;
  last_music_.intro_file_name = std::string(intro_file_name);
  last_music_.loop_file_name = std::string(loop_file_name);
  last_music_.volume = volume;
  if (!get_music_muted())
  {
    last_music_.intro_id = AudioEngine::play2d(intro_file_name, false, volume * music_volume_);
    AudioEngine::setFinishCallback(last_music_.intro_id, CC_CALLBACK_2(audio_helper::on_music_intro_ends, this));
  }
}

void audio_helper::pre_load_effect(const std::string& file_name) const
{
  AudioEngine::preload(file_name);
}

void audio_helper::pre_load_music(const std::string& loop_file_name) const
{
  AudioEngine::preload(loop_file_name);
}

void audio_helper::pre_load_music(const std::string& intro_file_name, const std::string& loop_file_name) const
{
  AudioEngine::preload(intro_file_name);
  AudioEngine::preload(loop_file_name);
}

void audio_helper::unload_effect(const std::string& file_name) const
{
  AudioEngine::uncache(file_name);
}

void audio_helper::pause_music() const
{
  if (!get_music_muted())
  {
    if (last_music_.intro_id != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::pause(last_music_.intro_id);
    }
    else if (last_music_.loop_id != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::pause(last_music_.loop_id);
    }
  }
}

void audio_helper::resume_music()
{
  if (!get_music_muted())
  {
    if (last_music_.intro_id != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::resume(last_music_.intro_id);
      AudioEngine::setVolume(last_music_.intro_id, last_music_.volume * music_volume_);
    }
    else if (last_music_.loop_id != AudioEngine::INVALID_AUDIO_ID)
    {
      AudioEngine::resume(last_music_.loop_id);
      AudioEngine::setVolume(last_music_.loop_id, last_music_.volume * music_volume_);
    }
    else
    {
      if (!last_music_.intro_file_name.empty())
      {
        play_music(last_music_.intro_file_name, last_music_.loop_file_name, last_music_.volume);
      }
      else if (!last_music_.loop_file_name.empty())
      {
        play_music(last_music_.loop_file_name, last_music_.volume);
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
  last_music_ = {
    AudioEngine::INVALID_AUDIO_ID,
    AudioEngine::INVALID_AUDIO_ID,
    "",
    "",
    1.f
  };
}

void audio_helper::unload_all_sounds()
{
  AudioEngine::uncacheAll();
  last_music_ = {
    AudioEngine::INVALID_AUDIO_ID,
    AudioEngine::INVALID_AUDIO_ID,
    "",
    "",
    1.f
  };
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
