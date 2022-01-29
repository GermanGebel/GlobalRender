#include "Render.h"




void Render::render(const std::string& outputFileName) {
    int width = camera_->width;
    int height = camera_->height;
    std::vector<std::vector<SpectralValues>> outLuminance(height, std::vector<SpectralValues>(width, SpectralValues(0)));

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

#pragma omp parallel for shared(height, width, outLuminance) default(none)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Ray ray = camera_->castRay(i, j);
            outLuminance[i][j] = getLuminance(ray);
        }
    }

#pragma omp parallel for shared(height, width, outLuminance) default(none)
    for (int i = 1; i < height - 1; ++i) {
        for (int j = 1; j < width - 1; ++j) {
            if (outLuminance[i][j].compare(outLuminance[i - 1][j]) > 1 ||
                outLuminance[i][j].compare(outLuminance[i][j - 1]) > 1 ||
                outLuminance[i][j].compare(outLuminance[i + 1][j]) > 1 ||
                outLuminance[i][j].compare(outLuminance[i][j + 1]) > 1) {
                for (int iOffset = 0; iOffset < ANTIALIASING_FACTOR; ++iOffset) {
                    for (int jOffset = 0; jOffset < ANTIALIASING_FACTOR; ++jOffset) {
                        Ray ray = camera_->castRay(i, j, ANTIALIASING_FACTOR, iOffset, jOffset);
                        outLuminance[i][j] += getLuminance(ray);
                    }
                }
                outLuminance[i][j] = outLuminance[i][j] / (ANTIALIASING_FACTOR * ANTIALIASING_FACTOR);
            }
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Rendering time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0 << "s" << std::endl;

    std::ofstream out(outputFileName);

    for (int waveLength : {400, 500, 600, 700}) {
        out << "wave_length " << waveLength << std::endl;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                out << outLuminance[i][j].values[waveLength] << " ";
            }
            out << std::endl;
        }
        out << std::endl;
    }

    out.close();
}


Render::HitPoint Render::getIntersection(const Ray& ray) {
    double t = std::numeric_limits<double>::max();
    int triangleId = -1;
    for (int id = 0; id < triangles_.size(); ++id) {
        if (triangles_[id].hitTest(ray, t)) {
            triangleId = id;
        }
    }
    vec3 hitPoint = triangleId == -1 ? vec3(0, 0, 0) : ray.origin + ray.direction * t;

    return { hitPoint, triangleId };
}


SpectralValues Render::getLuminance(const Ray& ray) {
    SpectralValues luminance(0);

    HitPoint nextHitPoint = getIntersection(ray);

    vec3 hitPoint = nextHitPoint.point;
    int triangleId = nextHitPoint.triangleId;
    if (triangleId == -1) {
        return luminance;
    }

    vec3 viewVec = camera_->origin - hitPoint;
    vec3 N = triangles_[triangleId].getNormal(viewVec);
    vec3 reflectDir = N * 2 * viewVec.dot(N) - viewVec;

    // diffuse
    for (const std::unique_ptr<Light>& light : lights_) {
        luminance += light->calculateLuminance(hitPoint, N, triangles_, triangleId);
    }

    // reflection
    Ray reflectRay(hitPoint, reflectDir, triangles_[triangleId].material->ks);
    for (int iter = 0; iter < 5 && reflectRay.ks > 0.01; ++iter) { // no more than 3 reflections
        HitPoint reflectionHitPoint = getIntersection(reflectRay);
        if (reflectionHitPoint.triangleId == -1) {
            break;
        }

        N = triangles_[reflectionHitPoint.triangleId].getNormal(-reflectRay.direction);
        for (const std::unique_ptr<Light>& light : lights_) {
            luminance += light->calculateLuminance(reflectionHitPoint.point, N, triangles_, reflectionHitPoint.triangleId) * reflectRay.ks;
        }

        viewVec = -reflectDir;
        reflectDir = N * 2 * viewVec.dot(N) - viewVec;
        double ks = reflectRay.ks * triangles_[reflectionHitPoint.triangleId].material->ks;
        reflectRay = Ray(reflectionHitPoint.point, reflectDir, ks);
    }
    return luminance;
}
