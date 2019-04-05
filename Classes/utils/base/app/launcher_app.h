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

#ifndef __LAUNCHER_APP_H__
#define __LAUNCHER_APP_H__

#include "basic_app.h"

// launcher application for desktop clients
class launcher_app : basic_app
{
public:
  // base_class
  using base_class = basic_app;

  // constructor
  launcher_app(std::string application_name, float width, float height);

  // launch application
  bool applicationDidFinishLaunching() override;

  virtual bool get_want_to_play() const
  {
    return want_to_play_;
  }

  virtual void set_want_to_play(const bool var)
  {
    want_to_play_ = var;
  }

protected:

  // do we want to play
  bool want_to_play_;
};

#endif // __LAUNCHER_APP_H__
