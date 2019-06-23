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

enum class CompletedResult
{
  NoRecordChange,
  NewLevelRecord,
  NewLevel3StarsRecord,
  NewLevelRecordAnd3StarsRecord
};

// class that represent a audio engine
class LevelManager final : public Ref
{
public:
  using BaseClass = Ref;

  LevelManager();
  ~LevelManager();

  bool init();
  void end();

  const unsigned short int& getNumLevels() const { return _numLevels; }

  unsigned short int getLevelStars(const unsigned short int level) const;
  bool isLevelEnabled(const unsigned short int level) const;
  std::string getLevelMap(const unsigned short int level) const;
  std::string getLevelName(const unsigned short int level) const;
  int getLevelTimeLimit(const unsigned short int level) const;
  std::string getLevelMusic(const unsigned short int level) const;
  CompletedResult setLevelCompleted(const unsigned short int level, const unsigned short int stars,
                                    const float time) const;
  float getLevelTimeRecord(const unsigned short int level) const;
  float getLevel3StarsRecord(const unsigned short int level) const;
  unsigned short int getNextLevel(const unsigned short int level) const;

  static constexpr auto NO_TIME_RECORD = 999999.f;

private:
  bool _initiated;
  unsigned short int _numLevels;

  ValueMap _levelData;
};

#endif // __LEVEL_MANAGER_H__
