#ifndef COLOR_H
#define COLOR_H

#include "rtweekend.h"

#include <iostream>

void write_color(std::ostream &out, color pixel_color) {
  //
  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(255.999 * pixel_color.x()) << ' '
      << static_cast<int>(255.999 * pixel_color.y()) << ' '
      << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

void write_color_aa(std::ostream &out, color pixel_color,
                    int samples_per_pixel) {
  // get pixel color.
  double r = pixel_color.x();
  double g = pixel_color.y();
  double b = pixel_color.z();

  // divide by samples per px
  r /= samples_per_pixel;
  g /= samples_per_pixel;
  b /= samples_per_pixel;

  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(256 * clamp(r, 0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(g, 0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(b, 0, 0.999)) << '\n';
}

void write_color_gamma(std::ostream &out, color pixel_color,
                       int samples_per_pixel) {
  // get pixel color.
  double r = pixel_color.x();
  double g = pixel_color.y();
  double b = pixel_color.z();

  // divide by samples per px, then apply gamma correction
  // by raising the value to 0.5 (aka a sqrt)
  double scale = 1.0 / samples_per_pixel;
  r = sqrt(scale * r);
  g = sqrt(scale * g);
  b = sqrt(scale * b);

  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(256 * clamp(r, 0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(g, 0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(b, 0, 0.999)) << '\n';
}

#endif
