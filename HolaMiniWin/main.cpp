
#include "miniwin.h"
using namespace miniwin;

int main() {
   vredimensiona(400, 300);
   rectangulo(100, 100, 300, 200);
   texto(152, 142, "Hola, MiniWin!");
   refresca();
   return 0;
}
