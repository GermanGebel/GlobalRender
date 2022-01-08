#ifndef CAMERA_H
#define CAMERA_H

#include "Math.h"

//struct vec3D{
//    float x;
//    float y;
//    float z;
//    vec3D(float x, float y, float z): x(x), y(y), z(z){};
//};

using Ray = Vec3f;

class Camera {
public:
  Camera(Vec3f eye, Vec3f center, float fov,
           int width, int height,
           Vec3f up = Vec3f{0.0,1.0,0.0});
    ~Camera()=default;

    Ray castRay(int X = -1, int Y = -1);
    Ray** renderMonteCarlo(int N);
    Ray** render();

private:
  const Vec3f eye;
  const Vec3f center;
  const Vec3f up;
  const float fov;
  const int WIDTH;
  const int HEIGHT;

  float dist;
  Vec3f matrixOfRotation[3] = {Vec3f{1.0,0.0,0.0},Vec3f{0.0,1.0,0.0},Vec3f{0.0,0.0,-1.0}};
  Vec3f getNorm(Vec3f inp);
};


#endif //CAMERA_H
