#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <vector>

class Pixel {
public:
  unsigned char r, g, b;

  unsigned char &operator[](size_t index) {
    switch (index) {
    case 0:
      return r;
    case 1:
      return g;
    case 2:
      return b;
    default:
      throw std::out_of_range("Pixel index out of range");
    }
  }
};

class Fraktal {
private:
  std::vector<std::vector<Pixel>> pixels;

public:
  Fraktal(size_t width, size_t height) {
    pixels.resize(height, std::vector<Pixel>(width));
  }

  std::vector<Pixel> &operator[](size_t index) { return pixels[index]; }

  void generateMandelbrot(double xMin, double xMax, double yMin, double yMax,
                          int maxIterations) {
    size_t width = pixels[0].size();
    size_t height = pixels.size();

    for (size_t i = 0; i < height; ++i) {
      for (size_t j = 0; j < width; ++j) {
        double x = xMin + (xMax - xMin) * static_cast<double>(j) /
                              static_cast<double>(width);
        double y = yMin + (yMax - yMin) * static_cast<double>(i) /
                              static_cast<double>(height);

        std::complex<double> z(0.0, 0.0);
        std::complex<double> c(x, y);

        int iteration = 0;
        while (std::abs(z) < 2.0 && iteration < maxIterations) {
          z = z * z + c;
          ++iteration;
        }

        double ratio =
            static_cast<double>(iteration) / static_cast<double>(maxIterations);
        unsigned char value = static_cast<unsigned char>(ratio * 255);

        pixels[i][j] = {value, value, value};
      }
    }
  }

  void saveToFile(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
      for (const auto &row : pixels) {
        for (const auto &pixel : row) {
          file.write(reinterpret_cast<const char *>(&pixel), sizeof(Pixel));
        }
      }
      file.close();
      std::cout << "Fraktal zapisany do pliku binarnego." << std::endl;
    } else {
      std::cout << "Nie można otworzyć pliku." << std::endl;
    }
  }
};

int main() {
  Fraktal fractal(100, 100);

  double xMin = -2.0;
  double xMax = 1.0;
  double yMin = -1.5;
  double yMax = 1.5;
  int maxIterations = 1000;

  fractal.generateMandelbrot(xMin, xMax, yMin, yMax, maxIterations);

  fractal.saveToFile("fraktal.bin");

  return 0;
}
