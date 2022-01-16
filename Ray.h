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

  Ray(const vec3& origin, const vec3& direction, float ks = 1)
  : origin(origin), direction(direction), ks(ks), luminance(SpectralValues(0)) {}

  void print() const {
    origin.print();
    direction.print();
    std::cout << "ks = " << ks << std::endl;
  }
};

#endif //RAYTRACING_RAY_H
