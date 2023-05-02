#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <memory>
#include <random>
#include <thread>

// usings
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// utility functions
inline double deg_to_rad(double degrees) { return degrees * pi / 180.0; }

// random doubles
inline double random_double() {
  // thread_local std::mt19937 generator;
  // static std::random_device rd{};
  // generator.seed(rd());
  // std::uniform_real_distribution<double> distribution(0.0, 0.9999999999);
  // // Returns a random real in [0,1).
  // thread_local std::mt19937 generator(std::random_device{}());
  // static std::uniform_real_distribution<double> distribution(0.0,
  // 0.9999999999); return distribution(generator);
  //
  return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
  thread_local std::mt19937 generator;
  //   static std::random_device rd{};
  //   generator.seed(rd());
  //   std::uniform_real_distribution<double> distribution(min, max);
  //   // Returns a random real in [0,1).
  //   return distribution(generator);
  return min + (max - min) * random_double();
}

// clamp a number within a range.
inline double clamp(double x, double min, double max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

// common headers
#include "vec3.h"

#endif
