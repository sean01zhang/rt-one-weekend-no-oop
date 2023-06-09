#include "vec3.h"

vec3 random_in_unit_sphere() {
  while (true) {
    auto p = vec3::random(-1, 1);
    if (p.length_squared() >= 1)
      continue;
    return p;
  }
}

vec3 random_unit_vector() { return unit_vector(random_in_unit_sphere()); }

vec3 random_in_hemisphere(const vec3 &normal) {
  vec3 in_unit_sphere = random_in_unit_sphere();
  if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
    return in_unit_sphere;
  else
    return -in_unit_sphere;
}

vec3 reflect(const vec3 &incident, const vec3 &norm) {
  return incident - 2 * dot(incident, norm) * norm;
}

vec3 refract(const vec3 &uv, const vec3 &n, double eta_i_over_eta_t) {
  auto cos_theta = fmin(dot(-uv, n), 1.0);
  vec3 r_out_perp = eta_i_over_eta_t * (uv + cos_theta * n);
  vec3 r_out_para = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_para;
}

vec3 random_in_unit_disk() {
  while (true) {
    auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
    if (p.length_squared() <= 1) return p;
  }
}
