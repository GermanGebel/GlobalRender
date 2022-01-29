#include "Material.h"

#include <random>

Material::~Material()
{
  delete kd;
  delete ks;
  delete ktd;
  delete kts;
  delete brdf;
}

Material::Material(Color& color, float kdCoeff, float ksCoeff,
                   float ktdCoeff, float ktsCoeff, float brdfCoeff) {
  this->kd = new Kd(color, kdCoeff);
  this->ks = new Ks(color, ksCoeff);
  this->ktd = new Ktd(color, ktdCoeff);
  this->kts = new Kts(color, ktsCoeff);
  this->brdf = new BRDF(color, brdfCoeff, 1);
}

Color Material::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return (kd == nullptr ? Color() : kd->CalculateLuminance(E, U, V, N)) +
  (ks == nullptr ? Color() : ks->CalculateLuminance(E, U, V, N)) +
  (ktd == nullptr ? Color() : ktd->CalculateLuminance(E, U, V, N)) +
  (kts == nullptr ? Color() : kts->CalculateLuminance(E, U, V, N)) +
  (brdf == nullptr ? Color() : brdf->CalculateLuminance(E, U, V, N));
}


Ray Material::TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const {
  auto event = chooseEvent(ray);
  if (event == nullptr) {
    Ray transformed;
    transformed.trash.lastEvent = TransformRayEvent::e_KILL;
    return transformed;
  }
  return event->TransformRay(ray, N, intersectionPoint);
}


SurfaceOpticProperty* Material::chooseEvent(const Ray& ray) const {
  static std::mt19937 gen;
  static std::uniform_real_distribution<float> dist(0, 1);

  // Вероятность выбора события
  float kd_prob = (kd == nullptr ? 0 : kd->getColor().dot(ray.color) * kd->getCoeff());
  float ks_prob = (ks == nullptr ? 0 : ks->getColor().dot(ray.color) * ks->getCoeff());
  float ktd_prob = (ktd == nullptr ? 0 : ktd->getColor().dot(ray.color) * ktd->getCoeff());
  float kts_prob = (kts == nullptr ? 0 : kts->getColor().dot(ray.color) * kts->getCoeff());
  float brdf_prob = (brdf == nullptr ? 0 : brdf->getColor().dot(ray.color) * brdf->getKr());

  // Выбор события
  float xi = dist(gen);
  if (kd && xi < kd_prob)
    return kd;

  xi -= kd_prob;
  if (ks && xi < ks_prob)
    return ks;

  xi -= ks_prob;
  if (ktd && xi < ktd_prob)
    return ktd;

  xi -= ktd_prob;
  if (kts && xi < kts_prob)
    return kts;

  xi -= kts_prob;
  if (brdf && xi < brdf_prob)
    return brdf;

  return nullptr;
}