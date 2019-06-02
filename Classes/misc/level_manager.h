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

#ifndef __LEVEL_MANAGER_H__
#define __LEVEL_MANAGER_H__

#include "../utils/utils.h"

enum class completed_result
{
  no_record_change,
  new_level_record,
  new_level_3_stars_record,
  new_level_record_and_3_stars_record
};

// class that represent a audio engine
class level_manager final : public Ref
{
public:
  typedef Ref parent;

  level_manager();
  ~level_manager();

  bool init();
  void end();

  const unsigned short int& get_num_levels() const { return num_levels_; }

  unsigned short int get_level_stars(const unsigned short int level) const;
  bool is_level_enabled(const unsigned short int level) const;
  std::string get_level_map(const unsigned short int level) const;
  std::string get_level_name(const unsigned short int level) const;
  int get_level_time_limit(const unsigned short int level) const;
  std::string get_level_music(const unsigned short int level) const;
  completed_result set_level_completed(const unsigned short int level, const unsigned short int stars,
                                       const float time) const;
  float get_level_time_record(const unsigned short int level) const;
  float get_level_3_stars_record(const unsigned short int level) const;
  static constexpr auto no_time_record = 999999.f;

private:
  bool initiated_;
  unsigned short int num_levels_;

  ValueMap level_data_;
};

#endif // __LEVEL_MANAGER_H__
