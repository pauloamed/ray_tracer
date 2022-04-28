#include "background.h"

namespace rt3 {
/*!
 * Samples a color base on spherical interpolation of an image or colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */

Color BackgroundColor::sampleXYZ(const Point2f &pixel_ndc) const {
  float t_ver = pixel_ndc.x();
  float t_hor = pixel_ndc.y();
  Color top = Color::interpolate_color(t_hor, corners[Corners_e::tl], corners[Corners_e::tr]);
  Color bottom = Color::interpolate_color(t_hor, corners[Corners_e::bl], corners[Corners_e::br]);

  return Color::interpolate_color(t_ver, top, bottom);
}


BackgroundColor *create_color_background(const ParamSet &ps) {

  BackgroundColor* background = nullptr;

  mapping_t mtype = retrieve( ps, "mapping", mapping_t::screen);
  if(ps.count("color")){
    Color c = retrieve( ps, "color", Color());
    background = new BackgroundColor(mtype, c, c, c, c);
  }else{
    Color tl = retrieve( ps, "tl", Color());
    Color bl = retrieve( ps, "bl", Color());
    Color tr = retrieve( ps, "tr", Color());
    Color br = retrieve( ps, "br", Color());
    background = new BackgroundColor(mtype, bl, tl, tr, br);
  }
  return background; 
}
} // namespace rt3
