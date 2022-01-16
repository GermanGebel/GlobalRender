#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H
#include <iostream>

struct Container {

};

struct Ray {
  vec3 origin;
  vec3 direction;
  double ks;
  SpectralValues luminance;
  Container trash;

  Ray() {}

  Ray(const vec3& origin, const vec3& direction, double ks = 1)
  : origin(origin), direction(direction), ks(ks), luminance(SpectralValues(0)) {}

  void print() const {
    origin.print();
    direction.print();
    std::cout << "ks = " << ks << std::endl;
  }
};

#endif //RAYTRACING_RAY_H
