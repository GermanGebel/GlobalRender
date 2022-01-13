#include "Color.h"
#include <cassert>

using namespace std;

Color::Color(vector<float> m1, vector<float> m2) {
    this->massOfWaveLength = m1;
    this->massOfColors = m2;
}


Color::Color(int start, int end, int step, vector<float> m2) {
    this->setMassOfWaveLength(start, end, step);
    this->massOfColors = m2;
}

void Color::setMassOfColors(vector<float> massOfColors) {
    this->massOfColors = massOfColors;
}

void Color::setMassOfWaveLength(vector<float> massOfWaveLength) {
    this->massOfWaveLength = massOfWaveLength;
}

void Color::setMassOfWaveLength(int start, int end, int step) {
    vector<float> newMassOfWaveLength;
    for (int i = start; i <= end; i += step) {
        newMassOfWaveLength.push_back(i);
    }
    this->massOfWaveLength = newMassOfWaveLength;
}

Color Color::operator+(Color color) {
    assert(this->massOfColors.size() == color.massOfColors.size());

    for (int i = 0; i < this->massOfColors.size(); i++){
        this->massOfColors[i] += color.massOfColors[i];
    }

    return *this;
}


Color Color::operator-(Color color) {
    assert(this->massOfColors.size() == color.massOfColors.size());

    for (int i = 0; i < this->massOfColors.size(); i++) {
        this->massOfColors[i] -= color.massOfColors[i];
    }

    return *this;
}

Color Color::operator*(float value) {
    for (int i = 0; i < this->massOfColors.size(); i++) {
        this->massOfColors[i] *= value;
    }

    return *this;
}

