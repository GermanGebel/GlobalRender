#include "Scene.h"
#include "Math.h"
#include "Color.h"

#include <fstream>
#include <sstream>
#include <limits>
#include <iostream>
#include <chrono>
#include <unordered_map>

enum GeometryParseState {
    INITIAL,
    VERTICES,
    TRIANGLES
};

void Scene::readSceneFromFiles(const std::string& geometryFile,
    const std::string& materialsFile,
    const std::string& lightsFile,
    const std::string& cameraFile) {
    readGeometry(geometryFile);
    readMaterials(materialsFile);
    readLights(lightsFile);
    readCamera(cameraFile);
}

  Scene::~Scene()
  {
    for (Geometry * geom : geometry_) {
      delete geom;
    }

    for (Light * light : lights_) {
      delete light;
    }

    for (Material * material : materials_) {
      delete material;
    }

    delete camera_;
  }

void Scene::readGeometry(const std::string& fileName) {
  std::ifstream in(fileName);

  std::string line;
  GeometryParseState state = GeometryParseState::INITIAL;
  std::vector<Vec3f> vertices;
  std::vector<Triangle> triangles;
  Mesh * mesh = nullptr;

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
          mesh = new Mesh();
          mesh->ownPoints_ = vertices;
          break;
        }
        std::istringstream iss(line);
        double x, y, z;
        iss >> x >> z >> y; // FIXME: Это странно
        vertices.emplace_back(x, y, z);
        break;
      }
      case GeometryParseState::TRIANGLES: {
        if (line.find("parts") != std::string::npos) {
          state = GeometryParseState::INITIAL;
          mesh->triangles_ = std::move(triangles);
          geometry_.emplace_back(mesh);
          vertices.clear();
          triangles.clear();
          break;
        }
        std::istringstream iss(line);
        int v1, v2, v3;
        iss >> v1 >> v2 >> v3;

        triangles.emplace_back(mesh, v1, v2, v3);
        break;
      }
    }
  }

  in.close();
}

void Scene::readMaterials(const std::string& fileName) {
  std::ifstream in(fileName);

  std::string line;
  int currentObjectId = -1;
  float kd, ks, ktd, kts, brdf;

  std::vector<float> colors;
  std::vector<int> waveLengths;

  while (getline(in, line)) {
    if (line.find("id") != std::string::npos) {
      if (currentObjectId != -1) {
        Color color;
        color.setWaveLengths(waveLengths);
        color.setColors(colors);

        materials_.push_back(new Material(color, kd, ks, ktd, kts, brdf));

        waveLengths.clear();
        colors.clear();
      }
      ++currentObjectId;

      getline(in, line);
      std::istringstream iss(line);
      iss >> kd >> ks >> ktd >> kts >> brdf;
      continue;
    }

    if (line == "") {
      continue;
    }

    std::istringstream iss(line);
    int waveLength;
    double col;
    iss >> waveLength >> col;
    waveLengths.push_back(waveLength);
    colors.push_back(col);
  }

  in.close();
}

void Scene::readLights(const std::string& fileName) {
  std::ifstream in(fileName);
  Vec3f origin;
  Vec3f normal;
  std::vector<float> intensityTable;
  std::unordered_map<int, double> kds;
  in >> origin.x >> origin.y >> origin.z;
  in >> normal.x >> normal.y >> normal.z;

  std::string line;
  getline(in, line); // '\n'
  getline(in, line);
  std::istringstream iss(line);
  std::string element;
  while(std::getline(iss, element, ' ')) {
    intensityTable.push_back(std::stof(element));
  }

  Color color;
  std::vector<float> colors;


  while (getline(in, line)) {
    std::istringstream iss(line);
    int waveLength;
    double kd;
    iss >> waveLength >> kd;
    colors.push_back(kd);
    color.waveLengths.push_back(waveLength);
  }

  color.setColors(std::move(colors));

  Light * light = new PointLight(color, intensityTable, origin, normal);
  lights_.emplace_back(light);
  in.close();
}

void Scene::readCamera(const std::string& fileName) {
  std::ifstream in(fileName);
  Vec3f origin, target;
  int width, height;
  float fov;
  in >> origin.x >> origin.y >> origin.z;
  in >> target.x >> target.y >> target.z;
  in >> width >> height;
  in >> fov;
  Color color;
  color.waveLengths = wave_lengths_;
  color.setColors(std::vector<float>(wave_lengths_.size(), 1.0f/wave_lengths_.size()));
  camera_ = new Camera(origin, target, fov, width, height, color);
  in.close();
}
