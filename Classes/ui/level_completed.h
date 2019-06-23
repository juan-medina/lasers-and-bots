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

#include "../menu/basic_menu.h"
#include "../utils/utils.h"

// foward declarations
class AudioHelper;
class LevelManager;
enum class CompletedResult;
class TextButton;

class level_completed final : public basic_menu
{
public:
  using base_class = basic_menu;

  level_completed();

  static level_completed* create(AudioHelper* audio_helper, LevelManager* level_manager);

  bool init(AudioHelper* audio_helper, LevelManager* level_manager);

  void display(const unsigned short int level, const float time, const unsigned short int stars,
               const CompletedResult completion, const ccMenuCallback& callback);

  void hide() override;

private:
  Label* add_labels(const std::string& label_text, const std::string& text, const Vec2& pos,
                    const float separation);

  void star_sound() const;

  void animate_label(Label* label) const;

protected:
  bool create_menu_items() override;

private:
  AudioHelper* audio_helper_;
  TextButton* continue_item_;
  Label* level_name_label_;

  std::vector<Sprite*> gray_stars_;
  std::vector<Sprite*> gold_stars_;
  std::vector<Label*> label_stars_;

  LevelManager* level_manager_;
  Label* level_total_time_label_;
  Label* level_time_limit_label_;
  Label* level_time_record_label_;
  Label* level_3_stars_record_label_;
};

#endif // __LEVEL_COMPLETED_CLASS__
