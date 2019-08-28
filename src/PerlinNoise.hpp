#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#define PI 3.141592653589793238463;
#include <glm.hpp>

class PerlinNoise
{
   public:
      PerlinNoise() {};

      static glm::mat2 genRotMat(double angle);
      static double random(double low, double high, double seed);
      static double lerp(double v0, double v1, double t);
      static void genPerlinNoise(double* map, double mapWidth, double frequency, int seed);
      static int idx2D(double x, double y, int width);

   private:
};

#endif
