
/*
 *  MiniWin: Un mini-conjunto de funciones para abrir una ventana, pintar en 
 *    ella y detectar la presión de algunas teclas. Básicamente para hacer 
 *    juegos sencillos.
 *
 *  (c) Pau Fernández, licencia MIT: http://es.wikipedia.org/wiki/MIT_License
 */

#ifndef _MINIWIN_H_
#define _MINIWIN_H_

#include <iostream>

#define main _main_ // Super-cutre hack! (pero funciona)
int _main_();
std::ostream& log();

// Funciones del API

namespace miniwin {
  
void borra();
void refresca();
void mensaje(std::string msj);
bool pregunta(std::string msj);
void espera(int miliseg);

int  vancho();
int  valto();
void vredimensiona(int ample, int alt);

void color(int c);
void color_rgb(int r, int g, int b);

void linea(float x_ini, float y_ini, float x_fin, float y_fin);
void rectangulo(float esq, float dalt, float dre, float baix);
void rectangulo_lleno(float izq, float arr, float der, float aba);
void circulo(float x_cen, float y_cen, float radi);
void circulo_lleno(float x_cen, float y_cen, float radi);
void texto(float x, float y, const std::string& texto);

int tecla();

enum { 
  IZQUIERDA, 
  DERECHA, 
  ARRIBA, 
  ABAJO, 
  ESCAPE, 
  NINGUNA 
};

enum {
  NEGRO,
  BLANCO,
  ROJO,
  AZUL,
  VERDE
};

}

#endif
