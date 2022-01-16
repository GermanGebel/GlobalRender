#pragma once

#include "SurfaceOpticProperties.h"

class Material {
public:
  Color CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const;
  Ray TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const;

private:
  SurfaceOpticProperty* chooseEvent(const Ray& ray) const;

private:
  Kd *kd;
  Ks *ks;
  Ktd *ktd;
  Kts *kts;
};
