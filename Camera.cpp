//
// Created by Alex- on 05.01.2022.
//

#include "Camera.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

vec3D Camera::getNorm(vec3D inp) {
    /*
     * input: inp - вектор для нормализации
     * output: vec3D - отнормалиованный вектор inp
     *
     * функция делает вводимый вектор едининой длины (нормализует его)
     * */
    float size = sqrt(inp.x * inp.x + inp.y * inp.y + inp.z * inp.z);
    return vec3D(inp.x / size, inp.y / size, inp.z / size);
}


Camera::Camera(const vec3D eye, const vec3D center, const float fov, const int width, const int height, const vec3D up):
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
    vec3D direct = getNorm(vec3D{center.x - eye.x, center.y - eye.y, eye.z - center.z});  //center - eye;
    dist = float(WIDTH) / 2.0 * (1.0/tan(3.14/180.0 * float(fov)/2.0));
    std::cout << "dir: (" << direct.x << ", " << direct.y << ", " << -direct.z << ") dist" << dist << std::endl;
    if (direct.x == 0.0 && direct.y == 0.0){
        if (direct.z == 1.0) return ;
        if (direct.z == -1.0){
            matrixOfRotation[2].z = 1;
            return ;
        }
    }
    vec3D dirToMatrix(0.0,0.0,-1);
    vec3D v = vec3D(direct.y*dirToMatrix.z - direct.z*dirToMatrix.y,                                                     //cross multiplication
                    direct.z*dirToMatrix.x - direct.x*dirToMatrix.z,
                    direct.x*dirToMatrix.y - direct.y*dirToMatrix.x );
    float c = direct.x*dirToMatrix.x + direct.y*dirToMatrix.y + direct.z*dirToMatrix.z;                                     //dot mul
    float s = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);                                                                  //norm or length of vector
    vec3D kmat[3] = {vec3D{0, -v.z, v.y}, vec3D{v.z, 0, -v.x}, vec3D{-v.y, v.x, 0}};
    vec3D kmatDotKmat[3] = {vec3D{kmat[0].x * kmat[0].x + kmat[0].y * kmat[1].x + kmat[0].z * kmat[2].x,                 //kmatDotKmat = kmat * kmat
                                  kmat[0].x * kmat[0].y + kmat[0].y * kmat[1].y + kmat[0].z * kmat[2].y,
                                  kmat[0].x * kmat[0].z + kmat[0].y * kmat[1].z + kmat[0].z * kmat[2].z},
                            vec3D{kmat[1].x * kmat[0].x + kmat[1].y * kmat[1].x + kmat[1].z * kmat[2].x,
                                  kmat[1].x * kmat[0].y + kmat[1].y * kmat[1].y + kmat[1].z * kmat[2].y,
                                  kmat[1].x * kmat[0].z + kmat[1].y * kmat[1].z + kmat[1].z * kmat[2].z},
                            vec3D{kmat[2].x * kmat[0].x + kmat[2].y * kmat[1].x + kmat[2].z * kmat[2].x,
                                  kmat[2].x * kmat[0].y + kmat[2].y * kmat[1].y + kmat[2].z * kmat[2].y,
                                  kmat[2].x * kmat[0].z + kmat[2].y * kmat[1].z + kmat[2].z * kmat[2].z}
                            };
    matrixOfRotation[0] = vec3D{float(1.0 + kmat[0].x+kmatDotKmat[0].x*((1.0-c)/(s*s))),
                                float(0.0 + kmat[0].y+kmatDotKmat[0].y*((1.0-c)/(s*s))),
                                float(0.0 + kmat[0].z+kmatDotKmat[0].z*((1.0-c)/(s*s)))};
    matrixOfRotation[1] = vec3D{float(0.0 + kmat[1].x+kmatDotKmat[1].x*((1.0-c)/(s*s))),
                                float(1.0 + kmat[1].y+kmatDotKmat[1].y*((1.0-c)/(s*s))),
                                float(0.0 + kmat[1].z+kmatDotKmat[1].z*((1.0-c)/(s*s)))};
    matrixOfRotation[2] = vec3D{float(0.0 + kmat[2].x+kmatDotKmat[2].x*((1.0-c)/(s*s))),
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

    vec3D vec1 = getNorm(vec3D{ newRay.x * matrixOfRotation[0].x + newRay.y * matrixOfRotation[0].y + newRay.z * matrixOfRotation[0].z,
                                    newRay.x * matrixOfRotation[1].x + newRay.y * matrixOfRotation[1].y + newRay.z * matrixOfRotation[1].z,
                                    newRay.x * matrixOfRotation[2].x + newRay.y * matrixOfRotation[2].y + newRay.z * matrixOfRotation[2].z
                 });

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