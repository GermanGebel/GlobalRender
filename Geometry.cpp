#include <random>
#include "Geometry.h"


Triangle::Triangle(Mesh* mesh, int v1, int v2, int v3)
    : mesh(mesh)
    , v1(v1)
    , v2(v2)
    , v3(v3)
{}


bool Triangle::hitTest(const Ray& ray, float& t) const {
  Vec3f e1 = mesh->ownPoints_[v2] - mesh->ownPoints_[v1];
  Vec3f e2 = mesh->ownPoints_[v3] - mesh->ownPoints_[v1];
  Vec3f pvec = cross(ray.direction, e2);
  float det = e1 * pvec;

  if (det < 1e-8 && det > -1e-8) return false;

  float inv_det = 1 / det;
  Vec3f tvec = ray.origin - mesh->ownPoints_[v1];
  float u = (tvec * pvec) * inv_det;
  if (u < 0 || u > 1) return false;

  Vec3f qvec = cross(tvec, e1);
  float v = (ray.direction * qvec) * inv_det;
  if (v < 0 || v + u > 1) return false;

  float dist = (e2 * qvec) * inv_det;
  if (dist < t && dist > 1e-8) {
    t = dist;
    return true;
  }
  return false;
}


Vec3f Triangle::getNormal(const Vec3f& direction) const {
  Vec3f N = cross(mesh->ownPoints_[v2] - mesh->ownPoints_[v1], mesh->ownPoints_[v3] - mesh->ownPoints_[v1]).normalize();
  return N * direction < 0 ? -N : N;
}


bool Triangle::isInside(const Vec3f& point) const {
  Vec3f edge0 = mesh->ownPoints_[v2] - mesh->ownPoints_[v1];
  Vec3f edge1 = mesh->ownPoints_[v3] - mesh->ownPoints_[v1];

  Vec3f C0 = point - mesh->ownPoints_[v1];
  Vec3f C1 = point - mesh->ownPoints_[v2];
  Vec3f C2 = point - mesh->ownPoints_[v3];

  float S = cross(edge0, edge1).length() / 2;

  float S1 = cross(C0, C1).length() / 2;
  float S2 = cross(C1, C2).length() / 2;
  float S3 = cross(C2, C0).length() / 2;

  return fabs(S - (S1 + S2 + S3)) < 1e-6;
}


float Triangle::getArea() const {
  Vec3f edge0 = mesh->ownPoints_[v2] - mesh->ownPoints_[v1];
  Vec3f edge1 = mesh->ownPoints_[v3] - mesh->ownPoints_[v1];

  return cross(edge0, edge1).length() / 2;
}


Vec3f Mesh::getNormal(const Vec3f& intersectionPoint, const Vec3f& direction) const {
  for (const auto& triangle : triangles_) {
    if (triangle.isInside(intersectionPoint))
      return triangle.getNormal(direction);
  }

  assert(false);
}


bool Mesh::hitTest(const Ray& ray, float& t) const {
  float minT = std::numeric_limits<float>::max();
  bool result = false;

  for (const auto& triangle : triangles_) {
    if (triangle.hitTest(ray, t)) {
      minT = std::min(minT, t);
      result = true;
    }
  }

  if (result) {
    t = minT;
  }

  return result;
}


Vec3f Mesh::randomSurfPoint() const {
  std::mt19937 gen;
  std::uniform_int_distribution<int> triangle_dist(0, triangles_.size() - 1);
  std::uniform_real_distribution<float> dist(0, 1);

  Triangle triangle = triangles_[triangle_dist(gen)];

  Vec3f v1 = ownPoints_[triangle.v1];
  Vec3f v2 = ownPoints_[triangle.v2];
  Vec3f v3 = ownPoints_[triangle.v3];

  float xi1 = dist(gen);
  float xi2 = dist(gen);
  if (xi1 + xi2 > 1) {
    xi1 = 1 - xi1;
    xi2 = 1 - xi2;
  }

  return (v2 - v1) * xi1 + (v3 - v1) * xi2 + v1;
}


float Mesh::getSurfaceArea() const {
  float area = 0.0;
  for (const auto& triangle : triangles_) {
    area += triangle.getArea();
  }
  return area;
}


Sphere::Sphere(const Vec3f& center, float radius)
    : center_(center)
    , radius_(radius)
{}


Vec3f Sphere::getNormal(const Vec3f& intersectionPoint, const Vec3f& direction) const {
  Vec3f N = (intersectionPoint - center_).normalize();
  return N * direction < 0 ? -N : N;
}


bool Sphere::hitTest(const Ray& ray, float& t) const {
  const Vec3f oc = ray.origin - center_;
  const float a = ray.direction * ray.direction;
  const float b = 2 * (oc * ray.direction);
  const float c = oc * oc - radius_ * radius_;
  float disc = b * b - 4 * a * c;

  if (disc < 1e-4) return false;

  disc = sqrt(disc);
  const float t0 = (-b - disc) / (2 * a);
  const float t1 = (-b + disc) / (2 * a);
  t = (t0 < t1) ? t0 : t1;

  return true;
}


Vec3f Sphere::randomSurfPoint() const {
  std::mt19937 gen;
  std::uniform_real_distribution<float> dist(0, 1);

  float xi1 = dist(gen);
  float xi2 = dist(gen);

  float phi = 2 * M_PI * xi1;
  float z = -radius_ + 2 * radius_ * xi2;
  float theta = acos(z / radius_);
  float x = radius_ * sin(theta) * cos(phi);
  float y = radius_ * sin(theta) * sin(phi);

  return Vec3f(x, y, z);
}


float Sphere::getSurfaceArea() const {
  return 4 * M_PI * radius_ * radius_;
}
