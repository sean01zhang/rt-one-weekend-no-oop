#include "hittable.h"

hittable make_sphere(point3 pos, double radius, material mat) {
  return {.type = SPHERE,
          .pos = pos,
          .dimension = vec3(radius, radius, radius),
          .mat = mat};
}

// hittables check hit
bool hittable_hit(const hittable &h, const ray &r, double t_min, double t_max,
                  hit_record &rec) {
  switch (h.type) {
  case SPHERE:
    vec3 org_min_c = r.origin - h.pos;
    double a = dot(r.direction, r.direction);
    double h_b = dot(r.direction, org_min_c);
    double c = dot(org_min_c, org_min_c) - h.dimension[0] * h.dimension[0];
    double discriminant = h_b * h_b - a * c;
    if (discriminant < 0) {
      return false;
    } else {
      double sqrtd = std::sqrt(discriminant);
      double root = (-h_b - sqrtd) / a;
      if (t_min > root || t_max < root) {
        root = (-h_b + sqrtd) / a;
        if (t_min > root || t_max < root) {
          return false;
        }
      }
      rec.t = root;
      rec.point = ray_extend(r, rec.t);
      // rec.norm = unit_vector(rec.point - center); // this will always be
      // radius
      rec.set_face_normal(r, (rec.point - h.pos) / h.dimension[0]);
      rec.mat_ptr = &(h.mat);
      return true;
    }
    break;
  }
}

bool hittable_list_hit(const hittable *hittable_list, const size_t len,
                       const ray &r, double t_min, double t_max,
                       hit_record &rec) {
  // probably want to distinguish between objects in front or behind
  hit_record rec_temp;
  bool is_hit = false;
  double closest_hit = t_max;

  for (int i = 0; i < len; ++i) {
    if (hittable_hit(hittable_list[i], r, t_min, closest_hit, rec_temp)) {
      is_hit = true;
      closest_hit = rec_temp.t;
      rec = rec_temp;
    }
  }

  return is_hit;
}
