#include "material.h"

inline material make_lambertian(color & albedo) {
  return {.type=LAMBERTIAN, .albedo=albedo};
}

inline material make_metal(color & albedo, double fuzz) {
  return {.type=METAL, .albedo=albedo, .fuzz=fuzz};
}

inline material make_dielectric(double ir) {
  return {.type=DIELECTRIC, .ir=ir};
}

double reflectance(double cosine, double ref_indx) {
  // use schlick approximation for reflectance
  auto r0 = (1 - ref_indx) / (1 + ref_indx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool scatter(const material * mat, const ray &r_in, const hit_record &rec,
             color &attenuation, ray &scattered) {
  switch (mat->type) {
  case LAMBERTIAN: {

    vec3 scatter_direction = rec.norm + random_unit_vector();

    if (scatter_direction.near_zero()) {
      scatter_direction = rec.norm;
    }

    scattered = ray{.origin = rec.point, .direction = scatter_direction};
    attenuation = mat->albedo;
    return true;
  }
  case METAL: {
    vec3 reflection_direction = reflect(unit_vector(r_in.direction), rec.norm);

    attenuation = mat->albedo;
    scattered = ray{.origin = rec.point,
                    .direction =
                        reflection_direction + mat->fuzz * random_unit_vector()};
    return (dot(scattered.direction, rec.norm) > 0);
  }
  case DIELECTRIC: {
    // attenuation should always be 1 - because the sphere doesn't absorb
    // anything.
    attenuation = color(1, 1, 1);
    double refraction_ratio = rec.front_face ? (1.0 / mat->ir) : mat->ir;
    vec3 unit_dir = unit_vector(r_in.direction);

    double cos_theta = fmin(dot(-unit_dir, rec.norm), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    // if you cannot refract
    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3 direction;
    if (cannot_refract ||
        reflectance(cos_theta, refraction_ratio) > random_double()) {
      direction = reflect(unit_dir, rec.norm);
    } else {
      direction = refract(unit_dir, rec.norm, refraction_ratio);
    }

    scattered = ray{.origin = rec.point, .direction = direction};
    return true;
  }
  }
}

