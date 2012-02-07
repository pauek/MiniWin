
#include "miniwin.h"
using namespace miniwin;

#include <cmath>
using namespace std;

#include "util.h"
#include "objetos.h"

Objeto::Objeto(float _x, float _y) {
   x = _x;
   y = _y;
   a = 0;
   vx = 0;
   vy = 0;
}

void Objeto::mueve() {
   x += vx;
   y += vy;
   limites(x, XTAM);
   limites(y, YTAM);
}

void Objeto::rota(double da) {
   a += da;
}

void Objeto::acelera() {
   float ax = 0.0, ay = -0.5;
   rotar(ax, ay, 0.0, 0.0, a);
   vx = vx + ax;
   vy = vy + ay;
}

void Nave::vertices(float& x1, float& y1,
                    float& x2, float& y2,
                    float& x3, float& y3) const {
   x1 = x;      y1 = y - 40;
   x2 = x - 15; y2 = y + 10;
   x3 = x + 15; y3 = y + 10;
   rotar(x1, y1, x, y, a);
   rotar(x2, y2, x, y, a);
   rotar(x3, y3, x, y, a);
}

void Nave::pinta() const {
   float x1, y1, x2, y2, x3, y3;
   vertices(x1, y1, x2, y2, x3, y3);
   linea(x1, y1, x2, y2);
   linea(x2, y2, x3, y3);
   linea(x3, y3, x1, y1);
}

bool Nave::choca(Asteroide* a) const {
   float x1, y1, x2, y2, x3, y3;
   vertices(x1, y1, x2, y2, x3, y3);
   return a->dentro(x1, y1) ||
          a->dentro(x2, y2) ||
          a->dentro(x3, y3) ||
          a->dentro((x1 + x2) / 2, (y1 + y2) / 2) ||
          a->dentro((x1 + x3) / 2, (y1 + y3) / 2);
}

Misil *Nave::dispara() const {
   float x1, y1, x2, y2, x3, y3;
   vertices(x1, y1, x2, y2, x3, y3);
   return new Misil(x1, y1, a);
}

Asteroide::Asteroide()
  : Objeto(0, 0)
{
   x = al_azar(XTAM);
   while (x > XTAM * .3 && x < XTAM * .7) {
      x = al_azar(XTAM);
   }
   y = al_azar(YTAM);
   while (y > YTAM * .3 && y < YTAM * .7) {
      y = al_azar(YTAM);
   }
   tam = 32;
   vx = al_azar(1.0) - 0.5; // -0.5 ... 0.5
   vy = al_azar(1.0) - 0.5;
   puntos();
}

Asteroide::Asteroide(float _x, float _y, float _tam,
                     float _vx, float _vy)
   : Objeto(_x, _y)
{
   tam = _tam;
   vx = _vx;
   vy = _vy;
   puntos();
}

void Asteroide::puntos() {
   for (int i = 0; i < 12; i++) {
      float _x = 0;
      float _y = tam + al_azar(tam * .5) - tam * .25;
      rotar(_x, _y, 0, 0, 30 * i);
      X.push_back(_x);
      Y.push_back(_y);
   }
}

void Asteroide::pinta() const {
   for (int i = 1; i < X.size(); i++) {
      linea(x + X[i-1], y + Y[i-1], x + X[i], y + Y[i]);
   }
   linea(x + X.back(), y + Y.back(), x + X[0], y + Y[0]);
}

bool Asteroide::dentro(float _x, float _y) const {
   float dx = _x - x, dy = _y - y;
   return sqrt(dx*dx + dy*dy) < tam;
}

void Asteroide::explota(list<Asteroide*>& A) const {
   if (tam > 30) {
      for (int i = 0; i < 4; i++) {
         float _vx = vx + al_azar(1.0) - .5;
         float _vy = vy + al_azar(1.0) - .5;
         A.push_back(new Asteroide(x, y, 16, _vx, _vy));
      }
   }
}

Misil::Misil(float _x, float _y, float _a)
  : Objeto(_x, _y)
{
   a = _a;
   vx = 0; vy = -5;
   rotar(vx, vy, 0, 0, a);
}

void Misil::pinta() const {
   float x1 = 0, y1 = -5, x2 = 0, y2 = 5;
   rotar(x1, y1, 0, 0, a);
   rotar(x2, y2, 0, 0, a);
   linea(x1 + x, y1 + y, x2 + x, y2 + y);
}

void Misil::mueve() {
   x += vx;
   y += vy;
}

bool Misil::fuera() const {
   return x > XTAM || x < 0 || y > YTAM || y < 0;
}

bool Misil::choca(Asteroide* ast) const {
   float x1 = 0, y1 = -5, x2 = 0, y2 = 5;
   rotar(x1, y1, 0, 0, a);
   rotar(x2, y2, 0, 0, a);
   return ast->dentro(x1 + x, y1 + y) ||
          ast->dentro(x2 + x, y2 + y);
}
