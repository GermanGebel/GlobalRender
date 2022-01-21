#pragma once
#include "Math.h"
#include "Light.h"
#include "structs.h"
#include "Ray.h"
#include "Geometry.h"

class Light {
public:
    Light(const Color& ks, std::vector<float> intensityTable, Vec3f normal, float flux = -1);
    Color getKs() const;
    virtual ~Light()=default;
    virtual Ray   fireRay() = 0;
    virtual Vec3f getRandomPointOfSurf() = 0;
    float calculateIlluminance(Vec3f startRayPoint, Vec3f endRayPoint);

protected:
    float calculateFlux();
    Vec3f getDir();
    std::vector<float> intensityTableExtension();

protected:
    float flux_;
    Vec3f normal_;
    std::vector<float> intensityTable_;
    Color ks_;
    std::vector<float> tabularProbabilities;
    std::vector<Vec3f> rotationMatrix;
};

class PointLight : public Light {
public:
    PointLight(const Color& ks, const std::vector<float> &intensityTable,
               const Vec3f &origin, const Vec3f &normal, float flux = -1);
    Ray fireRay() override;
    Vec3f getRandomPointOfSurf() override;
private:
    Vec3f origin_;
};

class RectangleLight : public Light {
public:
    RectangleLight(const Color& ks, const std::vector<float> &intensityTable,
                   const Vec3f &origin, const Vec3f &normal, const Mesh &mesh, float flux = -1);
    float calculateLuminance(const Vec3f& rayDir) ;
    Vec3f getRandomPointOfSurf() override;
    Ray fireRay() override;

private:
    std::vector<float> luminanceTable_;

    Mesh geometryMesh_;
};
