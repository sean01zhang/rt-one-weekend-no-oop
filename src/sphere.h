// #ifndef SPHERE_H
// #define SPHERE_H
//
// #include "hittable.h"
// #include "material.h"
// #include "rtweekend.h"
// #include "vec3.h"
//
// #include <cmath>
//
// class sphere : public hittable {
  // public:
  // sphere(point3 center, double r, material *m)
      // : center(center), radius(r), mat_ptr(m), type(SPHERE) {}
//
  // public:
  // point3 center;
  // double radius;
  // material * mat_ptr;
// }
//
//
// bool sphere::hit(const ray &r, double t_min, double t_max,
                 // hit_record &rec) const {
  // vec3 org_min_c = r.origin() - center;
  // double a = dot(r.direction(), r.direction());
  // double h_b = dot(r.direction(), org_min_c);
  // double c = dot(org_min_c, org_min_c) - radius * radius;
  // double discriminant = h_b * h_b - a * c;
  // if (discriminant < 0) {
    // return false;
  // } else {
    // double sqrtd = std::sqrt(discriminant);
    // double root = (-h_b - sqrtd) / a;
    // if (t_min > root || t_max < root) {
      // root = (-h_b + sqrtd) / a;
      // if (t_min > root || t_max < root) {
        // return false;
      // }
    // }
    // rec.t = root;
    // rec.point = r.at(rec.t);
    // // rec.norm = unit_vector(rec.point - center); // this will always be radius
    // rec.set_face_normal(r, (rec.point - center) / radius);
    // rec.mat_ptr = mat_ptr;
//
    // return true;
  // }
// }
//
// #endif
