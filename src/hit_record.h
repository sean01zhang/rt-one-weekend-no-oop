#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "rtweekend.h"
#include "ray.h"
#include "material.h"

struct material;

struct hit_record {
  point3 point;
  vec3 norm;
  const material *mat_ptr;
  double t;
  bool front_face;

  inline void set_face_normal(const ray &r, const vec3 &out_norm) {
    front_face = dot(r.direction, out_norm) < 0;
    norm = front_face ? out_norm : -out_norm;
  }
};

#endif
