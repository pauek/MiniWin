
#include "miniwin.h"
using namespace miniwin;

#include <list>
#include <cstdlib>
#include <sstream>
using namespace std;

#include "objetos.h"

class Juego {
   Nave N;
   list<Asteroide*> A;
   list<Misil*> M;
   bool _game_over;
public:
   Juego();
   bool game_over() const { return _game_over; }
   void movimiento();
   void colisiones();
   void tecla(int t);
   void repinta() const;
};

Juego::Juego()
  : N(XTAM / 2, YTAM / 2)
{
   srand(time(0)); // aleatorizar el primer elemento
   vredimensiona(XTAM, YTAM);

   for (int i = 0; i < 8; i++) {
      A.push_back(new Asteroide());
   }

   _game_over = false;
}

void Juego::movimiento() {
   N.mueve();
   for (auto it = A.begin(); it != A.end(); it++) {
      (*it)->mueve();
   }
   for (auto itm = M.begin(); itm != M.end(); itm++) {
      (*itm)->mueve();
   }
   auto itm = M.begin();
   while (itm != M.end()) {
      if ((*itm)->fuera()) {
         delete (*itm);
         itm = M.erase(itm);
      } else {
         itm++;
      }
   }
}

void Juego::colisiones() {
   // Nave con asteroides
   for (auto it = A.begin(); it != A.end(); it++) {
      if (N.choca(*it)) {
         _game_over = true;
      }
   }
   auto itm = M.begin();
   while (itm != M.end()) {
      // buscar un asteroide que tenga el misil dentro
      auto ita = A.begin();
      while (ita != A.end() && !(*itm)->choca(*ita)) {
         ita++;
      }
      if (ita != A.end()) {
         delete *itm;
         itm = M.erase(itm);
         (*ita)->explota(A);
         delete *ita;
         ita = A.erase(ita);
      } else {
         itm++;
      }
   }
}

void Juego::tecla(int t) {
   if (t == DERECHA) {
      N.rota(-10);
   } else if (t == IZQUIERDA) {
      N.rota(10);
   } else if (t == ARRIBA) {
      N.acelera();
   } else if (t == ESPACIO) {
      M.push_back(N.dispara());
   }
}

void Juego::repinta() const {
   borra();
   N.pinta();
   for (auto it = A.begin(); it != A.end(); it++) {
      (*it)->pinta();
   }
   for (auto itm = M.begin(); itm != M.end(); itm++) {
      (*itm)->pinta();
   }
   stringstream S;
   S << M.size() << ' ' << A.size();
   texto(20, 20, S.str());
   refresca();
}

int main() {
   Juego J;

   int t = tecla();
   while (t != ESCAPE) {
      if (!J.game_over()) {
         J.movimiento();
         J.colisiones();
         J.tecla(t);
      }
      J.repinta();

      espera(10);
      t = tecla();
   }
   vcierra();
   return 0;
}
