#include "ray.h"

point3 ray_extend(const ray & r, double t) {
  return r.origin + t * r.direction;
}
