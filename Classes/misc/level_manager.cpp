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

#include "level_manager.h"

level_manager::level_manager() :
  initiated_(false),
  num_levels_(0)
{
}

level_manager::~level_manager()
{
  if (initiated_)
  {
    end();
  }
}

bool level_manager::init()
{
  auto result = false;

  do
  {
    if (initiated_)
    {
      end();
    }

    level_data_ = FileUtils::getInstance()->getValueMapFromFile("levels/levels.plist");
    UTILS_BREAK_IF(level_data_.empty());

    const auto metadata = level_data_.at("metadata").asValueMap();
    UTILS_BREAK_IF(metadata.empty());

    const auto format = metadata.at("format").asString();
    UTILS_BREAK_IF(format != "levels");

    const auto version = metadata.at("version").asValueMap();
    UTILS_BREAK_IF(version.empty());

    const auto major = version.at("major").asInt();
    UTILS_BREAK_IF(major != 1);

    const auto minor = version.at("minor").asInt();
    UTILS_BREAK_IF(minor != 0);

    const auto levels = level_data_.at("levels").asValueVector();
    num_levels_ = levels.size();

    initiated_ = true;
    result = true;
  }
  while (false);

  return result;
}

void level_manager::end()
{
  if (initiated_)
  {
    // clear stuff
  }

  initiated_ = false;
}

void level_manager::set_level_stars(const unsigned short int level, const unsigned short int stars) const
{
  const auto current_stars = get_level_stars(level);

  if (stars > current_stars)
  {
    auto key = string_format("level_%04d_stars", level);
    UserDefault::getInstance()->setIntegerForKey(key.c_str(), stars);
  }
}

unsigned short int level_manager::get_level_stars(const unsigned short int level) const
{
  auto key = string_format("level_%04d_stars", level);
  return UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
}

bool level_manager::is_level_enabled(const unsigned short level) const
{
  if (level == 1)
  {
    return true;
  }
  return get_level_stars(level - 1) > 0;
}

std::string level_manager::get_level_map(const short level) const
{
  if (level <= num_levels_)
  {
    const auto levels = level_data_.at("levels").asValueVector();
    const auto level_data = levels.at(level - 1).asValueMap();
    return level_data.at("map").asString();
  }

  return std::string("");
}

std::string level_manager::get_level_name(const short level) const
{
  if (level <= num_levels_)
  {
    const auto levels = level_data_.at("levels").asValueVector();
    const auto level_data = levels.at(level - 1).asValueMap();
    return level_data.at("name").asString();
  }

  return std::string("");
}

int level_manager::get_level_time_limit(const short level) const
{
  if (level <= num_levels_)
  {
    const auto levels = level_data_.at("levels").asValueVector();
    const auto level_data = levels.at(level - 1).asValueMap();
    return level_data.at("time_limit").asInt();
  }

  return 0;
}

std::string level_manager::get_level_music(const short level) const
{
  if (level <= num_levels_)
  {
    const auto levels = level_data_.at("levels").asValueVector();
    const auto level_data = levels.at(level - 1).asValueMap();
    return level_data.at("music").asString();
  }

  return std::string("");
}
