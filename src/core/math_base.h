#ifndef MATH_BASE_H
#define MATH_BASE_H

#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include "error.h"
#include <sstream>

using std::array;
using std::string;
using std::vector;
using std::pair;
using std::copy_n;
using std::min;
using std::max;
using std::swap;

namespace rt3{

//=== aliases
typedef float real_type;
typedef size_t size_type;
typedef std::tuple<bool, std::string> result_type;

/*! Linear interpolation.
 * \param t The parameter, in [0,1].
 * \param v1 The initial interpolation value.
 * \param v2 The final interpolation value.
 * \return The interpolated value.
 */
//
inline float Lerp(float t, float v1, float v2) {
  return (1.f - t) * v1 + t * v2;
}

/// Clamp T to [low,high].
template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
  if (val < low)
    return low;
  else if (val > high)
    return high;
  else
    return val;
}

/// Degrees to radians.
inline real_type Radians(real_type deg) { return ((real_type)M_PI / 180.f) * deg; }

/// Radians to degreees.
inline real_type Degrees(real_type rad) { return (180.f / (real_type)M_PI) * rad; }


template<typename T>
T fastExp(T x, int exp){
    T ans = T(1);
    while(exp > 0){
        if(exp%2) ans = ans * x; 
        exp >>= 1;
        x = x * x;
    }
    return ans;
}


} // namespace rt3


#endif