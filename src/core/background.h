#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "paramset.h"
#include "rt3.h"

namespace rt3 {
// BackgroundSphereImage, BackgroundSkyBoxImage.
/*!
 * A background is basically a rectangle, have a color associated to each
 * corner. A background might be sampled based on a normalized coordinate in
 * \f$[0,1]^2.\f$. We generate samples with bilinear interpolation of the corner
 * colors. We consider the corners in a circular clockwise order, starting at
 * the lower-left corner.
 *
 * Alternatively, a background might be a regular image.
 */
class Background {
public:
  mapping_t mapping_type; //!< sampling type we use to pick a color.

  /// Ctro receiving a single color for the entire background.
  Background(mapping_t mt = mapping_t::spherical)
      : mapping_type{mt} { /* empty */
  }

  virtual ~Background(){/* empty */};
  virtual Color sampleXYZ(const Point2f &pixel_ndc) const { return Color(); }
};

class BackgroundColor : public Background {
private:
  /// Each corner has a color associated with.
  Color corners[4] = {{{0, 0, 0}}, {{0, 0, 0}}, {{0, 0, 0}}, {{0, 0, 0}}};
  /// Corner indices.
  enum Corners_e {
    bl = 0, //!< Bottom left corner.
    tl,     //!< Top left corner.
    tr,     //!< Top right corner.
    br      //!< Bottom right corner.
  };

public:
  /// Ctro receives a list of four colors, for each corner.
  BackgroundColor(mapping_t mt, Color bl, Color tl, Color tr, Color br) : Background(mt){
    corners[Corners_e::bl] = bl;
    corners[Corners_e::tl] = tl;
    corners[Corners_e::tr] = tr;
    corners[Corners_e::br] = br;
  }

  virtual ~BackgroundColor(){};

  Color sampleXYZ(const Point2f &pixel_ndc) const;
};

// factory pattern functions.
BackgroundColor *create_color_background(const ParamSet &ps);



} // namespace rt3
#endif
