#pragma once

#include "SurfaceOpticProperties.h"

class Material {
public:
  SurfaceOpticProperty* chooseEvent(const Ray& ray) const;

private:
  Kd *kd;
  Ks *ks;
  Ktd *ktd;
  Kts *kts;
};
