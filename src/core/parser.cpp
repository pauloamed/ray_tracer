/*!
 * Implementation of XML processing functions.
 * @file parser.h
 */

#include "parser.h"
#include "../api/api.h"
#include "paramset.h"
#include "rt3.h"

// === Function Implementation

namespace rt3 {

using rt3::Point3f;
using rt3::Vector3f;
using rt3::Vector3i;

template <typename T> T getPrimValue(stringstream &ss) {
  if constexpr (std::is_same_v<T, bool>) {
    string boolean;
    ss >> boolean;
    if (boolean == "true") {
      return true;
    } else if (boolean == "false") {
      return false;
    } else {
      RT3_ERROR("Invalid boolean value.");
      return T();
    }
  } else {
    T ret;
    ss >> ret;
    return ret;
  }
}

template <typename T>
vector<T> getMultipleValues(stringstream &ss, int size = -1) {
  vector<T> values;
  if (size == -1) {
    while (ss.good())
      values.push_back(getPrimValue<T>(ss));
  } else {
    values.resize(size);
    for (auto &x : values)
      x = getPrimValue<T>(ss);
  }
  return values;
}

template <typename T>
void parse_enum_attrib(stringstream &ss, ParamSet *ps_out, const string &name,
                       const vector<string> &names_list) {
  auto val = getPrimValue<string>(ss);
  for (size_t i = 0; i < names_list.size(); ++i) {
    if (val == names_list[i]) {
      (*ps_out)[name] = make_shared<Value<T>>(Value<T>(T(i)));
      return;
    }
  }
  RT3_ERROR("Couldn't match Enum.");
}

template <typename T>
void parse_single_prim_attrib(stringstream &ss, ParamSet *ps_out,
                              const string &name) {
  (*ps_out)[name] = make_shared<Value<T>>(Value<T>(getPrimValue<T>(ss)));
}

template <typename T_INTERNAL, typename T, int size>
void parse_single_composite_attrib(stringstream &ss, ParamSet *ps_out,
                                   const string &name) {

  vector<T_INTERNAL> values = getMultipleValues<T_INTERNAL>(ss, size);

  (*ps_out)[name] = make_shared<Value<T>>(Value<T>(T(values)));
}

void parse_color(stringstream &ss, ParamSet *ps_out, const string &name) {

  string peak(ss.str());
  vector<real_type> values = getMultipleValues<real_type>(ss, 3);
  if(peak.find(".") != string::npos){ // eh real
    (*ps_out)[name] = make_shared<Value<Color>>(Value<Color>(Color::make_color_from_real(values)));
  }else{
    (*ps_out)[name] = make_shared<Value<Color>>(Value<Color>(Color::make_color_from_int(values)));
  }
}

template <typename T, typename T_INTERNAL, int internal_size>
void parse_array_composite_attrib(stringstream &ss, ParamSet *ps_out,
                                  const string &name) {

  vector<T> values;
  while (ss.good()) {
    values.push_back(T(getMultipleValues<T_INTERNAL>(ss, internal_size)));
  }

  (*ps_out)[name] =
      make_shared<Value<vector<T>>>(Value<vector<T>>(vector<T>(values)));
}

template <typename T, typename T_INTERNAL, int internal_size>
void parse_ptr_array_ptr_composite_attrib(stringstream &ss, ParamSet *ps_out,
                                  const string &name) {

  using e_type = shared_ptr<T>;
  using vec_type = vector<e_type>;
  using final_type = shared_ptr<vec_type>;

  vec_type values;
  while (ss.good()) {
    auto element = getMultipleValues<T_INTERNAL>(ss, internal_size);
    values.push_back(make_shared<T>(element));
  }

  final_type val = make_shared<vec_type>(values);
  auto value = Value<final_type>(val);
  (*ps_out)[name] = make_shared<Value<final_type>>(val);
}

template <typename T>
void parse_array_prim_attrib(stringstream &ss, ParamSet *ps_out,
                             const string &name) {

  vector<T> values = getMultipleValues<T>(ss);

  (*ps_out)[name] =
      make_shared<Value<vector<T>>>(Value<vector<T>>(vector<T>(values)));
}

template <typename T>
void parse_ptr_array_prim_attrib(stringstream &ss, ParamSet *ps_out,
                             const string &name) {

  using vec_type = vector<T>;
  using final_type = shared_ptr<vec_type>;

  vec_type values = getMultipleValues<T>(ss);
  final_type val = make_shared<vec_type>(values);

  auto value = Value<final_type>(val);

  (*ps_out)[name] =
      make_shared<Value<final_type>>(value);
}

/// This is the entry function for the parsing process.
void parse(const char *scene_file_name) {
  tinyxml2::XMLDocument xml_doc;

  // Load file.
  if (xml_doc.LoadFile(scene_file_name) != tinyxml2::XML_SUCCESS)
    RT3_ERROR(std::string{"The file \""} + scene_file_name +
              std::string{"\" either is not available OR contains an invalid "
                          "RT3 scene provided!"});

  // ===============================================
  // Get a pointer to the document's root node.
  tinyxml2::XMLNode *p_root = xml_doc.FirstChild();
  if (p_root == nullptr)
    RT3_ERROR("Error while trying to find \"RT3\" tag in the scene file.");
  // ===============================================

  // Get the first-level tag inside the root node.
  auto *p_child{p_root->FirstChildElement()};
  if (p_child == nullptr)
    RT3_ERROR(
        "No \"children\" tags found inside the \"RT3\" tag. Empty scene file?");

  parse_tags(p_child, /* initial level */ 0);
}

/// Main loop that handles each possible tag we may find in a RT3 scene file.
void parse_tags(tinyxml2::XMLElement *p_element, int level) {
  // All messages sent to std::clog is for DEBUG purposes.
  std::clog << "[parse_tags()]: level is " << level << std::endl;

  // Traverse all items on the children's level.
  while (p_element != nullptr) {
    // Convert the attribute name to lowecase before testing it.
    auto tag_name = CSTR_LOWERCASE(p_element->Value());
    clog << "\n"
         << setw(level * 3) << ""
         << "***** Tag id is `" << tag_name << "`, at level " << level
         << std::endl;

    // Big switch for each possible RT3 tag type.
    if (tag_name == "background") {
      ParamSet ps;

      // TODO: retrieve information from the XML child into the ParamSet.
      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::BG_TYPE, "type"},
          {param_type_e::STRING, "filename"}, // Texture file name.
          {param_type_e::MAPPING, "mapping"}, // Type of mapping required.
          {param_type_e::COLOR, "color"}, // Single color for the entire background.
          {param_type_e::COLOR, "tl"}, // Top-left corner
          {param_type_e::COLOR, "tr"}, // Top-right corner
          {param_type_e::COLOR, "bl"}, // Bottom-left corner
          {param_type_e::COLOR, "br"}  // Bottom-right corner
      };
      parse_parameters(p_element, param_list, /* out */ &ps);

      // Calling the corresponding API method.
      API::background(ps);
    } else if (tag_name == "film") {
      ParamSet ps;

      // TODO: retrieve information from the XML child into the ParamSet.
      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::STRING, "type"},
          {param_type_e::STRING, "filename"},
          {param_type_e::IMAGE_TYPE, "img_type"},
          {param_type_e::INT, "x_res"},
          {param_type_e::INT, "y_res"},
          {param_type_e::ARR_REAL, "crop_window"},
          {param_type_e::STRING, "gamma_corrected"} // bool
      };
      parse_parameters(p_element, param_list, /* out */ &ps);

      // Calling the corresponding API method.
      API::film(ps);
    } else if (tag_name == "camera") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::CAMERA_TYPE, "type"},
          {param_type_e::SCREEN_WINDOW, "screen_window"},
          {param_type_e::REAL, "fovy"},
      };
      parse_parameters(p_element, param_list, &ps);

      API::camera(ps);
    } else if (tag_name == "object_instance_call") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::STRING, "name"}
      };
      parse_parameters(p_element, param_list, &ps);

      API::instantiate_obj(ps);
    } else if (tag_name == "object_instance_begin") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::STRING, "name"}
      };
      parse_parameters(p_element, param_list, &ps);

      API::start_obj_instance(ps);
    } else if (tag_name == "object_instance_end") {
      API::finish_obj_instance();
    } else if (tag_name == "push_gs") {
      API::push_GS();
    } else if (tag_name == "pop_gs") {
      API::pop_GS();
    } else if (tag_name == "push_ctm") {
      API::push_CTM();
    } else if (tag_name == "pop_ctm") {
      API::pop_CTM();
    } else if (tag_name == "identity") {
      API::identity();
    } else if (tag_name == "translate") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::VEC3F, "value"}
      };

      parse_parameters(p_element, param_list, &ps);

      API::translate(ps);
    } else if (tag_name == "scale") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::POINT3F, "value"}
      };

      parse_parameters(p_element, param_list, &ps);

      API::scale(ps);      
    } else if (tag_name == "rotate") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::VEC3F, "axis"},
          {param_type_e::REAL, "angle"}
      };

      parse_parameters(p_element, param_list, &ps);

      API::rotate(ps);      
    } else if (tag_name == "save_coord_system") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::STRING, "name"}
      };

      parse_parameters(p_element, param_list, &ps);

      API::save_coord_system(ps);       
    } else if (tag_name == "restore_coord_system") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::STRING, "name"}
      };

      parse_parameters(p_element, param_list, &ps);

      API::restore_coord_system(ps);         
    } else if (tag_name == "lookat") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::POINT3F, "look_from"},
          {param_type_e::POINT3F, "look_at"},
          {param_type_e::VEC3F, "up"},
      };
      parse_parameters(p_element, param_list, &ps);

      API::lookat(ps);
    } else if (tag_name == "integrator") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::INTEGRATOR_TYPE, "type"},

          // Blinn Phong
          {param_type_e::INT, "depth"},

          // Depth map integrator
          {param_type_e::REAL, "zmin"},
          {param_type_e::REAL, "zmax"},
          {param_type_e::COLOR, "near_color"},
          {param_type_e::COLOR, "far_color"},
      };
      parse_parameters(p_element, param_list, &ps);

      API::integrator(ps);
    } else if (tag_name == "material") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::MATERIAL_TYPE, "type"},

          // Flat
          {param_type_e::COLOR, "color"},

          // Blinn Phong
          {param_type_e::COLOR, "ambient"},
          {param_type_e::COLOR, "diffuse"},
          {param_type_e::COLOR, "specular"},
          {param_type_e::COLOR, "mirror"},
          {param_type_e::INT, "glossiness"},

      };
      parse_parameters(p_element, param_list, &ps);

      API::material(ps);
    } else if (tag_name == "make_named_material") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::MATERIAL_TYPE, "type"},

          // Name
          {param_type_e::STRING, "name"},

          // Flat
          {param_type_e::COLOR, "color"},

          // Blinn Phong
          {param_type_e::COLOR, "ambient"},
          {param_type_e::COLOR, "diffuse"},
          {param_type_e::COLOR, "specular"},
          {param_type_e::COLOR, "mirror"},
          {param_type_e::INT, "glossiness"},

      };
      parse_parameters(p_element, param_list, &ps);

      API::create_named_material(ps);
    } else if (tag_name == "named_material") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::STRING, "name"},
      };
      parse_parameters(p_element, param_list, &ps);

      API::named_material(ps);
    } else if (tag_name == "push_gs") {
      
    } else if (tag_name == "pop_gs") {
      
    } else if (tag_name == "accelerator") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::ACCELERATOR_TYPE, "type"},
          {param_type_e::INT, "max_prims_per_node"},
      };
      
      parse_parameters(p_element, param_list, &ps);

      API::accelerator(ps);
    } else if (tag_name == "object") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::OBJECT_TYPE, "type"},

          // Sphere
          {param_type_e::REAL, "radius"},
          {param_type_e::POINT3F, "center"},

          // Trianglemesh
          {param_type_e::STRING, "filename"},

          {param_type_e::INT, "ntriangles"},
          {param_type_e::PTR_ARR_INT, "indices"},
          {param_type_e::PTR_ARR_PTR_POINT3F, "vertices"},
          {param_type_e::PTR_ARR_PTR_NORMAL3F, "normals"},
          {param_type_e::PTR_ARR_PTR_POINT2F, "uv"},
          {param_type_e::BOOL, "reverse_vertex_order"},
          {param_type_e::BOOL, "compute_normals"},
          {param_type_e::BOOL, "backface_cull"},

      };
      parse_parameters(p_element, param_list, &ps);

      API::object(ps);
    } else if (tag_name == "light_source") {
      ParamSet ps;

      vector<std::pair<param_type_e, string>> param_list{
          {param_type_e::LIGHT_TYPE, "type"},

          // ambient and directional
          {param_type_e::COLOR, "L"},

          // point and spot
          {param_type_e::COLOR, "I"},

          // directional, point and spot
          {param_type_e::VEC3F, "scale"},
          {param_type_e::POINT3F, "from"},

          // direction and spot
          {param_type_e::POINT3F, "to"},

          // spot
          {param_type_e::REAL, "cutoff"},
          {param_type_e::REAL, "falloff"},

      };
      parse_parameters(p_element, param_list, &ps);

      API::light(ps);
    } else if (tag_name == "world_begin") {
      // We should get only one `world` tag per scene file.
      API::world_begin();
    } else if (tag_name == "world_end") {
      API::world_end();
    } else if (tag_name == "include") {
      parse(p_element->Attribute("filename"));
    } else if (tag_name == "render_again") {
      API::world_begin();
      API::world_end();
    } else {
      RT3_WARNING("Undefined tag `" + tag_name + "` found!");
    }

    // Get next (to the right) sibling on this tree level.
    p_element = p_element->NextSiblingElement();
  }

  // API::reset_engine();
}

/// Universal parameters parser.
/*!
 * This function receives a list of pairs <param_type, name>, traverse all the
 * attributes found in `p_element` and extract the attribute values into the
 * `ps_out` `ParamSet` object. Only named attributes found are read into the
 * `ps_out`.
 *
 * @param p_element XML element we are extracting information from.
 * @param param_list List of pairs <param_type, name> we need to extract from
 * the XML element.
 * @param ps_out The `ParamSet` object we need to fill in with parameter
 * information extracted from the XML element.
 */
void parse_parameters(tinyxml2::XMLElement *p_element,
                      const vector<std::pair<param_type_e, string>> param_list,
                      ParamSet *ps_out) {
  // std::clog << "parse_parameters(): p_element = " << p_element << endl;

  // Traverse the list of paramters pairs: type + name.
  for (const auto &e : param_list) {
    const auto &[type, name] = e; // structured binding, requires C++ 17
    std::clog << "---Parsing att \"" << name << "\", type = " << (int)type
              << "\n";

    // This is just a dispatcher to the proper extraction functions.

    const char *attr_val = p_element->Attribute(name.c_str());
    if (attr_val) {
      auto str = string(attr_val);
      stringstream ss(str);
      switch (type) {
      // PRIMITIVES
      case param_type_e::BOOL:
        parse_single_prim_attrib<bool>(ss, ps_out, name);
        break;
      case param_type_e::UINT:
        parse_single_prim_attrib<uint>(ss, ps_out, name);
        break;
      case param_type_e::INT:
        parse_single_prim_attrib<int>(ss, ps_out, name);
        break;
      case param_type_e::REAL:
        parse_single_prim_attrib<real_type>(ss, ps_out, name);
        break;
      case param_type_e::STRING:
        parse_single_prim_attrib<string>(ss, ps_out, name);
        break;
      // ENUMS
      case param_type_e::MAPPING:
        parse_enum_attrib<mapping_t>(ss, ps_out, name, mapping_t_names);
        break;
      case param_type_e::BG_TYPE:
        parse_enum_attrib<bg_type_t>(ss, ps_out, name, bg_type_t_names);
        break;
      case param_type_e::IMAGE_TYPE:
        parse_enum_attrib<image_type_t>(ss, ps_out, name, image_type_t_names);
        break;
      case param_type_e::CAMERA_TYPE:
        parse_enum_attrib<camera_type_t>(ss, ps_out, name, camera_type_t_names);
        break;
      case param_type_e::ACCELERATOR_TYPE:
        parse_enum_attrib<accelerator_type_t>(ss, ps_out, name, accelerator_type_t_names);
        break;
      case param_type_e::INTEGRATOR_TYPE:
        parse_enum_attrib<integrator_type_t>(ss, ps_out, name,
                                             integrator_type_t_names);
        break;
      case param_type_e::MATERIAL_TYPE:
        parse_enum_attrib<material_type_t>(ss, ps_out, name,
                                           material_type_t_names);
        break;
      case param_type_e::OBJECT_TYPE:
        parse_enum_attrib<object_type_t>(ss, ps_out, name, object_type_t_names);
        break;
      case param_type_e::LIGHT_TYPE:
        parse_enum_attrib<light_type_t>(ss, ps_out, name, light_type_t_names);
        break;
      // COMPOSITES
      case param_type_e::VEC3F:
        parse_single_composite_attrib<float, Vector3f, int(3)>(ss, ps_out,
                                                               name);
        break;
      case param_type_e::VEC3I:
        parse_single_composite_attrib<int, Vector3i, int(3)>(ss, ps_out, name);
        break;
      case param_type_e::NORMAL3F:
        parse_single_composite_attrib<real_type, Normal3f, int(3)>(ss, ps_out,
                                                                   name);
        break;
      case param_type_e::POINT3F:
        parse_single_composite_attrib<real_type, Point3f, int(3)>(ss, ps_out,
                                                                  name);
        break;
      case param_type_e::POINT2I:
        parse_single_composite_attrib<int, Point2i, int(2)>(ss, ps_out, name);
        break;
      case param_type_e::COLOR:
        parse_color(ss, ps_out, name);
        break;
      case param_type_e::SCREEN_WINDOW:
        parse_single_composite_attrib<real_type, ScreenWindow, int(4)>(
            ss, ps_out, name);
        break;
      // MULTIPLE PRIMITIVES
      case param_type_e::ARR_REAL:
        parse_array_prim_attrib<real_type>(ss, ps_out, name);
        break;
      case param_type_e::ARR_INT:
        parse_array_prim_attrib<int>(ss, ps_out, name);
        break;
      // MULTIPLE COMPOSITES
      case param_type_e::ARR_VEC3F:
        parse_array_composite_attrib<Vector3f, float, 3>(ss, ps_out, name);
        break;
      case param_type_e::ARR_VEC3I:
        parse_array_composite_attrib<Vector3i, int, 3>(ss, ps_out, name);
        break;
      case param_type_e::ARR_POINT3F:
        parse_array_composite_attrib<Point3f, float, 3>(ss, ps_out, name);
        break;
      // MULTIPLE COMPOSITES PTR
      case param_type_e::PTR_ARR_PTR_NORMAL3F:
        parse_ptr_array_ptr_composite_attrib<Normal3f, float, 3>(ss, ps_out, name);
        break;
      case param_type_e::PTR_ARR_PTR_POINT2F:
        parse_ptr_array_ptr_composite_attrib<Point2f, float, 2>(ss, ps_out, name);
        break;
      case param_type_e::PTR_ARR_PTR_POINT3F:
        parse_ptr_array_ptr_composite_attrib<Point3f, float, 3>(ss, ps_out, name);
        break;
      case param_type_e::PTR_ARR_INT:
        parse_ptr_array_prim_attrib<int>(ss, ps_out, name);
        break;
      default:
        RT3_WARNING(string{"parse_params(): unkonwn param type received!"});
        break;
      }
    }

    clog << "---Done!\n";
  }
}

//-------------------------------------------------------------------------------



} // namespace rt3
