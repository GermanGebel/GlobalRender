#include "SurfaceOpticProperties.h"

float Kd::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return 0;
}


Ray Kd::TransformRay(const Ray& ray, const Vec3f& N) const {
  return Ray();
}


float Ks::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return 0;
}


Ray Ks::TransformRay(const Ray& ray, const Vec3f& N) const {
  return Ray();
}


float Ktd::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return 0;
}


Ray Ktd::TransformRay(const Ray& ray, const Vec3f& N) const {
  return Ray();
}


float Kts::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return 0;
}


Ray Kts::TransformRay(const Ray& ray, const Vec3f& N) const {
  return Ray();
}
