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

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

audio_helper::audio_helper():
  initiated_(false),
  music_muted_(false),
  effects_muted_(false),
  last_music_("")
{
  // init object
  init();
}

audio_helper::~audio_helper()
{
  // if we are init we need to end
  if (initiated_)
  {
    end();
  }
}

bool audio_helper::init()
{
  // get configuration
  set_effects_muted(UserDefault::getInstance()->getBoolForKey("effects_muted", effects_muted_));
  set_music_muted(UserDefault::getInstance()->getBoolForKey("music_muted", music_muted_));

  // not music played, yet
  last_music_ = "";

  // object is init
  initiated_ = true;

  return true;
}

void audio_helper::end()
{
  // end audio engine
  SimpleAudioEngine::end();

  // save config values
  save_values();

  // we are not init
  initiated_ = false;
}

unsigned int audio_helper::play_effect(const char* file_name) const
{
  // if we are muted exit
  if (get_effects_muted())
  {
    return 0;
  }
  // play effect
  return SimpleAudioEngine::getInstance()->playEffect(file_name);
}

void audio_helper::play_music(const char* file_name)
{
  // store music name
  last_music_ = std::string(file_name);

  // if we are muted exit
  if (get_music_muted())
  {
    return;
  }
  // play music
  SimpleAudioEngine::getInstance()->playBackgroundMusic(last_music_.c_str(), true);
}

void audio_helper::pre_load_effect(const char* file_name)
{
  // pre-load effect
  SimpleAudioEngine::getInstance()->preloadEffect(file_name);
}

void audio_helper::pre_load_music(const char* file_name)
{
  // pre-load music
  SimpleAudioEngine::getInstance()->preloadBackgroundMusic(file_name);
}

void audio_helper::unload_effect(const char* file_name)
{
  SimpleAudioEngine::getInstance()->unloadEffect(file_name);
}

void audio_helper::toggle_sound()
{
  // if we are muted
  if (get_effects_muted())
  {
    // un mute
    set_effects_muted(false);
  }

  else
  {
    // stop all effects and set to mute
    SimpleAudioEngine::getInstance()->stopAllEffects();
    set_effects_muted(true);
  }

  // save config values
  save_values();
}

void audio_helper::toggle_music()
{
  // if we are mute
  if (get_music_muted())
  {
    // un mute
    set_music_muted(false);

    // if we have last music
    if (last_music_.length())
    {
      // play again
      play_music(last_music_.c_str());
    }
  }

  else
  {
    // stop music and mute
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    set_music_muted(true);
  }

  // save config values
  save_values();
}

void audio_helper::app_to_bg() const
{
  // if we wasn't muted pause music
  if (!get_music_muted())
  {
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
  }
}

void audio_helper::app_to_fg() const
{
  // if we wasn't muted resume music
  if (!get_music_muted())
  {
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
  }
}

void audio_helper::save_values() const
{
  // save config values
  UserDefault::getInstance()->setBoolForKey("effects_muted", effects_muted_);
  UserDefault::getInstance()->setBoolForKey("music_muted_", music_muted_);

  UserDefault::getInstance()->flush();
}

void audio_helper::app_exit()
{
  end();
}

void audio_helper::stop_all_sounds()
{
  SimpleAudioEngine::getInstance()->stopAllEffects();
  SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
