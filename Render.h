#pragma once
#include "Math.h"

class Render {
public:
	void render(const std::string& outputFileName);
private:
	struct HitPoint {
		Vec3f point;
		int triangleId;
	};

	HitPoint getIntersection(const Ray& ray);
	SpectralValues getLuminance(const Ray& ray);

	static const int ANTIALIASING_FACTOR = 4;
};
