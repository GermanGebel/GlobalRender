#pragma once
#include "Math.h"
#include "Ray.h"
#include "Scene.h"

class Render {
public:
  Render(Scene * scene_);
//	void render(const std::string& outputFileName);
	void renderZBuffer(const std::string& outputFileName);
private:
	struct HitPoint {
		Vec3f point;
		int triangleId;
	};

private:
  Geometry* getIntersection(const Ray& ray, float& t, Geometry *exclude = nullptr) const;
  Light* chooseLight() const;

private:
	static const int ANTIALIASING_FACTOR = 4;
	static const int MAX_RENDER_DEPTH = 1000;

	float totalFlux = 0;
	std::vector<float> fluxes;

	Scene *scene;
};
