#include "Scene.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <iostream>
#include <chrono>
#include "Math.h"
#include "Color.h"


enum GeometryParseState {
    INITIAL,
    VERTICES,
    TRIANGLES
};

void Scene::readSceneFromFiles(const std::string& geometryFile,
    const std::string& materialsFile,
    const std::string& lightsFile,
    const std::string& cameraFile) {
    readGeometry(geometryFile, materialsFile);
    readLights(lightsFile);
    readCamera(cameraFile);
}


void Scene::readGeometry(const std::string& fileName, const std::string& materialsFileName) {
  std::ifstream in(fileName);

  std::vector<std::shared_ptr<Material>> materials = readMaterials(materialsFileName);

  std::string line;
  GeometryParseState state = GeometryParseState::INITIAL;
  int currentObjectId = 0;
  std::vector<Vec3f> vertices;
  std::vector<Geometry> geometries;

  while (getline(in, line)) {
    switch (state) {
      case GeometryParseState::INITIAL: {
        if (line.find("vertices") != std::string::npos) {
          state = GeometryParseState::VERTICES;
        }
        break;
      }
      case GeometryParseState::VERTICES: {
        if (line.find("triangles") != std::string::npos) {
          state = GeometryParseState::TRIANGLES;
          break;
        }
        std::istringstream iss(line);
        double x, y, z;
        iss >> x >> z >> y;
        vertices.push_back(Vec3f(x, y, z));
        break;
      }
      case GeometryParseState::TRIANGLES: {
        if (line.find("parts") != std::string::npos) {
          state = GeometryParseState::INITIAL;
          vertices.clear();
          ++currentObjectId;
          break;
        }
        std::istringstream iss(line);
        int v1, v2, v3;
        iss >> v1 >> v2 >> v3;
        triangles_.push_back(Triangle(currentObjectId, vertices[v1], vertices[v2], vertices[v3], materials[currentObjectId]));
        break;
      }
    }
  }

  in.close();
}


std::vector<std::shared_ptr<Material>> Scene::readMaterials(const std::string& fileName) {
  std::ifstream in(fileName);

  std::vector<std::shared_ptr<Material>> materials;
  std::string line;
  int currentObjectId = -1;
  double kd, ks;
  std::unordered_map<int, double> color;
  while (getline(in, line)) {
    if (line.find("id") != std::string::npos) {
      if (currentObjectId != -1) {
        materials.push_back(std::make_shared<Material>(color, kd, ks));
        color.clear();
      }
      ++currentObjectId;

      getline(in, line);
      std::istringstream iss(line);
      iss >> kd >> ks;
      continue;
    }
    std::istringstream iss(line);
    int waveLength;
    double col;
    iss >> waveLength >> col;
    color[waveLength] = col;
  }

  in.close();

  return materials;
}


void Scene::readLights(const std::string& fileName) {
  std::ifstream in(fileName);
  vec3 origin;
  //double intensity;
  std::vector<float> intensityTable;
  Color color;
  std::unordered_map<int, double> kds;
  in >> origin.x >> origin.y >> origin.z;
  in >> intensity;
  std::string line;
  getline(in, line); // '\n'
  while (getline(in, line)) {
    std::istringstream iss(line);
    int waveLength;
    double kd;
    iss >> waveLength >> kd;
    kds[waveLength] = kd;
  }
  lights_.push_back(std::make_unique<PointLight>(origin, intensity, kds));
  in.close();
}


void Scene::readCamera(const std::string& fileName) {
  std::ifstream in(fileName);
  Vec3f origin, target;
  int width, height;
  std::unordered_map<int, double> color;
  in >> origin.x >> origin.y >> origin.z;
  in >> target.x >> target.y >> target.z;
  in >> width >> height;
  const float fov = 60;
  camera_ = std::make_unique<Camera>(origin, target, fov, width, height);
  in.close();
}
