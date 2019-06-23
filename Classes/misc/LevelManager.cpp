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

#include "LevelManager.h"

LevelManager::LevelManager() : _initiated(false), _numLevels(0) {}

LevelManager::~LevelManager()
{
  if (_initiated)
  {
    end();
  }
}

bool LevelManager::init()
{
  auto result = false;

  do
  {
    if (_initiated)
    {
      end();
    }

    _levelData = FileUtils::getInstance()->getValueMapFromFile("levels/levels.plist");
    UTILS_BREAK_IF(_levelData.empty());

    const auto metadata = _levelData.at("metadata").asValueMap();
    UTILS_BREAK_IF(metadata.empty());

    const auto format = metadata.at("format").asString();
    UTILS_BREAK_IF(format != "levels");

    const auto version = metadata.at("version").asValueMap();
    UTILS_BREAK_IF(version.empty());

    const auto major = version.at("major").asInt();
    UTILS_BREAK_IF(major != 1);

    const auto minor = version.at("minor").asInt();
    UTILS_BREAK_IF(minor != 0);

    const auto levels = _levelData.at("levels").asValueVector();
    _numLevels = levels.size();

    _initiated = true;
    result = true;
  } while (false);

  return result;
}

void LevelManager::end()
{
  if (_initiated)
  {
    // clear stuff
  }

  _initiated = false;
}

CompletedResult LevelManager::setLevelCompleted(const unsigned short int level,
                                                const unsigned short int stars, const float time) const
{
  auto result = CompletedResult::NoRecordChange;

  const auto userDefault = UserDefault::getInstance();
  const auto currentStars = getLevelStars(level);
  const auto key = StringFormat("level_%04d", level);
  const auto currentRecord = getLevelTimeRecord(level);
  if (time < currentRecord)
  {
    userDefault->setFloatForKey((key + "_time_record").c_str(), time);
    result = CompletedResult::NewLevelRecord;
  }
  if (stars == 3)
  {
    const auto current3StarsRecord = getLevel3StarsRecord(level);
    if (time < current3StarsRecord)
    {
      userDefault->setFloatForKey((key + "_3_stars_time_record").c_str(), time);
      if (result == CompletedResult::NoRecordChange)
      {
        result = CompletedResult::NewLevel3StarsRecord;
      }
      else
      {
        result = CompletedResult::NewLevelRecordAnd3StarsRecord;
      }
    }
  }
  if (stars > currentStars)
  {
    userDefault->setIntegerForKey((key + "_stars").c_str(), stars);
  }

  return result;
}

float LevelManager::getLevelTimeRecord(const unsigned short level) const
{
  const auto key = StringFormat("level_%04d_time_record", level);
  return UserDefault::getInstance()->getFloatForKey(key.c_str(), NO_TIME_RECORD);
}

float LevelManager::getLevel3StarsRecord(const unsigned short level) const
{
  const auto key = StringFormat("level_%04d_3_stars_time_record", level);
  return UserDefault::getInstance()->getFloatForKey(key.c_str(), NO_TIME_RECORD);
}

unsigned short int LevelManager::getLevelStars(const unsigned short int level) const
{
  const auto key = StringFormat("level_%04d_stars", level);
  return UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
}

bool LevelManager::isLevelEnabled(const unsigned short level) const
{
  if (level == 1)
  {
    return true;
  }
  return getLevelStars(level - 1) > 0;
}

std::string LevelManager::getLevelMap(const unsigned short level) const
{
  if (level <= _numLevels)
  {
    const auto levels = _levelData.at("levels").asValueVector();
    const auto levelData = levels.at(level - 1).asValueMap();
    return levelData.at("map").asString();
  }

  return std::string("");
}

std::string LevelManager::getLevelName(const unsigned short level) const
{
  if (level <= _numLevels)
  {
    const auto levels = _levelData.at("levels").asValueVector();
    const auto levelData = levels.at(level - 1).asValueMap();
    return levelData.at("name").asString();
  }

  return std::string("");
}

int LevelManager::getLevelTimeLimit(const unsigned short level) const
{
  if (level <= _numLevels)
  {
    const auto levels = _levelData.at("levels").asValueVector();
    const auto levelData = levels.at(level - 1).asValueMap();
    return levelData.at("time_limit").asInt();
  }

  return 0;
}

std::string LevelManager::getLevelMusic(const unsigned short level) const
{
  if (level <= _numLevels)
  {
    const auto levels = _levelData.at("levels").asValueVector();
    const auto levelData = levels.at(level - 1).asValueMap();
    return levelData.at("music").asString();
  }

  return std::string("");
}

unsigned short int LevelManager::getNextLevel(const unsigned short int level) const
{
  if (level < _numLevels)
  {
    return static_cast<unsigned short int>(level + 1);
  }

  return _numLevels;
}
