#include "Color.h"

#include <vector>
#include <cassert>

std::vector<int> Color::waveLengths;

void Color::setWaveLengths(int start, int end, int step) {
    // Создаем массив длин волн с определенным шагом
    waveLengths.clear();
    for (int i = start; i <= end; i += step) {
      waveLengths.push_back(i);
    }
}

void Color::setWaveLengths(const std::vector<int>& waves) {
    waveLengths = waves;
};

Color::Color() : colors(waveLengths.size(), 0) {};

Color::Color(const std::vector<float>& colors) : colors(colors) {}

void Color::setColors(const std::vector<float>& colors) {
    this->colors = colors;
}

std::vector<float> Color::getColors() const {
    return colors;
}

Color Color::operator+(const Color& color) const {
    assert(this->colors.size() == color.colors.size());

    Color result;

    for (int i = 0; i < colors.size(); i++) {
        result.colors[i] = colors[i] + color.colors[i];
    }

    return result;
}

Color Color::operator+(float value) const {
    Color result;

    for (int i = 0; i < colors.size(); i++) {
        result.colors[i] = colors[i] + value;
    }

    return result;
}

Color Color::operator-(const Color& color) const {
    assert(this->colors.size() == color.colors.size());

    Color result;

    for (int i = 0; i < colors.size(); i++) {
      result.colors[i] = colors[i] - color.colors[i];
    }

    return result;
}


Color Color::operator-(float value) const {
    Color result;

    for (int i = 0; i < colors.size(); i++) {
      result.colors[i] = colors[i] - value;
    }

    return result;
}

Color Color::operator*(const Color& color) const {
    assert(this->colors.size() == color.colors.size());

    Color result;

    for (int i = 0; i < colors.size(); i++) {
      result.colors[i] = colors[i] * color.colors[i];
    }

    return result;
}

Color Color::operator*(float value) const {
    Color result;

    for (int i = 0; i < colors.size(); i++) {
      result.colors[i] = colors[i] * value;
    }

    return result;
}

float Color::dot(const Color& color) const {
  assert(this->colors.size() == color.colors.size());

  float result = 0;

  for (int i = 0; i < colors.size(); i++) {
    result += colors[i] * color.colors[i];
  }

  return result;
}

float& Color::operator[](const size_t i) {
    assert(i < colors.size());
    return colors[i];
}

const float& Color::operator[](const size_t i) const {
    assert(i < colors.size());
    return colors[i];
}

//#include <iostream>
//
//template <typename T>
//void showVector(const std::vector<T>& v) {
//    for (int i = 0; i < v.size(); i++) {
//        std::cout << v[i] << " ";
//    }
//    std::cout << std::endl;
//}
//
//void test() {
//    Color::setWaveLengths(500, 700, 100);
//    showVector(Color::waveLengths);
//
//    Color c1, c2, c3;
//    c1.setColors({400, 700, 100});
//    showVector(c1.getColors());
//
//    c2.setColors({450, 750, 100});
//    showVector(c2.getColors());
//
//    c1 = c1 + c2;
//    showVector(c1.getColors());
//
//    c1 = c1 + 400;
//    showVector(c1.getColors());
//
//    c1 = c1 - c2;
//    showVector(c1.getColors());
//
//    c1 = c1 - 400;
//    showVector(c1.getColors());
//
//    c1 = c1 * 2;
//    showVector(c1.getColors());
//
//    std::cout << c1 * c2 << std::endl;
//
//    c1 = c1 * 0;
//    showVector(c1.getColors());
//
//    c1 + c3; // assert check
//}
//
//int main() {
//    test();
//
//    return 0;
//}
