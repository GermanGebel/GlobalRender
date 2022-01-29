#include "Render.h"

#include <chrono>
#include <fstream>
#include <random>

Render::Render(Scene * scene_) : scene(scene_)
{

}

void Render::renderZBuffer(const std::string &outputFileName) {
  int width = scene->camera_->getWidth();
  int height = scene->camera_->getHeight();

  std::vector<std::vector<Color>> outLuminance(height, std::vector<Color>(width));

//  std::vector<std::vector<float>> zbuffer(height, std::vector<float>(width, 0.0);

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  std::vector<std::vector<Ray>> rays = scene->camera_->render();

  fluxes.resize(scene->lights_.size());
  fluxes[0] = scene->lights_[0]->getFlux();
  for (int i = 1; i < scene->lights_.size(); ++i) {
    fluxes[i] = fluxes[i - 1] + scene->lights_[i - 1]->getFlux();
  }

  totalFlux = fluxes.back() + scene->lights_.back()->getFlux();

#pragma omp parallel for shared(height, width, outLuminance) default(none)
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      Ray ray = rays[i][j];  // TODO проверить индексы

      int depth;
      for (depth = 1; depth <= MAX_RENDER_DEPTH; ++depth) {
        float t = std::numeric_limits<float>().max();
        Geometry* hittedGeometry = getIntersection(ray, t);

        if (hittedGeometry == nullptr) { // Нет пересечений
          break;
        }

        if (hittedGeometry->sourceLight_) { // Попали в источник света
          // Попали после отражения или преломления
          // TODO первый луч
          if (ray.trash.lastEvent == TransformRayEvent::e_KS || ray.trash.lastEvent == TransformRayEvent::e_KTS) {
            // TODO уточнить про параметр rayDir
            outLuminance[i][j] = outLuminance[i][j] + ((RectangleLight*)hittedGeometry->sourceLight_)->calculateLuminance(ray.direction);
          }

//          ray = hittedGeometry->sourceLight_->fireRay();
//          ++depth; // TODO надо ли учитывать источник света при делении яркости
//          continue;
        }

        Vec3f intersectionPoint = ray.origin + ray.direction * t;
        Vec3f N = hittedGeometry->getNormal(intersectionPoint, -ray.direction).normalize();
        Material *material = scene->materials_[hittedGeometry->materialId_];

        // Считаем освещенность
        Light *light = chooseLight();
        Vec3f lightPoint = light->getRandomPointOfSurf();
        // Проверяем есть ли препятствия
        Ray lightRay(lightPoint, intersectionPoint - lightPoint);
        // TODO подобрать eps в зависимости от размеров сцены
        t = (intersectionPoint - lightPoint).length() - 2e-4;
        Ray lightRayFake = lightRay;
        lightRayFake.origin = lightRayFake.origin + lightRayFake.direction * 1e-4;
        float illuminance = getIntersection(lightRayFake, t) == nullptr ? light->calculateIlluminance(intersectionPoint, N, lightPoint) : 0;

        // Считаем яркость
        Color E(illuminance); // TODO освещенность должна быть спектральной?
        outLuminance[i][j] = outLuminance[i][j] + material->CalculateLuminance(E, ray.direction, lightRay.direction, N);
        SurfaceOpticProperty *surfaceOpticProperty = material->chooseEvent(ray);
        if (surfaceOpticProperty == nullptr) { // луч поглотился
          break;
        }

        // TODO сместить начало луча
        ray = surfaceOpticProperty->TransformRay(ray, N, intersectionPoint);

        if (ray.trash.lastEvent == TransformRayEvent::e_KILL) {
          break;
        }
      }

      outLuminance[i][j] = outLuminance[i][j] / depth;
    }
  }

//#pragma omp parallel for shared(height, width, outLuminance) default(none)
//  for (int i = 1; i < height - 1; ++i) {
//    for (int j = 1; j < width - 1; ++j) {
//      if (outLuminance[i][j].compare(outLuminance[i - 1][j]) > 1 ||
//          outLuminance[i][j].compare(outLuminance[i][j - 1]) > 1 ||
//          outLuminance[i][j].compare(outLuminance[i + 1][j]) > 1 ||
//          outLuminance[i][j].compare(outLuminance[i][j + 1]) > 1) {
//        for (int iOffset = 0; iOffset < ANTIALIASING_FACTOR; ++iOffset) {
//          for (int jOffset = 0; jOffset < ANTIALIASING_FACTOR; ++jOffset) {
//            Ray ray = camera_->castRay(i, j, ANTIALIASING_FACTOR, iOffset,
//                                       jOffset);
//            outLuminance[i][j] += getLuminance(ray);
//          }
//        }
//        outLuminance[i][j] = outLuminance[i][j] /
//                             (ANTIALIASING_FACTOR * ANTIALIASING_FACTOR);
//      }
//    }
//  }

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Rendering time = " <<
            std::chrono::duration_cast<std::chrono::milliseconds>(
                    end - begin).count() / 1000.0 << "s" << std::endl;

  std::ofstream out(outputFileName);

  for (int waveLengthIter = 0; waveLengthIter < Color::waveLengths.size(); ++waveLengthIter) {
    out << "wave_length " << Color::waveLengths[waveLengthIter] << std::endl;
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        out << outLuminance[i][j][waveLengthIter] << " ";
      }
      out << std::endl;
    }
    out << std::endl;
  }

  out.close();
}


Geometry* Render::getIntersection(const Ray& ray, float& t) const {
  t = std::numeric_limits<float>().max();
  Geometry* hitted = nullptr;
  for (const auto& geometry : scene->geometry_) {
    if (geometry->hitTest(ray, t)) {
      hitted = geometry;
    }
  }

  return hitted;
}


Light* Render::chooseLight() const {
  static std::mt19937 gen;
  static std::uniform_real_distribution<float> dist(0, 1);

  float xi = dist(gen) * totalFlux;
  int l = 0, r = scene->lights_.size();
  while (r - l > 1) {
    int m = (l + r) / 2;
    if (fluxes[m] < xi) {
      l = m;
    } else {
      r = m;
    }
  }

  return scene->lights_[l];
}

//void Render::render(const std::string& outputFileName) {
//    int width = scene->camera_->width;
//    int height = scene->camera_->height;
//    std::vector<std::vector<SpectralValues>> outLuminance(height, std::vector<SpectralValues>(width, SpectralValues(0)));
//
//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//
//#pragma omp parallel for shared(height, width, outLuminance) default(none)
//    for (int i = 0; i < height; ++i) {
//        for (int j = 0; j < width; ++j) {
//            Ray ray = camera_->castRay(i, j);
//            outLuminance[i][j] = getLuminance(ray);
//        }
//    }
//
//#pragma omp parallel for shared(height, width, outLuminance) default(none)
//    for (int i = 1; i < height - 1; ++i) {
//        for (int j = 1; j < width - 1; ++j) {
//            if (outLuminance[i][j].compare(outLuminance[i - 1][j]) > 1 ||
//                outLuminance[i][j].compare(outLuminance[i][j - 1]) > 1 ||
//                outLuminance[i][j].compare(outLuminance[i + 1][j]) > 1 ||
//                outLuminance[i][j].compare(outLuminance[i][j + 1]) > 1) {
//                for (int iOffset = 0; iOffset < ANTIALIASING_FACTOR; ++iOffset) {
//                    for (int jOffset = 0; jOffset < ANTIALIASING_FACTOR; ++jOffset) {
//                        Ray ray = camera_->castRay(i, j, ANTIALIASING_FACTOR, iOffset, jOffset);
//                        outLuminance[i][j] += getLuminance(ray);
//                    }
//                }
//                outLuminance[i][j] = outLuminance[i][j] / (ANTIALIASING_FACTOR * ANTIALIASING_FACTOR);
//            }
//        }
//    }
//
//    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//    std::cout << "Rendering time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0 << "s" << std::endl;
//
//    std::ofstream out(outputFileName);
//
//    for (int waveLength : {400, 500, 600, 700}) {
//        out << "wave_length " << waveLength << std::endl;
//        for (int i = 0; i < height; ++i) {
//            for (int j = 0; j < width; ++j) {
//                out << outLuminance[i][j].values[waveLength] << " ";
//            }
//            out << std::endl;
//        }
//        out << std::endl;
//    }
//
//    out.close();
//}
