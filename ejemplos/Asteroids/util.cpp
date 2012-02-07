
#include <cstdlib>
#include <cmath>
using namespace std;

#include "util.h"

void rotar(float& x, float& y, float cx, float cy, float da) {
   float dx = x - cx;
   float dy = y - cy;
   float r = sqrt(dx*dx + dy*dy);
   float a = atan2(dy, dx);
   a -= da / 180 * M_PI;
   x = cx + r * cos(a);
   y = cy + r * sin(a);
}

void limites(float& f, float max) {
   if (f > max) {
      f -= max;
   } else if (f < 0) {
      f += max;
   }
}

float al_azar(float max) {
   return max * (float(rand()) / float(RAND_MAX));
}
