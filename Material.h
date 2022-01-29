#pragma once

#include "SurfaceOpticProperties.h"
#include "BRDF.h"

class Material {
public:
  Material() = default;
  Material(Color& color, float kdCoeff,
                     float ksCoeff, float ktdCoeff,
                     float ktsCoeff, float brdfCoeff);

  ~Material();

public:
  Color CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const;
  Ray TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const;

private:
  SurfaceOpticProperty* chooseEvent(const Ray& ray) const;

public:
  Kd *kd;
  Ks *ks;
  Ktd *ktd;
  Kts *kts;
  BRDF *brdf;
};