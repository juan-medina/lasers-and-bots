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

#ifndef __LEVEL_MANAGER_H__
#define __LEVEL_MANAGER_H__

#include "../utils/utils.h"

// class that represent a audio engine
class level_manager final : public Ref
{
public:
  typedef Ref parent;

  level_manager();
  ~level_manager();

  bool init();
  void end();

  void set_stars(const unsigned short int level, const unsigned short int stars) const;
  unsigned short int get_stars(const unsigned short int level) const;

  const unsigned short int& get_num_levels() const
  {
    return num_levels_;
  }

  bool is_level_enabled(const unsigned short int level) const;

  std::string get_map_level(const short int level) const;

private:

  bool initiated_;
  unsigned short int num_levels_;

  ValueMap level_data_;
};

#endif // __LEVEL_MANAGER_H__
