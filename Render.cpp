#include "Render.h"

Render::Render(Scene * scene_) : scene(scene_)
{

}

void Render::renderZBuffer(const std::string &outputFileName) {
  int width = scene->camera_->getWidth();
  int height = scene->camera_->getHeight();

  std::vector<std::vector<float>> zbuffer(height, std::vector<float>(width, 0.0);

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  std::vector<std::vector<Ray>> rays = scene->camera_->render();

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
            Ray ray = camera_->castRay(i, j, ANTIALIASING_FACTOR, iOffset,
                                       jOffset);
            outLuminance[i][j] += getLuminance(ray);
          }
        }
        outLuminance[i][j] = outLuminance[i][j] /
                             (ANTIALIASING_FACTOR * ANTIALIASING_FACTOR);
      }
    }
  }

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Rendering time = " <<
            std::chrono::duration_cast<std::chrono::milliseconds>(
                    end - begin).count() / 1000.0 << "s" << std::endl;

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

//void Render::render(const std::string& outputFileName) {
//    int width = scene->camera_->width;
//    int height = scene->camera_->height;
//    std::vector<std::vector<SpectralValues>> outLuminance(height, std::vector<SpectralValues>(width, SpectralValues(0)));
//
//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//
//#pragma omp parallel for shared(height, width, outLuminance) default(none)
//    for (int i = 0; i < height; ++i) {
//        for (int j = 0; j < width; ++j) {
//            Ray ray = camera_->castRay(i, j);
//            outLuminance[i][j] = getLuminance(ray);
//        }
//    }
//
//#pragma omp parallel for shared(height, width, outLuminance) default(none)
//    for (int i = 1; i < height - 1; ++i) {
//        for (int j = 1; j < width - 1; ++j) {
//            if (outLuminance[i][j].compare(outLuminance[i - 1][j]) > 1 ||
//                outLuminance[i][j].compare(outLuminance[i][j - 1]) > 1 ||
//                outLuminance[i][j].compare(outLuminance[i + 1][j]) > 1 ||
//                outLuminance[i][j].compare(outLuminance[i][j + 1]) > 1) {
//                for (int iOffset = 0; iOffset < ANTIALIASING_FACTOR; ++iOffset) {
//                    for (int jOffset = 0; jOffset < ANTIALIASING_FACTOR; ++jOffset) {
//                        Ray ray = camera_->castRay(i, j, ANTIALIASING_FACTOR, iOffset, jOffset);
//                        outLuminance[i][j] += getLuminance(ray);
//                    }
//                }
//                outLuminance[i][j] = outLuminance[i][j] / (ANTIALIASING_FACTOR * ANTIALIASING_FACTOR);
//            }
//        }
//    }
//
//    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//    std::cout << "Rendering time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0 << "s" << std::endl;
//
//    std::ofstream out(outputFileName);
//
//    for (int waveLength : {400, 500, 600, 700}) {
//        out << "wave_length " << waveLength << std::endl;
//        for (int i = 0; i < height; ++i) {
//            for (int j = 0; j < width; ++j) {
//                out << outLuminance[i][j].values[waveLength] << " ";
//            }
//            out << std::endl;
//        }
//        out << std::endl;
//    }
//
//    out.close();
//}
