#pragma once

#include <vector>
#include <memory>
#include <string>
#include "structs.h"
#include "Light.h"
#include "Camera.h"
#include "Geometry.h"

class Scene {
public:
  void readSceneFromFiles(const std::string& geometryFile,
                          const std::string& materialsFile,
                          const std::string& lightsFile,
                          const std::string& cameraFile);

private:

  void readGeometry(const std::string& fileName, const std::string& materialsFileName);
  std::vector<std::shared_ptr<Material>> readMaterials(const std::string& fileName);
  void readLights(const std::string& fileName);
  void readCamera(const std::string& fileName);

  //std::vector<Triangle> triangles_; // class Mesh : public Geometry  std::vector<Triangle> triangles_

  std::vector<Geometry> geometry_;
  std::vector<std::unique_ptr<Light>> lights_; // seems OK..?
  std::unique_ptr<Camera> camera_ = nullptr; //

};
