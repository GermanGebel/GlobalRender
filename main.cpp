#include "Camera.h"
#include "Scene.h"

int main() {
  Scene scene;
  std::string geometryFile = "../cornel_box0/cornel_box0.shp";
  std::string materialsFile = "../cornel_box0/materials.txt";
  scene.readGeometry(geometryFile, materialsFile);


//  std::string lightsFile = "../cornel_box0/lights.txt";
//  std::string cameraFile = "../cornel_box0/camera.txt";
//
//  scene.readSceneFromFiles(geometryFile, materialsFile, lightsFile, cameraFile);
//  scene.render("output.txt");


//  Vec3f eye(0, 0, 0);
//  Vec3f center(0, 0, 11);
//  float fov = 45;
//  int width = 60, height = 60;
//  Camera camera(eye, center, fov, width, height);
//  auto a = camera.render();
//  return 0;
}
