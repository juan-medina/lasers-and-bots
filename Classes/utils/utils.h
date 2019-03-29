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

#ifndef __UTILS_H__
#define __UTILS_H__

// include cocos2d headers
#include "cocos2d.h"

// usign cocos2d namespace
USING_NS_CC;
using namespace std;

// log a line with the format <class::function> : <data to be loged> -> <file>(<line number>)
#define UTILS_LOG(s, ...) CCLOG(string(string("%s : ") + string(s) + string(" -> %s (%d)")).c_str(), __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__)

// Replace for CC_BREAK_IF that logs when condintion is true
#define UTILS_BREAK_IF(cond)            \
    if (cond)                           \
    {                                   \
        UTILS_LOG("(%s)", "" #cond ""); \
        break;                          \
    }

// subtract color2 from color 1
#define COLORS3_SUB(color1, color2) Color3B(color1.r - color2.r, color1.g - color2.g, color1.b - color2.b)

// add color 2 to color 1
#define COLORS3_ADD(color1, color2) Color3B(color1.r + color2.r, color1.g + color2.g, color1.b + color2.b)

// multiplies color components by a factor
#define COLORS3_MUL(color, factor) Color3B(color.r* factor, color.g* factor, color.b* factor)

// interpolate two colors by a factor
#define COLORS3_INTERPOLATE(color1, color2, factor) Color3B(color1.r + (color2.r - color1.r) * factor, color1.g + (color2.g - color1.g) * factor, color1.b + (color2.b - color1.b) * factor)

std::string string_format(const std::string fmt_str, ...);

#endif // __UTILS_H__
