#ifndef OBJETOS_H
#define OBJETOS_H

#include <list>
#include <vector>

const float XTAM = 640, YTAM = 480;

class Objeto {
protected:
   float x, y, a, vx, vy;
public:
   Objeto(float _x, float _y);
   void mueve();
   void rota(double da);
   void acelera();
};

class Asteroide : public Objeto {
   float tam;
   std::vector<float> X, Y;

   void puntos();
public:
   Asteroide();
   Asteroide(float _x, float _y, float _tam,
             float _vx, float _vy);
   void pinta() const;
   bool dentro(float x, float y) const;
   void explota(std::list<Asteroide*>& A) const;
};

class Misil : public Objeto {
public:
   Misil(float _x, float _y, float _a);
   void pinta() const;
   void mueve();
   bool fuera() const;
   bool choca(Asteroide* a) const;
};

class Nave : public Objeto {
   void vertices(float& x1, float& y1,
                 float& x2, float& y2,
                 float& x3, float& y3) const;
public:
   Nave(float _x, float _y) : Objeto(_x, _y) {}
   void pinta() const;
   bool choca(Asteroide* a) const;
   Misil *dispara() const;
};

#endif // OBJETOS_H
