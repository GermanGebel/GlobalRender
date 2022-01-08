#include <random>
#include "Geometry.h"


Triangle::Triangle(Mesh* mesh, int v1, int v2, int v3)
    : mesh(mesh)
    , v1(v1)
    , v2(v2)
    , v3(v3)
{}


bool Triangle::hitTest(const Ray& ray, double& t) const {
  vec3 e1 = mesh->ownPoints_[v2] - mesh->ownPoints_[v1];
  vec3 e2 = mesh->ownPoints_[v3] - mesh->ownPoints_[v1];
  vec3 pvec = ray.direction.cross(e2);
  double det = e1.dot(pvec);

  if (det < 1e-8 && det > -1e-8) return false;

  double inv_det = 1 / det;
  vec3 tvec = ray.origin - mesh->ownPoints_[v1];
  double u = tvec.dot(pvec) * inv_det;
  if (u < 0 || u > 1) return false;

  vec3 qvec = tvec.cross(e1);
  double v = ray.direction.dot(qvec) * inv_det;
  if (v < 0 || v + u > 1) return false;

  double dist = e2.dot(qvec) * inv_det;
  if (dist < t && dist > 1e-8) {
    t = dist;
    return true;
  }
  return false;
}


vec3 Triangle::getNormal(const vec3& direction) const {
  vec3 N = (mesh->ownPoints_[v2] - mesh->ownPoints_[v1]).cross(mesh->ownPoints_[v3] - mesh->ownPoints_[v1]).normalize();
  return N.dot(direction) < 0 ? -N : N;
}


bool Triangle::isInside(const vec3& point) const {
  vec3 edge0 = mesh->ownPoints_[v2] - mesh->ownPoints_[v1];
  vec3 edge1 = mesh->ownPoints_[v3] - mesh->ownPoints_[v2];

  vec3 C0 = point - mesh->ownPoints_[v1];
  vec3 C1 = point - mesh->ownPoints_[v2];
  vec3 C2 = point - mesh->ownPoints_[v3];

  double S = edge0.cross(edge1).length() / 2;

  double S1 = C0.cross(C1).length() / 2;
  double S2 = C1.cross(C2).length() / 2;
  double S3 = C2.cross(C0).length() / 2;

  return fabs(S - (S1 + S2 + S3)) < 1e-6;
}


vec3 Mesh::getNormal(const vec3& intersectionPoint, const vec3& direction) const {
  for (const auto& triangle : triangles_) {
    if (triangle.isInside(intersectionPoint))
      return triangle.getNormal(direction);
  }

  assert(false);
}


bool Mesh::hitTest(const Ray& ray, double& t) const {
  double minT = std::numeric_limits<double>::max();
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


vec3 Mesh::randomSurfPoint() const {
  std::mt19937 gen;
  std::uniform_int_distribution<int> triangle_dist(0, triangles_.size() - 1);
  std::uniform_real_distribution<double> dist(0, 1);

  Triangle triangle = triangles_[triangle_dist(gen)];

  vec3 v1 = ownPoints_[triangle.v1];
  vec3 v2 = ownPoints_[triangle.v2];
  vec3 v3 = ownPoints_[triangle.v3];

  double xi1 = dist(gen);
  double xi2 = dist(gen);
  if (xi1 + xi2 > 1) {
    xi1 = 1 - xi1;
    xi2 = 1 - xi2;
  }

  return (v2 - v1) * xi1 + (v3 - v1) * xi2 + v1;
}


Sphere::Sphere(const vec3& center, double radius)
    : center_(center)
    , radius_(radius)
{}


vec3 Sphere::getNormal(const vec3& intersectionPoint, const vec3& direction) const {
  vec3 N = (intersectionPoint - center_) / radius_;
  return N.dot(direction) < 0 ? -N : N;
}


bool Sphere::hitTest(const Ray& ray, double& t) const {
  const vec3 oc = ray.origin - center_;
  const double a = ray.direction.dot(ray.direction);
  const double b = 2 * oc.dot(ray.direction);
  const double c = oc.dot(oc) - radius_ * radius_;
  double disc = b * b - 4 * a * c;

  if (disc < 1e-4) return false;

  disc = sqrt(disc);
  const double t0 = (-b - disc) / (2 * a);
  const double t1 = (-b + disc) / (2 * a);
  t = (t0 < t1) ? t0 : t1;

  return true;
}


vec3 Sphere::randomSurfPoint() const {
  std::mt19937 gen;
  std::uniform_real_distribution<double> dist(0, 1);

  double xi1 = dist(gen);
  double xi2 = dist(gen);

  double phi = 2 * M_PI * xi1;
  double z = -radius_ + 2 * radius_ * xi2;
  double theta = acos(z / radius_);
  double x = radius_ * sin(theta) * cos(phi);
  double y = radius_ * sin(theta) * sin(phi);

  return vec3(x, y, z);
}
