#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "rtweekend.h"
#include "vec3.h"

class camera {
public:
  camera() {
    double aspect = 16.0 / 9.0;
    double vp_height = 2.0;
    double vp_width = vp_height * aspect;
    double focal_length = 1.0;

    origin = vec3(0, 0, 0);
    horizontal = vec3(vp_width, 0, 0);
    vertical = vec3(0, vp_height, 0);
    lower_left =
        origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
  }

  camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect,
         double aperture, double focus_dist) { // vertical FOV in degrees
    double theta = deg_to_rad(vfov);
    double vp_height = 2.0 * tan(theta / 2);

    double vp_width = vp_height * aspect;

    // these three vectors describe the orientation of the camera
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    origin = lookfrom;
    horizontal =
        vp_width * u; // multiplying by u gets the correct horizontal vector
    vertical = vp_height * v;

    // change the horizontal and vertical with consideration of focus distance
    // change the lower left based on where the focus dist is
    horizontal *= focus_dist;
    vertical *= focus_dist;
    lower_left = origin - w * focus_dist - horizontal / 2 - vertical / 2;

    lens_radius = aperture / 2;
  }

  ray get_ray_blur(double s, double t) const {
    // random point on lens radius
    vec3 rd = lens_radius * random_in_unit_disk();
    // orient lens radius on uv plane.
    vec3 offset = u * rd.x() + v * rd.y();

    return {.origin = origin + offset,
            .direction =
                lower_left + horizontal * s + vertical * t - (origin + offset)};
  }

public:
  vec3 origin;
  vec3 horizontal;
  vec3 vertical;
  vec3 lower_left;
  vec3 u, v, w;
  double lens_radius;
};

#endif
