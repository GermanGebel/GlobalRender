#ifndef CAMERA_H
#define CAMERA_H

struct vec3D{
    float x;
    float y;
    float z;
    vec3D(float x, float y, float z): x(x), y(y), z(z){};
};

struct Ray{
    float x;
    float y;
    float z;
};

class Camera {
    const vec3D eye;
    const vec3D center;
    const vec3D up;
    const float fov;
    const int WIDTH;
    const int HEIGHT;

    float dist;
    vec3D matrixOfRotation[3] = {vec3D{1.0,0.0,0.0},vec3D{0.0,1.0,0.0},vec3D{0.0,0.0,-1.0}};
    vec3D getNorm(vec3D inp);
public:
    Camera(const vec3D eye,
           const vec3D center,
           const float fov,
           const int width,
           const int height,
           const vec3D up = vec3D{0.0,1.0,0.0});
    ~Camera() {};


    Ray castRay(int X = -1, int Y = -1);
    Ray** renderMonteCarlo(int N);
    Ray** render();
};


#endif //CAMERA_H
