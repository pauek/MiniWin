
#include "miniwin.h"
using namespace miniwin;

struct Pelota {
   float x, y;
   float dx, dy;
};

void pinta_pelota(const Pelota& P) {
   borra();
   circulo(P.x, P.y, 20);
   refresca();
}

void mueve_pelota(Pelota& P) {
   const float factor = 0.97;
   P.x += P.dx;
   P.y += P.dy;
   if (P.x > vancho() - 20) {
      P.dx = -P.dx * factor;
      P.x = vancho() - 20;
   } else if (P.x < 20) {
      P.dx = -P.dx * factor;
      P.x = 20;
   } else if (P.y > valto() - 20) {
      P.dy = -P.dy * factor;
      P.y = valto() - 20;
   } else if (P.y < 20) {
      P.dy = -P.dy * factor;
      P.y = 20;
   }
   P.dy += 0.1;
}

int main() {
   vredimensiona(800, 600);
   Pelota P = { 50, 50, 2, 2 };
   while (tecla() != ESCAPE) {
      mueve_pelota(P);
      pinta_pelota(P);
      espera(5);
   }
   vcierra();
   return 0;
}
