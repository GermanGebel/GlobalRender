#pragma once

#include "structs.h"
#include "Light.h"
#include "Camera.h"
#include "Geometry.h"
#include "Material.h"

#include <vector>
#include <memory>
#include <string>


class Scene {
public:
  void readSceneFromFiles(const std::string& geometryFile,
                          const std::string& materialsFile,
                          const std::string& lightsFile,
                          const std::string& cameraFile);


  ~Scene();
public:
  std::vector<int> wave_lengths_;

public:

  void readGeometry(const std::string& fileName);
  void readMaterials(const std::string& fileName);
  void readLights(const std::string& fileName);
  void readCamera(const std::string& fileName);

  std::vector<Geometry *> geometry_;
  std::vector<Light *> lights_;
  std::vector<Material *> materials_;
  Camera * camera_ = nullptr; //

};
