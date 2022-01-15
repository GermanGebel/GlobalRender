#include "SurfaceOpticProperties.h"

float Kd::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return 0;
}


Ray Kd::TransformRay(const Ray& ray, const Vec3f& N) const {
  return Ray();
}


const Color& Kd::getColor() const {
  return color;
}


float Kd::getCoeff() const {
  return coeff;
}


float Ks::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return 0;
}


Ray Ks::TransformRay(const Ray& ray, const Vec3f& N) const {
  return Ray();
}


const Color& Ks::getColor() const {
  return color;
}


float Ks::getCoeff() const {
  return coeff;
}


float Ktd::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return 0;
}


Ray Ktd::TransformRay(const Ray& ray, const Vec3f& N) const {
  return Ray();
}


const Color& Ktd::getColor() const {
  return color;
}


float Ktd::getCoeff() const {
  return coeff;
}


float Kts::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return 0;
}


Ray Kts::TransformRay(const Ray& ray, const Vec3f& N) const {
  return Ray();
}


const Color& Kts::getColor() const {
  return color;
}


float Kts::getCoeff() const {
  return coeff;
}
