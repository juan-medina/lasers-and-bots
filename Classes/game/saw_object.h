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
#ifndef __SAW_CLASS__
#define __SAW_CLASS__

#include "harm_object.h"

class saw_object final : public harm_object
{
public:
  using base_class = harm_object;

  saw_object();

  static saw_object* create(physics_shape_cache* physics_shape_cache, const std::string& image,
                            const std::string& shape, const int damage, const float rotation_time, const float movement,
                            const float movement_time, const float stop_time);

  bool init(physics_shape_cache* physics_shape_cache, const std::string& image, const std::string& shape,
            const int damage, const float rotation_time, const float movement, const float movement_time,
            const float stop_time);
};


#endif // __SAW_CLASS__
