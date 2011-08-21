
#include "miniwin.h"
using namespace miniwin;

int main() {
  vredimensiona(200, 200);
  linea(0, 0, 100, 100);
  color(AZUL);
  rectangulo(10, 10, 50, 50);
  color(ROJO);
  circulo(100, 50, 20);
  color(AZUL);
  circulo_lleno(50, 100, 20);
  color(VERDE);
  rectangulo_lleno(50, 50, 100, 100);
  refresca();  
}
