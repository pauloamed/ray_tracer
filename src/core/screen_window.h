#ifndef SCREEN_WINDOW_H
#define SCREEN_WINDOW_H

#include "math_base.h"

namespace rt3{

struct ScreenWindow{
    real_type left, right;
    real_type bottom, top;

    ScreenWindow() = default;

    ScreenWindow(real_type l, real_type r, real_type b, real_type t):
    left(l), right(r), bottom(b), top(t){}

    ScreenWindow(const vector<real_type> &vals):
      ScreenWindow(vals[0], vals[1], vals[2], vals[3]){}

    real_type width() const { return right - left; }
    real_type height() const { return top - bottom; }
};

}




#endif