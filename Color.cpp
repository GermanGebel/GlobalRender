/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby,
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

Color::Color() {};

Color::Color(vector<float> colors) {
    this->colors = colors;
};

void Color::setColors(vector<float> colors) {
    this->colors = colors;
}

void Color::setColors(int start, int end, int step) {
    // создаем массив цветов с определенным шагом
    vector<float> newColors;
    for (int i = start; i <= end; i += step) {
        newColors.push_back(i * 1e-9);
    }
    this->colors = newColors;
}

vector<float> Color::getColors() {
    return this->colors;
}

Color Color::operator+(Color color) {
    assert(this->colors.size() == color.colors.size());

    for (int i = 0; i < this->colors.size(); i++) {
        this->colors[i] += color.colors[i];
    }

    return *this;
}

Color Color::operator+(float value) {
    for (int i = 0; i < this->colors.size(); i++) {
        this->colors[i] += value;
    }

    return *this;
}

Color Color::operator-(Color color) {
    assert(this->colors.size() == color.colors.size());

    for (int i = 0; i < this->colors.size(); i++) {
        this->colors[i] -= color.colors[i];
    }

    return *this;
}


Color Color::operator-(float value) {
    for (int i = 0; i < this->colors.size(); i++) {
        this->colors[i] -= value;
    }

    return *this;
}

Color Color::operator*(float value) {
    for (int i = 0; i < this->colors.size(); i++) {
        this->colors[i] *= value;
    }

    return *this;
}


void showVector(vector<float> v) {
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}

vector<float> Color::waveLengths;

void test() {
    Color c1, c2, c3;
    c1.setColors(400, 700, 100);
    showVector(c1.getColors());

    c2.setColors(450, 750, 100);
    showVector(c2.getColors());

    c1 = c1 + c2;
    showVector(c1.getColors());

    c1 = c1 + 400 * 1e-9;
    showVector(c1.getColors());

    c1 = c1 - c2;
    showVector(c1.getColors());

    c1 = c1 - 400 * 1e-9;
    showVector(c1.getColors());

    c1 = c1 * 0;
    showVector(c1.getColors());

    Color::setWaveLengths(380, 720, 10);
    showVector(Color::waveLengths);

    c1 + c3; // assert check
}

int main() {
    test();

    return 0;
}
