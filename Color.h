// #pragma once

#include <vector>

using namespace std;

class Color {
private:
    vector<float> massOfColors;
public:
    static vector<float> massOfWaveLength;
    Color(vector<float> massOfWaveLength, vector<float> massOfColors);
    Color(int start, int end, int step, vector<float> massOfColors);
    void setMassOfWaveLength(int start, int end, int step);
    void setMassOfWaveLength(vector<float> x);
    void setMassOfColors(vector<float> x);
    vector<float> getMassOfColors();
    void dotMassOfColors(float value);
    Color operator+(Color color);
    Color operator-(Color color);
    Color operator*(float value);    
};
