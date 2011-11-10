
#include <cmath>
using namespace std;
#include "miniwin.h"
using namespace miniwin;

const int TAM = 500;
const int UN_SEGUNDO = 1000;

// Rota con centro (cx, cy) un ángulo da
void rota(float& x, float& y, float cx, float cy, float da) {
  float dx = x - cx, dy = y - cy;
  float r = sqrt(dx*dx + dy*dy);
  float a = atan2(dy, dx);
  a += da / 180 * M_PI; // se resta porque las y's estan giradas
  x = r * cos(a) + cx;
  y = r * sin(a) + cy;
}

// pinta una aguja con un ángulo
void aguja(float angulo, float longitud) {
  float cx = TAM / 2.0, cy = TAM / 2.0;
  float x = TAM / 2.0, y = TAM / 2.0 - longitud;
  rota(x, y, cx, cy, angulo);
  linea(cx, cy, x, y);
}

void incrementa_un_segundo(float& H, float& M, float& S) {
  S += 6.0;
  if (S >= 360.0) {
    S = 0.0;
    M += 6;
    if (M >= 360.0) {
      M = 0.0;
    }
    H += 0.5;
    if (H >= 360.0) {
      H = 0.0;
    }
  }
}

void pinta_esfera() {
  for (int i = 0; i < 60; i++) {
    float x = 0.0, y = -TAM/2.0;
    rota(x, y, 0.0, 0.0, i * 6.0);
    float F1 = 0.95, F2 = 0.90;
    color_rgb(50, 50, 100);
    if (i % 5 == 0) {
      F2 = 0.86;
      color_rgb(80, 80, 120);
    }
    linea(F1*x + TAM/2.0, F1*y + TAM/2.0, 
	  F2*x + TAM/2.0, F2*y + TAM/2.0);
  }
}

void pinta_agujas(float H, float M, float S) {
  pinta_esfera();
  color(BLANCO);
  aguja(H, TAM/2.0 * .5);
  aguja(M, TAM/2.0 * .95);
  color_rgb(128, 128, 128);
  aguja(S, TAM/2.0 * .85);
  refresca();
}

int main() {
  vredimensiona(TAM, TAM);
  float hora = 0, minuto = 0, segundo = 0; // en decenas de grados
  
  while (tecla() != ESCAPE) {
    borra();
    pinta_esfera();
    pinta_agujas(hora, minuto, segundo);
    espera(UN_SEGUNDO);
    incrementa_un_segundo(hora, minuto, segundo);
  }
  vcierra();
}
