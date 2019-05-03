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
class audio_helper final : public Ref
{
public:
  // base_class
  typedef Ref parent;

  // constructor
  audio_helper();

  // destructor
  ~audio_helper();

  // play an effect
  int play_effect(const std::string& file_name, const bool loop = false, const float volume = 1.0f) const;

  // play music
  void play_music(const std::string& file_name, const float volume = 1.0f);

  // pre-load an effect
  static void pre_load_effect(const std::string& file_name);

  // pre-load music
  static void pre_load_music(const std::string& file_name);

  // unload and effect
  static void unload_effect(const std::string& file_name);

  // toggle sound
  void toggle_sound();

  // toggle music
  void toggle_music();

  // pause the music
  void pause_music() const;

  // resume the music
  void resume_music() const;

  // app is going to background
  void app_to_bg() const;

  // app is going to foreground
  void app_to_fg() const;

  // the application is going to exit
  void app_exit();

  // get default audio helper
  static audio_helper* get_instance()
  {
    // create if not create default helper and return it
    static audio_helper helper;

    return &helper;
  }

  // stops all sounds
  static void stop_all_sounds();
  void stop_sound(int sound);
  void pause_sound(int sound);
  void resume_sound(int sound);

  bool get_music_muted() const noexcept
  {
    return music_muted_;
  }

  void set_music_muted(const bool var) noexcept
  {
    music_muted_ = var;
  };

  bool get_effects_muted() const noexcept
  {
    return effects_muted_;
  }

  void set_effects_muted(const bool var) noexcept
  {
    effects_muted_ = var;
  };

  int get_last_music() const noexcept
  {
    return last_music_;
  };

  // end helper
  void end();


protected:

  // is the object initiated ?
  bool initiated_;

  // init helper
  bool init();


  // is music muted
  bool music_muted_;

  // is effects muted
  bool effects_muted_;

  // last music requested to play
  int last_music_;
};

#endif // __AUDIO_HELPER_H__
