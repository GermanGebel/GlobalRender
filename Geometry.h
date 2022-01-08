#pragma once

#include "structs.h"
#include <vector>

class Mesh;

struct Triangle {
  Triangle(Mesh* mesh, int v1, int v2, int v3);

  bool hitTest(const Ray& ray, double& t) const;
  vec3 getNormal(const vec3& direction) const;
  bool isInside(const vec3& point) const;

  Mesh* mesh;
  int v1, v2, v3;
};

class Geometry {
public:
  virtual vec3 randomSurfPoint() const = 0;
  virtual vec3 getNormal(const vec3& intersectionPoint, const vec3& direction) const = 0;
  virtual bool hitTest(const Ray& ray, double& t) const = 0;

public:
  int materialId_;
  // TODO sourceLight ?
};

class Mesh : public Geometry {
public:
  vec3 randomSurfPoint() const override;
  vec3 getNormal(const vec3& intersectionPoint, const vec3& direction) const override;
  bool hitTest(const Ray& ray, double& t) const override;

public:
  std::vector<Triangle> triangles_;
  std::vector<vec3> ownPoints_;
};

class Sphere : public Geometry {
public:
  Sphere() {}
  Sphere(const vec3& center, double radius);

  vec3 randomSurfPoint() const override;
  vec3 getNormal(const vec3& intersectionPoint, const vec3& direction) const override;
  bool hitTest(const Ray& ray, double& t) const override;

public:
  vec3 center_;
  double radius_ = 0;
};
