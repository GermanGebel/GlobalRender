#include "SurfaceOpticProperties.h"

#include <random>

Color Kd::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return (E * color) * coeff / M_PI;
}


Ray Kd::TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const {
  std::mt19937 gen;
  std::uniform_real_distribution<float> dist(0, 1);

  float R = 1;
  float phi = 2 * M_PI * dist(gen);
  float z = -R + 2 * R * dist(gen);
  float theta = acosf(z / R);

  float x = sinf(theta) * cosf(phi) * R;
  float y = sinf(theta) * sinf(phi) * R;

  Vec3f direction(x, y, z);
  direction = direction + N;
  direction = direction.normalize();

  Ray transformedRay;

  transformedRay.direction = direction;
  transformedRay.origin = intersectionPoint;
  transformedRay.color = color * ray.color * coeff;

  return transformedRay;
}


const Color& Kd::getColor() const {
  return color;
}


float Kd::getCoeff() const {
  return coeff;
}


Color Ks::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
    return Color();
}


Ray Ks::TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const {
    Vec3f direction = ray.direction;
    Vec3f specularDirection = direction - N * 2 * (direction * N);
    specularDirection = specularDirection.normalize();

    Ray transformedRay;

    transformedRay.direction = specularDirection;
    transformedRay.origin = intersectionPoint;
    transformedRay.color = color * ray.color * coeff;

    return transformedRay;
}

const Color& Ks::getColor() const {
  return color;
}


float Ks::getCoeff() const {
  return coeff;
}


Color Ktd::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return Color();
}


Ray Ktd::TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const {
  std::mt19937 gen;
  std::uniform_real_distribution<float> dist(0, 1);

  float R = 1;
  float phi = 2 * M_PI * dist(gen);
  float z = -R + 2 * R * dist(gen);
  float theta = acosf(z / R);

  float x = sinf(theta) * cosf(phi) * R;
  float y = sinf(theta) * sinf(phi) * R;

  Vec3f direction(x, y, z);
  direction = direction - N;
  direction = direction.normalize();

  Ray transformedRay;

  transformedRay.direction = direction;
  transformedRay.origin = intersectionPoint;
  transformedRay.color = color * ray.color * coeff;

  return transformedRay;
}


const Color& Ktd::getColor() const {
  return color;
}


float Ktd::getCoeff() const {
  return coeff;
}


Color Kts::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return Color();
}


Ray Kts::TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const {
  return Ray();
}


const Color& Kts::getColor() const {
  return color;
}


float Kts::getCoeff() const {
  return coeff;
}
