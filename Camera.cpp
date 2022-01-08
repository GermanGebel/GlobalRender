//
// Created by Alex- on 05.01.2022.
//

#include "Camera.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

Camera::Camera(const Vec3f eye, const Vec3f center, const float fov, const int width, const int height, const Vec3f up):
                eye(eye), center(center), fov(fov), WIDTH(width), HEIGHT(height), up(up) {
    /*
     * input: eye - место, где находится камера
     *        center - точка, которая будет находиться в центре кадра
     *        fov - (field of view) угол обзора камеры
     *        width, height - ширина и высота матрицы в пикселях
     *        up - вектор верха камеры, чтобы камеру можно было "наклонять" влево/вправо (ещё не сделал)
     *
     * конструктор расчитывает matrixOfRotation - матрицу поворота луча
     * */
    Vec3f direct = Vec3f(center.x - eye.x, center.y - eye.y, eye.z - center.z).normalize();  //center - eye;
    dist = float(WIDTH) / 2.0 * (1.0/tan(3.14/180.0 * float(fov)/2.0));
    std::cout << "dir: (" << direct.x << ", " << direct.y << ", " << -direct.z << ") dist" << dist << std::endl;
    if (direct.x == 0.0 && direct.y == 0.0){
        if (direct.z == 1.0) return ;
        if (direct.z == -1.0){
            matrixOfRotation[2].z = 1;
            return ;
        }
    }
    Vec3f dirToMatrix(0.0,0.0,-1);
    Vec3f v = cross(direct, dirToMatrix);
    float c = direct*dirToMatrix; //dot mul
    float s = v.norm(); //norm or length of vector
    Vec3f kmat[3] = {Vec3f{0, -v.z, v.y}, Vec3f{v.z, 0, -v.x}, Vec3f{-v.y, v.x, 0}};
    Vec3f kmatDotKmat[3] = {Vec3f{kmat[0].x * kmat[0].x + kmat[0].y * kmat[1].x + kmat[0].z * kmat[2].x,                 //kmatDotKmat = kmat * kmat
                                  kmat[0].x * kmat[0].y + kmat[0].y * kmat[1].y + kmat[0].z * kmat[2].y,
                                  kmat[0].x * kmat[0].z + kmat[0].y * kmat[1].z + kmat[0].z * kmat[2].z},
                            Vec3f{kmat[1].x * kmat[0].x + kmat[1].y * kmat[1].x + kmat[1].z * kmat[2].x,
                                  kmat[1].x * kmat[0].y + kmat[1].y * kmat[1].y + kmat[1].z * kmat[2].y,
                                  kmat[1].x * kmat[0].z + kmat[1].y * kmat[1].z + kmat[1].z * kmat[2].z},
                            Vec3f{kmat[2].x * kmat[0].x + kmat[2].y * kmat[1].x + kmat[2].z * kmat[2].x,
                                  kmat[2].x * kmat[0].y + kmat[2].y * kmat[1].y + kmat[2].z * kmat[2].y,
                                  kmat[2].x * kmat[0].z + kmat[2].y * kmat[1].z + kmat[2].z * kmat[2].z}
                            };
    matrixOfRotation[0] = Vec3f{float(1.0 + kmat[0].x+kmatDotKmat[0].x*((1.0-c)/(s*s))),
                                float(0.0 + kmat[0].y+kmatDotKmat[0].y*((1.0-c)/(s*s))),
                                float(0.0 + kmat[0].z+kmatDotKmat[0].z*((1.0-c)/(s*s)))};
    matrixOfRotation[1] = Vec3f{float(0.0 + kmat[1].x+kmatDotKmat[1].x*((1.0-c)/(s*s))),
                                float(1.0 + kmat[1].y+kmatDotKmat[1].y*((1.0-c)/(s*s))),
                                float(0.0 + kmat[1].z+kmatDotKmat[1].z*((1.0-c)/(s*s)))};
    matrixOfRotation[2] = Vec3f{float(0.0 + kmat[2].x+kmatDotKmat[2].x*((1.0-c)/(s*s))),
                                float(0.0 + kmat[2].y+kmatDotKmat[2].y*((1.0-c)/(s*s))),
                                float(1.0 + kmat[2].z+kmatDotKmat[2].z*((1.0-c)/(s*s)))};


}

Ray Camera::castRay(int X, int Y) {
    /*
     * input: X - точка матрицы по оси OX
     *        Y - точка матрицы по оси OY     *
     * output: Ray - луч, направленный в заданом направлении
     *
     * функция вычисляет направление луча по следующему алгоритму:
     * 1. распологается матрца на расстоянии dist на оси OZ
     * 2. выбирается точка на этой матрице
     * 3. полученный вектор (указывает в точку на матрице) поворачивается на матрицу поворота из конструктора
     * например: если width = 5, height = 4, то точка на матрице будет иметь координату (-2, 1.5, dist), а потом
     * она будет повернута по матрице
     * */

    if (X == -1){
        X = rand() % (HEIGHT-1) + 1;
        Y = rand() % (WIDTH-1) + 1;
    }

    float h = float(HEIGHT) / 2.0 - 0.5;
    float w = float(WIDTH) / 2.0 - 0.5;

    Ray newRay{float(-w + X), float(h - Y), dist};

    Vec3f vec1 = Vec3f(newRay.x * matrixOfRotation[0].x + newRay.y * matrixOfRotation[0].y + newRay.z * matrixOfRotation[0].z,
                       newRay.x * matrixOfRotation[1].x + newRay.y * matrixOfRotation[1].y + newRay.z * matrixOfRotation[1].z,
                       newRay.x * matrixOfRotation[2].x + newRay.y * matrixOfRotation[2].y + newRay.z * matrixOfRotation[2].z).normalize();

    std::cout << "vec (" << X << ", " << Y << "): " << vec1.x << " " << vec1.y << " " << vec1.z << std::endl;
    return Ray{vec1.x,vec1.y,vec1.z};
}

Ray** Camera::renderMonteCarlo(int N) {
    /*
     * input: N - количество лучей для рандомного испускания
     * output: matrix - матрица лучей (с их точками начала и направления)
     *
     * функция выпускает N случайны лучей из камеры
     * */

    Ray** matrix = new Ray*[HEIGHT];
    for (int i = 0; i < HEIGHT; i++){
        matrix[i] = new Ray[WIDTH];
    }

    for (int i = 0; i < N; i++){
        int Y = rand() % (HEIGHT-1);
        int X = rand() % (WIDTH-1);
        matrix[Y][X] = castRay(X, Y);
    }
    return matrix;
}


Ray** Camera::render() {

    /*
     * output: matrix - матрица лучей (с их точками начала и направления)
     *
     * функция по лучу в каждую точку камеры
     * */

    Ray** matrix = new Ray*[HEIGHT];
    for (int i = 0; i < HEIGHT; i++){
        matrix[i] = new Ray[WIDTH];
    }

    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            matrix[i][j] = castRay(j,i);
        }
    }
    return matrix;
}