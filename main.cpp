#include "Camera.h"
#include "Scene.h"
#include "Render.h"
#include <vector>

int main() {
  Color::setWaveLengths(std::vector<int>{400,500, 600, 700});

  Scene scene;
  std::string geometryFile = "../cornel_box0/cornel_box0.shp";
  std::string materialsFile = "../cornel_box0/materials.txt";
  std::string cameraFile = "../cornel_box0/camera.txt";
  std::string lightsFile = "../cornel_box0/lights.txt";
  scene.readMaterials(materialsFile);
  scene.readGeometry(geometryFile);
  scene.readCamera(cameraFile);
  scene.readLights(lightsFile);

  Render render(&scene);
  render.renderZBuffer("output.txt");
}