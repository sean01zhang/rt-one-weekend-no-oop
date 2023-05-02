#ifndef HITTABLE_H
#define HITTABLE_H

#include "material.h"
#include "hit_record.h"
#include "rtweekend.h"
#include "vec3.h"

enum hittable_type { SPHERE };

// hittable struct
struct hittable {
  hittable_type type;
  point3 pos;
  vec3 dimension;
  material mat;
};

hittable make_sphere(point3, double, material);
bool hittable_hit(const hittable &, const ray &, double, double, hit_record &);
bool hittable_list_hit(const hittable *, const size_t, const ray &, double,
                       double, hit_record &);


#endif
