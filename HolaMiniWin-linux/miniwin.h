
/*
 *  MiniWin: Un mini-conjunto de funciones para abrir una ventana, pintar en
 *    ella y detectar la presión de algunas teclas. Básicamente para hacer
 *    juegos sencillos.
 *
 *  (c) Pau Fernández, licencia MIT: http://es.wikipedia.org/wiki/MIT_License
 */

// VERSION: 0.2.2


#ifndef _MINIWIN_H_
#define _MINIWIN_H_

#include <iostream>

#ifndef MINIWIN_SOURCE
#define main _main_ // Super-cutre hack! (pero funciona)
#endif

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
void vcierra();

void color(int c);
void color_rgb(int r, int g, int b);

void punto(float x, float y);
void linea(float x_ini, float y_ini, float x_fin, float y_fin);
void rectangulo(float izq, float arr, float der, float aba);
void rectangulo_lleno(float izq, float arr, float der, float aba);
void circulo(float x_cen, float y_cen, float radio);
void circulo_lleno(float x_cen, float y_cen, float radio);
void texto(float x, float y, const std::string& texto);

int tecla();

bool  raton(float& x, float& y);
bool  raton_dentro();
float raton_x();
float raton_y();
void  raton_botones(bool& izq, bool& der);
bool  raton_boton_izq();
bool  raton_boton_der();

enum {
  ESCAPE,
  IZQUIERDA, DERECHA, ARRIBA, ABAJO,
  F1, F2, F3, F4, F5, F6, F7, F8, F9, F10,
  ESPACIO,
  RETURN,
  NINGUNA
};

enum {
  NEGRO, ROJO, VERDE, AZUL,
  AMARILLO, MAGENTA, CYAN, BLANCO
};

} // namespace miniwin

#endif

