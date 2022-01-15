#pragma once

#include "Color.h"
#include "Math.h"

struct Ray; // TODO заменить на новый луч

class SurfaceOpticProperty {
public:
  // U - вектор наблюдения, V - вектор освещения
  virtual float CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const = 0;
  virtual Ray TransformRay(const Ray& ray, const Vec3f& N) const = 0;
};

class Kd : public SurfaceOpticProperty {
public:
  float CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const override;
  Ray TransformRay(const Ray& ray, const Vec3f& N) const override;

private:
  Color color;
  float k;
};

class Ks : public SurfaceOpticProperty {
public:
  float CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const override;
  Ray TransformRay(const Ray& ray, const Vec3f& N) const override;

private:
  Color color;
  float k;
};

class Ktd : public SurfaceOpticProperty {
public:
  float CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const override;
  Ray TransformRay(const Ray& ray, const Vec3f& N) const override;

private:
  Color color;
  float k;
};

class Kts : public SurfaceOpticProperty {
public:
  float CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const override;
  Ray TransformRay(const Ray& ray, const Vec3f& N) const override;

private:
  Color color;
  float k;
};
