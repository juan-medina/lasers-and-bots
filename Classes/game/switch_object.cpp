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

#include "switch_object.h"

switch_object::switch_object():
  on_(false)
{
}

switch_object* switch_object::create(const string& target)
{
  switch_object* ret = nullptr;

  do
  {
    auto object = new switch_object();
    UTILS_BREAK_IF(object == nullptr);

    if (object->init(target))
    {
      object->autorelease();
    }
    else
    {
      delete object;
      object = nullptr;
    }

    ret = object;
  }
  while (false);

  return ret;
}

bool switch_object::init(const string& target)
{
  auto ret = false;

  do
  {
    UTILS_BREAK_IF(!base_class::init("09_Switch (2).png", "switch"));

    UTILS_BREAK_IF(!set_shape("08_Switch"));

    target_ = target;

    ret = true;
  }
  while (false);

  return ret;
}

void switch_object::on()
{
  if (is_off())
  {
    change_frame("08_Switch (1).png");
    on_ = true;
  }
}
