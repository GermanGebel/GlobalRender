#include "BRDF.h"

#include <random>

BRDF::BRDF(const Color& color, float kr, float e)
: color(color)
, kr(kr)
, e(e)
{
  float integral = 2 * M_PI / (e + 1); // cos(theta) ^ e * sin(theta) * dtheta * dphi (phi - (0, 360), theta - (0, 90))
  c = kr / integral; // коэффициент нормализации
}

Color BRDF::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  float cosTheta = -U * N; // косинус угла между нормалью и направлением наблюдения
  return (color * E) * c * kr * powf(cosTheta, e)*(1/M_PI); // (25.2) Ray Tracing from Ground Up
}

Ray BRDF::TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const {
  static std::mt19937 gen;
  static std::uniform_real_distribution<float> dist(0, 1);

  // Направление зеркального отражения
  Vec3f specularDirection = ray.direction - N * 2 * (ray.direction * N);
  specularDirection = specularDirection.normalize();

  // Оси новой системы координат
  Vec3f Z = specularDirection;
  Vec3f X = cross(Z, N).normalize();
  Vec3f Y = cross(Z, X).normalize();

  // генерируем луч
  float phi = 2 * M_PI * dist(gen);
  float cosTheta = powf(1 - dist(gen), 1 + e);
  float sinTheta = sqrtf(1 - cosTheta * cosTheta);

  Vec3f direction(sinTheta * cosf(phi), sinTheta * sinf(phi), cosTheta);
  direction = direction.normalize();

  // Переходим в глобальную систему координат
  float x = direction.x * X.x + direction.y * Y.x + direction.z * Z.x;
  float y = direction.y * X.y + direction.y * Y.y + direction.y * Z.y;
  float z = direction.z * X.z + direction.z * Y.z + direction.z * Z.z;

  direction = Vec3f(x, y, z).normalize();

  Ray transformedRay;
  transformedRay.direction = direction;
  transformedRay.origin = intersectionPoint;
  transformedRay.trash.lastEvent = TransformRayEvent::e_BRDF;
  transformedRay.color = ray.color * color;

  if (direction * N < 0)
    transformedRay.trash.lastEvent = TransformRayEvent::e_KILL;

  return transformedRay;
}


const Color& BRDF::getColor() const {
  return color;
}


float BRDF::getKr() const {
  return kr;
}