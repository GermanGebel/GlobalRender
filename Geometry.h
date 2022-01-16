#pragma once

#include "Math.h"
#include "Ray.h"

#include <vector>

class Mesh;

struct Triangle {
  Triangle(Mesh* mesh, int v1, int v2, int v3);

  bool hitTest(const Ray& ray, float& t) const;
  Vec3f getNormal(const Vec3f& direction) const;
  bool isInside(const Vec3f& point) const;
  float getArea() const;

  Mesh* mesh;
  int v1, v2, v3;
};

class Geometry {
public:
  virtual Vec3f randomSurfPoint() const = 0;
  virtual Vec3f getNormal(const Vec3f& intersectionPoint, const Vec3f& direction) const = 0;
  virtual bool hitTest(const Ray& ray, float& t) const = 0;
  virtual float getSurfaceArea() const = 0;

public:
  int materialId_;
  // TODO sourceLight ?
};

class Mesh : public Geometry {
public:
  Vec3f randomSurfPoint() const override;
  Vec3f getNormal(const Vec3f& intersectionPoint, const Vec3f& direction) const override;
  bool hitTest(const Ray& ray, float& t) const override;
  float getSurfaceArea() const override;

public:
  std::vector<Triangle> triangles_;
  std::vector<Vec3f> ownPoints_;
};

class Sphere : public Geometry {
public:
  Sphere() {}
  Sphere(const Vec3f& center, float radius);

  Vec3f randomSurfPoint() const override;
  Vec3f getNormal(const Vec3f& intersectionPoint, const Vec3f& direction) const override;
  bool hitTest(const Ray& ray, float& t) const override;
  float getSurfaceArea() const override;

public:
  Vec3f center_;
  float radius_ = 0;
};
