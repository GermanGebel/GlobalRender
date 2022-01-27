#pragma once

#include "Color.h"
#include "Math.h"
#include "Ray.h"

class SurfaceOpticProperty {
public:
  // U - вектор наблюдения, V - вектор освещения
  virtual Color CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const = 0;
  virtual Ray TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const = 0;
};

class Kd : public SurfaceOpticProperty {
public:
  Kd(const Color& color, float coeff);

  Color CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const override;
  Ray TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const override;

  const Color& getColor() const;
  float getCoeff() const;

private:
  Color color;
  float coeff;
};

class Ks : public SurfaceOpticProperty {
public:
  Ks(const Color& color, float coeff);

  Color CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const override;
  Ray TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const override;

  const Color& getColor() const;
  float getCoeff() const;

private:
  Color color;
  float coeff;
};

class Ktd : public SurfaceOpticProperty {
public:
  Ktd(const Color& color, float coeff);

  Color CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const override;
  Ray TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const override;

  const Color& getColor() const;
  float getCoeff() const;

private:
  Color color;
  float coeff;
};

class Kts : public SurfaceOpticProperty {
public:
  Kts(const Color& color, float coeff);

  Color CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const override;
  Ray TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const override;

  const Color& getColor() const;
  float getCoeff() const;

private:
  Color color;
  float coeff;
};
