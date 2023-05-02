#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"
#include "camera.h"
#include "color.h"


// give it a ray and an object and it'll tell you what to color it
color ray_color(const ray &r, const hittable * world, int world_length, int depth) {

  ray temp_ray = r;
  // base is blue sky portion
  double blue_portion = 0.5 * (unit_vector(r.direction).y() + 1.0);
  color final = blue_portion * color(0.5, 0.7, 1.0) +
         (1 - blue_portion) * color(1, 1, 1);

  for (int i = 0; i < depth; ++i) {
    hit_record rec;
    if (hittable_list_hit(world, world_length, temp_ray, 0.0001, infinity, rec)) {
      color attenuation;
      if (scatter(rec.mat_ptr, temp_ray, rec, attenuation, temp_ray)) {
        final = attenuation * final;
      } else {
        return color(0, 0, 0);
      }
    } else {
      // hits nothing, return sky.
      return final;
    }
  }

  return color(0, 0, 0);
}

std::vector<hittable> random_scene() {
  std::vector<hittable> world;

  // generate the ground first.
  material material_ground = {.type=LAMBERTIAN, .albedo=color(0.5, 0.5, 0.5)};
  world.push_back(make_sphere(point3(0, -1000, 0), 1000, material_ground));

  // generate all the random spheres
  for (int a = -11; a < 11; ++a) {
    for (int b = -11; b < 11; ++b) {
      double choose_mat = random_double();
      double radius = 0.2;
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        material sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = {.type=LAMBERTIAN, .albedo=albedo};
          world.push_back(make_sphere(center, radius, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = {.type=METAL, .albedo=albedo, .fuzz=fuzz};
          world.push_back(make_sphere(center, radius, sphere_material));
        } else {
          // glass
          sphere_material = {.type=DIELECTRIC, .ir=1.5};
          world.push_back(make_sphere(center, radius, sphere_material));
        }
      }
    }
  }

  material material1= {.type=DIELECTRIC, .ir=1.5};
  world.push_back(make_sphere(point3(0, 1, 0), 1.0, material1));

  // auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  material material2 = {.type=LAMBERTIAN, .albedo=color(0.4, 0.2, 0.1)};
  world.push_back(make_sphere(point3(-4, 1, 0), 1.0, material2));

  material material3 = {.type=METAL, .albedo=color(0.7, 0.6, 0.5), .fuzz=0};
  world.push_back(make_sphere(point3(4, 1, 0), 1.0, material3));

  return world;
}

std::vector<color> output;
std::mutex output_lock;

struct tracing_properties {
  double img_height;
  double img_width;
  hittable * world;
  size_t world_size;
  double max_depth;
  camera cam;
  int samples_per_px;
};

void trace_image(struct tracing_properties &prop) {
  srand((unsigned int)time(0));

  // does the computation
  std::vector<color> local_result{};
  for (int j = prop.img_height - 1; j >= 0; --j) {
    for (int i = 0; i < prop.img_width; ++i) {
      // do stuff
      color temp(0, 0, 0);
      for (int s = 0; s < prop.samples_per_px; ++s) {
        // only sample [0, 1) + pixel. If that pixel is half hit half not, then
        // it is blurred.
        double u = (i + random_double()) / (prop.img_width - 1);
        double v = (j + random_double()) / (prop.img_height - 1);
        temp +=
            ray_color(prop.cam.get_ray_blur(u, v), prop.world, prop.world_size, prop.max_depth);
      }
      local_result.push_back(temp);
    }
  }

  // writes to the mutex
  output_lock.lock();
  if (output.size() == 0) {
    output = local_result;
  } else {
    for (int i = 0; i < local_result.size(); ++i) {
      output[i] += local_result[i];
    }
  }
  output_lock.unlock();
}
int main() {
  // perf 
  auto start = std::chrono::steady_clock::now();

  // image
  const double aspect = 3.0 / 2.0;
  const double img_width = 800;
  const double img_height = static_cast<int>(img_width / aspect);

  // image quality settings
  const int samples_per_px = 30;
  const int max_depth = 50;

  // world 
  std::vector<hittable> world = random_scene();
  
  // camera
  point3 lookfrom(13, 2, 3);
  point3 lookat(0, 0, 0);
  vec3 world_up(0, 1, 0);

  // set the focus distance to where lookat is.
  double focus_distance = 10.0;
  double aperture = 0.1;

  camera cam(lookfrom, lookat, world_up, 20, aspect, aperture, focus_distance);

  // render
  tracing_properties props = {img_height, img_width, world.data(), world.size(),
                              max_depth,  cam,       samples_per_px};

  int num_threads = 8;
  int samples_per_px_global = samples_per_px * num_threads;

  std::vector<std::thread> threads;
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back(trace_image, std::ref(props));
  }

  for (int i = 0; i < num_threads; ++i) {
    threads[i].join();
  }

  std::cout << "P3\n" << img_width << " " << img_height << "\n255\n";

  output_lock.lock();
  for (auto &c : output) {
    write_color_gamma(std::cout, c, samples_per_px_global);
  }
  output_lock.unlock();

  auto finish = std::chrono::steady_clock::now();
  double elapsed_seconds = std::chrono::duration_cast<
    std::chrono::duration<double> >(finish - start).count();

  std::cerr << "Elapsed Time: " << elapsed_seconds << std::endl;


}
