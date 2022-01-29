#include "Camera.h"
#include "Scene.h"
#include "Render.h"

int main() {
  Color::setWaveLengths(400, 700, 100);

  Scene scene;
  std::string geometryFile = "../cornel_box0/cornel_box0.shp";
  std::string materialsFile = "../cornel_box0/materials.txt";
  std::string cameraFile = "../cornel_box0/camera.txt";
  std::string lightsFile = "../cornel_box0/lights.txt";
  scene.readGeometry(geometryFile);
  scene.readCamera(cameraFile);
  scene.readLights(lightsFile);
  scene.readMaterials(materialsFile);

  Render render(&scene);
  render.renderZBuffer("output.txt");
}
