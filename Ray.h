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
  Color color;
  Container trash;

  Ray() {}

  Ray(const Vec3f& origin, const Vec3f& direction)
  : origin(origin), direction(direction), color(Color()) {}
};

#endif //RAYTRACING_RAY_H
