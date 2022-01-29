#ifndef RAYTRACING_MATH_H
#define RAYTRACING_MATH_H

#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

template<size_t DIM, class T>
struct vec {
    vec() { for (size_t i = DIM; i--; data_[i] = T()); }

    T &operator[](const size_t i) {
        assert(i < DIM);
        return data_[i];
    }

    const T &operator[](const size_t i) const {
        assert(i < DIM);
        return data_[i];
    }

private:
    T data_[DIM];
};

typedef vec<2, float> Vec2f;
typedef vec<2, double> Vec2d;
typedef vec<3, float> Vec3f;
typedef vec<3, double> Vec3d;

template<class T>
struct vec<2, T> {
    vec() : x(T()), y(T()) {}

    vec(T X, T Y) : x(X), y(Y) {}

    const T &operator[](const size_t i) const {
        assert(i < 2 && i >= 0);
        if (i == 0) {
            return x;
        } else if (i == 1) {
            return y;
        }
    }

    T &operator[](const size_t i) {
        return const_cast<T &>(static_cast<const vec<2, T> &>(*this)[i]);
    }

    T x, y;
};

template<class T>
struct vec<3, T> {
    vec() : x(T()), y(T()), z(T()) {}

    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    const T &operator[](const size_t i) const {
        assert(i < 3 && i >= 0);
        if (i == 0) {
            return x;
        } else if (i == 1) {
            return y;
        } else if (i == 2) {
            return z;
        }
    }

    T &operator[](const size_t i) {
        return const_cast<T &>(static_cast<const vec<3, T> &>(*this)[i]);
    }

    T norm() { return std::sqrt(x * x + y * y + z * z); }

    vec<3, T> &normalize(T l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }

    T length() const {
        return sqrt(x * x + y * y + z * z);
    }

    T x, y, z;
};

template<size_t DIM, class T>
bool operator==(const vec<DIM, T> &lhs, const vec<DIM, T> &rhs) {
    if (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z)
        return true;
    return false;
}

template<size_t DIM, class T>
T operator*(const vec<DIM, T> &lhs, const vec<DIM, T> &rhs) {
    T ret = T();
    for (size_t i = DIM; i--; ret += lhs[i] * rhs[i]);
    return ret;
}

template<size_t DIM, class T>
vec<DIM, T> operator+(vec<DIM, T> lhs, const vec<DIM, T> &rhs) {
    for (size_t i = DIM; i--; lhs[i] += rhs[i]);
    return lhs;
}

template<size_t DIM, class T>
vec<DIM, T> operator-(vec<DIM, T> lhs, const vec<DIM, T> &rhs) {
    for (size_t i = DIM; i--; lhs[i] -= rhs[i]);
    return lhs;
}

template<size_t DIM, class T, typename U>
vec<DIM, T> operator*(const vec<DIM, T> &lhs, const U &rhs) {
    vec<DIM, T> ret;
    for (size_t i = DIM; i--; ret[i] = lhs[i] * rhs);
    return ret;
}

template<size_t DIM, class T>
vec<DIM, T> operator-(const vec<DIM, T> &lhs) {
    return lhs * T(-1);
}



template<class T>
vec<3, T> cross(vec<3, T> v1, vec<3, T> v2) {
    return vec<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                     v1.x * v2.y - v1.y * v2.x);
}

template<size_t DIM, class T>
std::ostream &operator<<(std::ostream &out, const vec<DIM, T> &v) {
    for (unsigned int i = 0; i < DIM; i++) {
        out << v[i] << " ";
    }
    return out;
}


//std::vector<Vec3f> getRotationMatrix(Vec3f direct, Vec3f dirToMatrix) {
//    direct = direct.normalize();
//    dirToMatrix = dirToMatrix.normalize();
//
//    std::vector<Vec3f> matrixOfRotation(3);
//
//    if (direct == dirToMatrix) {
//        matrixOfRotation[0] = {1, 0, 0};
//        matrixOfRotation[1] = {0, 1, 0};
//        matrixOfRotation[2] = {0, 0, 1};
//
//        return matrixOfRotation;
//    }
//
//    if (direct == -dirToMatrix) {
//        matrixOfRotation[0] = {-1, 0, 0};
//        matrixOfRotation[1] = {0, -1, 0};
//        matrixOfRotation[2] = {0, 0, -1};
//
//        return matrixOfRotation;
//    }
//
//    Vec3f v = Vec3f(direct.y*dirToMatrix.z - direct.z*dirToMatrix.y,                                                     //cross multiplication
//                  direct.z*dirToMatrix.x - direct.x*dirToMatrix.z,
//                  direct.x*dirToMatrix.y - direct.y*dirToMatrix.x );
//    float c = direct.x*dirToMatrix.x + direct.y*dirToMatrix.y + direct.z*dirToMatrix.z;                                     //dot mul
//    float s = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);                                                                  //norm or length of vector
//    Vec3f kmat[3] = {Vec3f{0, -v.z, v.y}, Vec3f{v.z, 0, -v.x}, Vec3f{-v.y, v.x, 0}};
//    Vec3f kmatDotKmat[3] = {Vec3f{kmat[0].x * kmat[0].x + kmat[0].y * kmat[1].x + kmat[0].z * kmat[2].x,                 //kmatDotKmat = kmat * kmat
//                                kmat[0].x * kmat[0].y + kmat[0].y * kmat[1].y + kmat[0].z * kmat[2].y,
//                                kmat[0].x * kmat[0].z + kmat[0].y * kmat[1].z + kmat[0].z * kmat[2].z},
//                            Vec3f{kmat[1].x * kmat[0].x + kmat[1].y * kmat[1].x + kmat[1].z * kmat[2].x,
//                                kmat[1].x * kmat[0].y + kmat[1].y * kmat[1].y + kmat[1].z * kmat[2].y,
//                                kmat[1].x * kmat[0].z + kmat[1].y * kmat[1].z + kmat[1].z * kmat[2].z},
//                            Vec3f{kmat[2].x * kmat[0].x + kmat[2].y * kmat[1].x + kmat[2].z * kmat[2].x,
//                                kmat[2].x * kmat[0].y + kmat[2].y * kmat[1].y + kmat[2].z * kmat[2].y,
//                                kmat[2].x * kmat[0].z + kmat[2].y * kmat[1].z + kmat[2].z * kmat[2].z}
//    };
//    matrixOfRotation[0] = Vec3f{float(1.0 + kmat[0].x+kmatDotKmat[0].x*((1.0-c)/(s*s))),
//                               float(0.0 + kmat[0].y+kmatDotKmat[0].y*((1.0-c)/(s*s))),
//                               float(0.0 + kmat[0].z+kmatDotKmat[0].z*((1.0-c)/(s*s)))};
//    matrixOfRotation[1] = Vec3f{float(0.0 + kmat[1].x+kmatDotKmat[1].x*((1.0-c)/(s*s))),
//                               float(1.0 + kmat[1].y+kmatDotKmat[1].y*((1.0-c)/(s*s))),
//                               float(0.0 + kmat[1].z+kmatDotKmat[1].z*((1.0-c)/(s*s)))};
//    matrixOfRotation[2] = Vec3f{float(0.0 + kmat[2].x+kmatDotKmat[2].x*((1.0-c)/(s*s))),
//                               float(0.0 + kmat[2].y+kmatDotKmat[2].y*((1.0-c)/(s*s))),
//                               float(1.0 + kmat[2].z+kmatDotKmat[2].z*((1.0-c)/(s*s)))};
//
//    return matrixOfRotation;
//}



#endif //RAYTRACING_MATH_H
