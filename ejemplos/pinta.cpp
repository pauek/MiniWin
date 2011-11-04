
#include "miniwin.h"
using namespace miniwin;

int main() {
   vredimensiona(500, 500);
   float x, y, ux, uy;
   bool inicio = true, izq, der;

   int t = tecla();
   while (t != ESCAPE) {
      if (raton(x, y)) {
         raton_botones(izq, der);
         if (izq || der) {
            if (inicio) {
               ux = x, uy = y;
               inicio = false;
            }
            if (raton_boton_izq()) {
               color(AMARILLO);
            } else if (raton_boton_der()) {
               color(ROJO);
            }
            linea(ux, uy, x, y);
            ux = x, uy = y;
            refresca();
         } else {
            inicio = true;
         }
      }
      t = tecla();
   }
   vcierra();
   return 0;
}
