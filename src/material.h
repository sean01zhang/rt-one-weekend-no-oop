#ifndef MATERIAL_H
#define MATERIAL_H

#include "hit_record.h"
#include "ray.h"
#include "rtweekend.h"
#include "vec3.h"

struct hit_record;

enum material_type { LAMBERTIAN, METAL, DIELECTRIC };

struct material {
  material_type type;
  color albedo;
  double fuzz;
  double ir;
};

inline material make_lambertian(color &);
inline material make_metal(color &, double);
inline material make_dielectric(double);

double reflectance(double, double);
bool scatter(const material *, const ray &, const hit_record &, color &, ray &);

#endif
