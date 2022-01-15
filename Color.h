// #pragma once

#include <vector>

using namespace std;

class Color {
private:
    vector<float> colors;
public:
    static vector<float> waveLengths;

    static void setWaveLengths(int start, int end, int step) {
        // создаем массив цветов с определенным шагом
        vector<float> newWaves;
        for (int i = start; i <= end; i += step) {
            newWaves.push_back(i * 1e-9);
        }
        waveLengths = newWaves;
    };

    static void setWaveLengths(vector<float> waves) {
        waveLengths = waves;
    };

    Color();
    Color(vector<float> colors);
    void setColors(vector<float> x);
    void setColors(int start, int end, int step);
    vector<float> getColors();
    void dot(float value);
    Color operator+(Color color);
    Color operator+(float value);
    Color operator-(Color color);
    Color operator-(float value);

    Color operator*(float value);
};