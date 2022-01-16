#include "Material.h"

#include <random>

Color Material::CalculateLuminance(const Color& E, const Vec3f& U, const Vec3f& V, const Vec3f& N) const {
  return kd->CalculateLuminance(E, U, V, N);
  // TODO добавить BRDF
}


Ray Material::TransformRay(const Ray& ray, const Vec3f& N, const Vec3f& intersectionPoint) const {
  auto event = chooseEvent(ray);
  if (event == nullptr)
    return Ray(); // FIXME нужен более умный способ показать, что луч поглотился
  return event->TransformRay(ray, N, intersectionPoint);
}


SurfaceOpticProperty* Material::chooseEvent(const Ray& ray) const {
  std::mt19937 gen;
  std::uniform_real_distribution<float> dist(0, 1);

  // TODO Добавить BRDF
  // Вероятность выбора события
  float kd_prob = kd->getColor().dot(ray.color) * kd->getCoeff();
  float ks_prob = ks->getColor().dot(ray.color) * ks->getCoeff();
  float ktd_prob = ktd->getColor().dot(ray.color) * ktd->getCoeff();
  float kts_prob = kts->getColor().dot(ray.color) * kts->getCoeff();

  // Нормализация вероятностей
  float prob_sum = kd_prob + ks_prob + ktd_prob + kts_prob;
  kd_prob /= prob_sum;
  ks_prob /= prob_sum;
  ktd_prob /= prob_sum;
  kts_prob /= prob_sum;

  // Выбор события
  float xi = dist(gen);
  if (xi < kd_prob)
    return kd;

  xi -= kd_prob;
  if (xi < ks_prob)
    return ks;

  xi -= ks_prob;
  if (xi < ktd_prob)
    return ktd;

  xi -= ktd_prob;
  if (xi < kts_prob)
    return kts;

  return nullptr;
}
