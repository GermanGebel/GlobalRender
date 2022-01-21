#include "Ray.h"
#include "Geometry.h"
#include "structs.h"

#include <vector>
#include <time.h>
#include <random>
#include <memory>
#include <utility>

#include "Light.h"


int binarySearch(const std::vector<float> &array, double value) {
    int left = 0, right = array.size();

    while (left + 1 < right) {
        int middle = (left + right) / 2;
        if (array[middle] < value)
            left = middle;
        else
            right = middle;
    }
    return left;
}

// =============================================================================
// =============================== Light =======================================
// =============================================================================

Light::Light(const Color &ks, std::vector<float> intensityTable, Vec3f normal, float flux) :
        flux_(flux), intensityTable_(std::move(intensityTable)), ks_(ks), normal_(normal.normalize()) {

    // Вычисляем интеграл по tempTable
    // это потом источника света
    float calculated_flux = calculateFlux();
    if (flux_ == -1) {
        flux_ = calculated_flux;
    } else {
        for (auto &intensity : intensityTable_) {
            intensity *= flux / calculated_flux;
        }
    }

    srand(time(0));
    tabularProbabilities.push_back(intensityTable_[0]);
    for (int i = 1; i < intensityTable_.size(); ++i) {
        tabularProbabilities.push_back(
                intensityTable_[i] + tabularProbabilities[i - 1]);
    }
}

Color Light::getKs() const {
    return ks_;
}

std::vector<float> Light::intensityTableExtension() {
    // делаем tempTable копией intensityTable но с большей дискритизацией
    // это уменьшит ошибку для расчета общего потока и таблицы яркостей
    std::vector<float> tempTable;
    if (intensityTable_.size() == 1) {
        for (int i = 0; i < 90; i++) {
            tempTable.push_back(intensityTable_[0]);
        }
    } else {
        int width = 90.0 / (float(intensityTable_.size()) - 1.0);
        for (int i = 0; i < intensityTable_.size() - 1; i++) {
            float k = (intensityTable_[i + 1] - intensityTable_[i]) / width;
            tempTable.push_back(intensityTable_[i]);
            for (int j = 1; j < int(width); j++) {
                tempTable.push_back(tempTable.back() + k);
            }
        }
        tempTable.push_back(intensityTable_.back());
    }
    return tempTable;
}

float Light::calculateFlux() {
    // Проинтегрировать таблицу интенсивностей и получить общий поток
    // интегрируем по сфере
    std::vector<float> intensityTable = intensityTableExtension();
    float width = 90.0f / float(intensityTable.size() - 1);
    float angle = 0;
    float totalFlux = 0;
    for (int step = 0; step < intensityTable.size() - 1; step++) {
        totalFlux += (intensityTable[step] + intensityTable[step + 1]) / 2.0 *
                     (cos(angle * M_PI / 180.0) - cos((angle + width) * M_PI / 180.0));
        angle += width;
    }
    totalFlux *= 2 * M_PI;
    return totalFlux;
}

float Light::calculateIlluminance(Vec3f surfPoint, Vec3f lightPoint) {

    // Расчер освещенност (есть точка источнка, и как он освещает какую-то точку в пространсве
    // E = I/(r*r) * cos(alpha)

    Vec3f direRay = surfPoint - lightPoint;
    float distanse = direRay.length();
    float angle = acos((direRay.x * normal_.x + direRay.y * normal_.y + direRay.z * normal_.z)/distanse) * 180 / M_PI;

    if (angle > 90)
        return 0;
    float step = 90.0 / (intensityTable_.size() - 1);

    int leftPosLumTable = angle / step;
    float luminance = intensityTable_[leftPosLumTable] + ((intensityTable_[leftPosLumTable + 1] -
                                                           intensityTable_[leftPosLumTable]) / step) *
                                                         (angle - leftPosLumTable * step);

    return luminance * cos(angle * M_PI / 180)/ (distanse * distanse );
}

Vec3f Light::getDir() {
    //Выбор направления испускаемого луча с учетом нормали и таблицы интенсивности
    float phi = rand() % 360;
    float theta;
    if (tabularProbabilities.size() == 1) {
        theta = rand() % 90;
    } else {
        float randIntens =
                rand() %
                int(tabularProbabilities.back() - tabularProbabilities[0]) +
                tabularProbabilities[0];
        int leftPos = binarySearch(tabularProbabilities, randIntens);

        float widht = 90.0 / (tabularProbabilities.size() - 1);           //22.5


        theta = widht * leftPos +
                (widht / (tabularProbabilities[leftPos + 1] -
                          tabularProbabilities[leftPos])) *
                (randIntens - tabularProbabilities[leftPos]);
    }

    phi = phi * M_PI / 180;
    theta = theta * M_PI / 180;
    float x = sin(theta) * cos(phi);
    float y = sin(theta) * sin(phi);
    float z = cos(theta);

    Vec3f dir{x, y, z};
    Vec3f result;
    result.x = rotationMatrix[0].x * dir.x + rotationMatrix[0].y * dir.y + rotationMatrix[0].z * dir.z;
    result.y = rotationMatrix[1].x * dir.x + rotationMatrix[1].y * dir.y + rotationMatrix[1].z * dir.z;
    result.z = rotationMatrix[2].x * dir.x + rotationMatrix[2].y * dir.y + rotationMatrix[2].z * dir.z;

    return result;
}


// =============================================================================
// =============================== PointLight ==================================
// =============================================================================
PointLight::PointLight(const Color &ks,
                       const std::vector<float> &intensityTable,
                       const Vec3f &origin, const Vec3f &normal, float flux) :
        Light(ks, intensityTable, normal, flux), origin_(origin) {
    rotationMatrix = getRotationMatrix(Vec3f(0, 0, 1), normal_);
}

Vec3f PointLight::getRandomPointOfSurf(){
    return origin_;
};

Ray PointLight::fireRay() {
    Ray newRay;
    newRay.luminance = ks_;
    newRay.direction = getDir();
    newRay.origin = origin_;
    return newRay;
}

// =============================================================================
// =============================== RectangleLight ==============================
// =============================================================================

RectangleLight::RectangleLight(const Color &ks,
                               const std::vector<float> &intensityTable,
                               const Vec3f &origin, const Vec3f &normal,
                               const Mesh &mesh, float flux) :
        Light(ks, intensityTable, normal, flux), geometryMesh_(mesh) {

    float dS = geometryMesh_.getSurfaceArea(); //mesh.getS();
    std::vector<float> extendIntensityTable = intensityTableExtension();
    float step = M_PI_2 / (extendIntensityTable.size() - 1);
    for (int i = 0; i < extendIntensityTable.size() - 1; i++) {
        luminanceTable_.push_back(extendIntensityTable[i] / (dS * cos(i * step)));
    }
    luminanceTable_.push_back(extendIntensityTable[extendIntensityTable.size() - 1] /
                              (dS * cos(89.0 * M_PI / 180.0)));

    rotationMatrix = getRotationMatrix(Vec3f(0, 0, 1),
                                       geometryMesh_.getNormal(geometryMesh_.randomSurfPoint(), normal_));
}

Vec3f RectangleLight::getRandomPointOfSurf(){
    return geometryMesh_.randomSurfPoint();
};

float RectangleLight::calculateLuminance(const Vec3f &rayDir) {
    Vec3f normal = geometryMesh_.getNormal(geometryMesh_.randomSurfPoint(), normal_);
    // Луч гулял по сцене и попал на источник света. Надо узнать испускаемую яркость в данном направлении
    normal = Vec3f(-normal.x, -normal.y, -normal.z);
    float angle = acos((rayDir.x * normal.x + rayDir.y * normal.y +
                        rayDir.z * normal.z) /
                       (sqrt(rayDir.x * rayDir.x + rayDir.y * rayDir.y +
                             rayDir.z * rayDir.z) *
                        sqrt(normal.x * normal.x + normal.y * normal.y +
                             normal.z * normal.z))) * 180 /
                  M_PI;              ///?????
    if (angle >= 90)
        return 0;
    if (luminanceTable_.size() == 1) {
        return luminanceTable_[0];
    }
    float step = 90.0 / (luminanceTable_.size() - 1);             //step = 22.5, angle = 50

    int leftPosLumTable = angle / step;
    float luminance = luminanceTable_[leftPosLumTable] +
                      ((luminanceTable_[leftPosLumTable + 1] -
                        luminanceTable_[leftPosLumTable]) / step) *
                      (angle - leftPosLumTable * step);
    return luminance;
}

Ray RectangleLight::fireRay() {
    Ray newRay;
    newRay.luminance = ks_;
    newRay.direction = getDir();
    newRay.origin = geometryMesh_.randomSurfPoint() +
                    geometryMesh_.getNormal(geometryMesh_.randomSurfPoint(), normal_) * 0.00001;
    return newRay;
}
