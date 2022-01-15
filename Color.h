#pragma once

#include <vector>
#include <cstdlib>

class Color {
private:
    std::vector<float> colors;
public:
    static std::vector<int> waveLengths;

    static void setWaveLengths(int start, int end, int step);
    static void setWaveLengths(const std::vector<int>& waves);

    Color();
    Color(const std::vector<float>& colors);
    void setColors(const std::vector<float>& x);
    std::vector<float> getColors() const;
    Color operator+(const Color& color) const;
    Color operator+(float value) const;
    Color operator-(const Color& color) const;
    Color operator-(float value) const;
    Color operator*(const Color& color) const;
    Color operator*(float value) const;
    float dot(const Color& color) const;

    float &operator[](const size_t i);
    const float &operator[](const size_t i) const;
};
