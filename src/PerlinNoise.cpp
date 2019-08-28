#include "PerlinNoise.hpp"
#include <glm.hpp>
#include <math.h>
#include <random>
#include <iostream>

int PerlinNoise::idx2D(double x, double y, int width)
{
   return (int)(y * width + x);
}

glm::mat2 PerlinNoise::genRotMat(double angle)
{
   glm::mat2 ret;

   ret[0][0] = (float)std::cos(angle);
   ret[1][0] = (float)-std::sin(angle);
   ret[0][1] = (float)std::sin(angle);
   ret[1][1] = (float)std::cos(angle);

   return ret;
}

double PerlinNoise::lerp(double v0, double v1, double t)
{
   return (1.0 - t) * v0 + t * v1;
}

double PerlinNoise::random(double low, double high, double seed)
{
   std::mt19937 mt((unsigned int)seed);
   std::uniform_real_distribution<double> generator(low, high);

   return generator(mt);
}

void PerlinNoise::genPerlinNoise(double* map, double mapWidth, double frequency, int seed)
{
   std::vector<glm::vec2> gridPointVecs;

   for(int i = 0; i < (frequency + 1) * (frequency + 1); i++)
   {
      glm::vec2 gridP = genRotMat(random(0, 360, seed * ((double)i + 3))) * glm::vec2(1.0, 0);
      gridPointVecs.push_back(gridP);
   }

   for(double y = 0; y < mapWidth; y++)
   {
      for(double x = 0; x < mapWidth; x++)
      {
         double x0 = 0.0;
         double y0 = 0.0;
         double x1 = 1.0;
         double y1 = 1.0;

         double cellX0 = std::floor(x / mapWidth * frequency);
         double cellY0 = std::floor(y / mapWidth * frequency);
         double cellX1 = cellX0 + 1;
         double cellY1 = cellY0 + 1;

         double px = (double)((int)x % (int)(mapWidth / frequency)) / (mapWidth / frequency);
         double py = (double)((int)y % (int)(mapWidth / frequency)) / (mapWidth / frequency);

         glm::vec2 vec1 = glm::vec2(px - x0, py - y0);
         glm::vec2 vec2 = glm::vec2(px - x1, py - y0);
         glm::vec2 vec3 = glm::vec2(px - x0, py - y1);
         glm::vec2 vec4 = glm::vec2(px - x1, py - y1);

         double s = (gridPointVecs[idx2D(cellX0, cellY0, (int)(frequency + 1))].x * vec1.x) + (gridPointVecs[idx2D(cellX0, cellY0, (int)(frequency + 1))].y * vec1.y);
         double t = (gridPointVecs[idx2D(cellX1, cellY0, (int)(frequency + 1))].x * vec2.x) + (gridPointVecs[idx2D(cellX1, cellY0, (int)(frequency + 1))].y * vec2.y);
         double u = (gridPointVecs[idx2D(cellX0, cellY1, (int)(frequency + 1))].x * vec3.x) + (gridPointVecs[idx2D(cellX0, cellY1, (int)(frequency + 1))].y * vec3.y);
         double v = (gridPointVecs[idx2D(cellX1, cellY1, (int)(frequency + 1))].x * vec4.x) + (gridPointVecs[idx2D(cellX1, cellY1, (int)(frequency + 1))].y * vec4.y);

         double Sx = (5.9025 * std::pow(px, 5) - 17.2025 * std::pow(px, 4) + 14.8 * std::pow(px, 3) - 2.7 * std::pow(px, 2) + 0.2 * px);

		   // a and b must be interpolated in the correct order - b after a.
         float a = (float)lerp(s, t, Sx);
         float b = (float)lerp(u, v, Sx);

         double Sy = (5.9025 * std::pow(py, 5) - 17.2025 * std::pow(py, 4) + 14.8 * std::pow(py, 3) - 2.7 * std::pow(py, 2) + 0.2 * py);

         map[(int)(y * mapWidth + x)] = lerp(a, b, Sy);
      }
   }
}
