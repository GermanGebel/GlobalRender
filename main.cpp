#include "Math.h"
#include "Color.h"
#include "Ray.h"
#include "Geometry.h"
#include "Light.h"
#include "Camera.h"



int main() {

    std::vector<float> colors {400, 500, 600};
    std::vector<float> intensityTable{100, 50, 80, 60, 30, 10};
    Vec3f origin {0, 0, 10};
    Color myCol(colors);
    myCol.getColors();
//    Vec3f normal {0, 0, -1};
//    PointLight pLight(myCol,intensityTable, origin, normal, 440);
//    auto ks = pLight.getKs();
//    for (int i = 0; i < 10; i++){
//        auto temp = pLight.fireRay();
//        std::cout << temp.direction.x << " " << temp.direction.y << " " << temp.direction.z << "\n";
//    }
//    Vec3f point {0,0,5};
//    float E = pLight.calculateIlluminance(point, pLight.getRandomPointOfSurf());
//    std:: cout << E << std::endl;

    std::vector<Vec3f> ownPoints_;
    ownPoints_.push_back(Vec3f{-5, -5, 10});
    ownPoints_.push_back(Vec3f{5, -5, 10});
    ownPoints_.push_back(Vec3f{0, 5, 10    });
    Mesh myMesh;
    myMesh.ownPoints_ = ownPoints_;
    Triangle myTriangle(&myMesh, 0, 1, 2);
    myMesh.triangles_.push_back(myTriangle);

    Vec3f normal {0, 0, -1};
    RectangleLight rectangleLight(myCol, intensityTable, origin, normal, myMesh);
    for (int i = 0; i < 10; i++){
        auto temp = rectangleLight.fireRay();
        std::cout << temp.origin.x << " " << temp.origin.y << " " << temp.origin.z << "|| " << temp.direction.x << " " << temp.direction.y << " " << temp.direction.z << "\n";
    }
    Vec3f point {0,0,0};
    float E = rectangleLight.calculateLuminance(Vec3f(0,1,5));
    std:: cout << E << std::endl;

    return 0;
}
