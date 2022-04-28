#ifndef PARSER_H
#define PARSER_H 1

/*!
 * Implementation of XML processing functions.
 * @file parser.cpp
 */

#include "../ext/tinyxml2.h"
#include <iostream>
using std::boolalpha;
using std::cerr;
using std::clog;
using std::cout;
using std::endl;
#include <iomanip>
using std::setw;
#include <string>
using std::string;
#include <memory>
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
#include <algorithm>
using std::copy;
#include <sstream>
using std::stringstream;
#include <iterator>
using std::begin;
using std::endl;
using std::istream_iterator;
#include <vector>
using std::vector;
#include <utility>
using std::pair;
#include <optional>
using std::optional;

#include "error.h"
#include "paramset.h"

namespace rt3 {

/// Lambda expression that transform a c-style string to a lowercase c++-stype
/// string version.
auto STR_LOWER = [](const char *c_str) -> std::string {
  std::string str{c_str};
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
};

// === Enumerations
/// Type of possible paramter types we may read from the input scene file.
enum class param_type_e : int {
// PRIMITIVES
  BOOL = 0,
  INT,         //!< Single integet
  UINT,        //!< Single unsigned int
  REAL,        //!< Single real number
  STRING,      //!< Single string
// ENUMS
  MAPPING,
  BG_TYPE,
  IMAGE_TYPE,
  CAMERA_TYPE,
  LIGHT_TYPE,
  INTEGRATOR_TYPE,
  ACCELERATOR_TYPE,
  MATERIAL_TYPE,
  OBJECT_TYPE,
// COMPOSITES
  VEC3F,       //!< Single Vector3f
  SCREEN_WINDOW,       //!< Single Vector3f
  VEC3I,       //!< Single Vector3i
  NORMAL3F,    //!< Single Normal3f
  POINT3F,     //!< Single Point3f
  POINT2I,     //!< Single Point2i
  COLOR,       //!< Single Color
// MULTIPLE PRIMITIVES
  ARR_INT,     //!< An array of integers
  ARR_REAL,    //!< An array of real numbers
// MULTIPLE COMPOSITES
  ARR_VEC3F,   //!< An array of Vector3f
  ARR_VEC3I,   //!< An array of Vector3i
  ARR_COLOR,   //!< An array of Color
  ARR_POINT3F, //!< An array of Point3f
  ARR_POINT2F, //!< An array of Point3f
  PTR_ARR_INT, //!< An array of Normal3f
  PTR_ARR_PTR_NORMAL3F, //!< An array of Normal3f
  PTR_ARR_PTR_POINT2F, //!< An array of Point3f
  PTR_ARR_PTR_POINT3F //!< An array of Normal3f
};
// === parsing functions.
void parse(const char *);
void parse_tags(tinyxml2::XMLElement *, int);
void parse_parameters(tinyxml2::XMLElement *p_element,
                      const vector<std::pair<param_type_e, string>> param_list,
                      ParamSet *ps_out);

//-------------------------------------------------------------------------------
} // namespace rt3

#endif // PARSER_H
