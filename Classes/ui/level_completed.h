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
#ifndef __LEVEL_COMPLETED_CLASS__
#define __LEVEL_COMPLETED_CLASS__

#include "../utils/utils.h"
#include "resizable_window.h"

//foward declarations
class audio_helper;
class resizable_window;
class level_manager;

class level_completed final : public resizable_window
{
public:
  using base_class = resizable_window;

  level_completed();

  static level_completed* create(audio_helper* audio_helper, level_manager* level_manager);

  bool init(audio_helper* audio_helper, level_manager* level_manager);

  void display(const unsigned short int level, const float time, const unsigned short int stars,
               const ccMenuCallback& callback);

private:

  void star_sound() const;

  audio_helper* audio_helper_;
  MenuItemSprite* continue_item_;
  Label* sub_label_;

  std::vector<Sprite*> gray_stars_;
  std::vector<Sprite*> gold_stars_;
  std::vector<Label*> label_stars_;

  level_manager* level_manager_;
};


#endif // __LEVEL_COMPLETED_CLASS__
