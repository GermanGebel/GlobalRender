#pragma once
#include "Math.h"
#include "Ray.h"
#include "Scene.h"

class Render {
public:
  Render(Scene * scene_);
//	void render(const std::string& outputFileName);
	void renderZBuffer(const std::string& outputFileName)
private:
	struct HitPoint {
		Vec3f point;
		int triangleId;
	};

	static const int ANTIALIASING_FACTOR = 4;
	Scene *scene;
};
