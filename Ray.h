#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H
#include <iostream>

#include "Color.h"
#include "Math.h"

struct Container {

};

struct Ray {
  Vec3f origin;
  Vec3f direction;
  float ks;
  Color luminance;
  Container trash;

  Ray() {}

  Ray(const Vec3f& origin, const Vec3f& direction, float ks = 1)
  : origin(origin), direction(direction), ks(ks), luminance(Color()) {}
};

#endif //RAYTRACING_RAY_H
