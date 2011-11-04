
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "miniwin.h"
using namespace miniwin;

float frand(float max) {
   return max * float(rand()) / float(RAND_MAX);
}

int main() {
   srand(time(0));
   vredimensiona(500, 500);
   float x = frand(500), y = frand(500);
   rectangulo(x - 15, y - 15, x + 15, y + 15);
   refresca();

   float rx, ry;
   bool inicio = true, izq, der;

   int t = tecla();
   while (t != ESCAPE) {
      if (raton(rx, ry)) {
         if (fabs(x - rx) < 15 && fabs(y - ry) < 15) {
            x = frand(500), y = frand(500);
            borra();
            rectangulo(x - 15, y - 15, x + 16, y + 16);
            refresca();
         }
      }
      t = tecla();
   }
   vcierra();
   return 0;
}
