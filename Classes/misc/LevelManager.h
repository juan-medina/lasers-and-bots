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

#ifndef __LEVELMANAGER_H__
#define __LEVELMANAGER_H__

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
  ~LevelManager() override;

  bool init();
  void end();

  const unsigned short int& getNumLevels() const { return _numLevels; }

  unsigned short int getLevelStars(unsigned short int level) const;
  bool isLevelEnabled(unsigned short int level) const;
  std::string getLevelMap(unsigned short int level) const;
  std::string getLevelName(unsigned short int level) const;
  int getLevelTimeLimit(unsigned short int level) const;
  std::string getLevelMusic(unsigned short int level) const;
  CompletedResult setLevelCompleted(unsigned short int level, unsigned short int stars, float time) const;
  float getLevelTimeRecord(unsigned short int level) const;
  float getLevel3StarsRecord(unsigned short int level) const;
  unsigned short int getNextLevel(unsigned short int level) const;

  static constexpr auto NO_TIME_RECORD = 999999.f;

private:
  bool _initiated;
  unsigned short int _numLevels;

  ValueMap _levelData;
};

#endif //__LEVELMANAGER_H__
