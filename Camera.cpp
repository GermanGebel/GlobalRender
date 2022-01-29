//
// Created by Alex- on 05.01.2022.
//

#include "Camera.h"
#include "structs.h"
#include "iostream"

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

    Vec3f dirToMatrix(0.0,0.0,1);

    matrixOfRotation = getRotationMatrix(dirToMatrix, direct);

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
     *
     * P.S. размер пикселя 1.
     * */

    if (X == -1){
        X = rand() % (HEIGHT);
        Y = rand() % (WIDTH);
    }
    float displacedX = X + ((float)rand() / RAND_MAX);
    float displacedY = Y + ((float)rand() / RAND_MAX);

    float h = float(HEIGHT) / 2.0 - 0.5;
    float w = float(WIDTH) / 2.0 - 0.5;

    Vec3f direction{float(-w + displacedX), float(h - displacedY), dist};

    direction = Vec3f {matrixOfRotation[0].x * direction.x  + matrixOfRotation[0].y * direction.y + matrixOfRotation[0].z * direction.z,
                       matrixOfRotation[1].x * direction.x  + matrixOfRotation[1].y * direction.y + matrixOfRotation[1].z * direction.z,
                       matrixOfRotation[2].x * direction.x  + matrixOfRotation[2].y * direction.y + matrixOfRotation[2].z * direction.z,
    }.normalize();

    Ray newRay;
    newRay.origin = eye;
    newRay.direction = direction;
    /////////////цвет////////////////////////

    return newRay;
}

std::vector<std::vector<Ray>> Camera::render() {

    /*
     * output: matrix - матрица лучей (с их точками начала и направления)
     *
     * функция по лучу в каждую точку камеры
     * */

    std::vector<std::vector<Ray>> matrix(HEIGHT);
    for (int i = 0; i < HEIGHT; i++){
        matrix[i].resize(WIDTH);
    }

    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            matrix[i][j] = castRay(j,i);
        }
    }
    return matrix;
}













//std::vector<std::vector<Ray>> Camera::renderMonteCarlo(int N) {
//    /*
//     * input: N - количество лучей для рандомного испускания
//     * output: matrix - матрица лучей (с их точками начала и направления)
//     *
//     * функция выпускает N случайны лучей из камеры
//     * */
//
//    std::vector<std::vector<Ray>> matrix(HEIGHT);
//    for (int i = 0; i < HEIGHT; i++){
//        matrix[i].resize(WIDTH);
//    }
//
//    for (int i = 0; i < N; i++){
//        int Y = rand() % (HEIGHT-1);
//        int X = rand() % (WIDTH-1);
//        matrix[Y][X] = castRay(X, Y);
//    }
//    return matrix;
//}