#include "Camera.h"

int main() {
// Удалено из-за нарушения ODR, необходимо восстанавливать по мере разработки, за пояснениями -> К Юле
//  Scene scene;
//
//  std::string geometryFile = "../cornel_box0/cornel_box0.shp";
//  std::string materialsFile = "../cornel_box0/materials.txt";
//  std::string lightsFile = "../cornel_box0/lights.txt";
//  std::string cameraFile = "../cornel_box0/camera.txt";
//
//  scene.readSceneFromFiles(geometryFile, materialsFile, lightsFile, cameraFile);
//  scene.render("output.txt");


  Vec3f eye(0, 0, 0);
  Vec3f center(0, 0, 11);
  float fov = 45;
  int width = 60, height = 60;
  Camera camera(eye, center, fov, width, height);
  auto a = camera.render();
  auto b = camera.renderMonteCarlo(895165);
  return 0;


  return 0;
}
