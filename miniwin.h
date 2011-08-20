
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

void borra();
void refresca();
void muestra_mensaje(char* missatge);
bool muestra_pregunta(char* missatge);
void espera(int miliseg);

int  ventana_ancho();
int  ventana_alto();
void ventana_redimensiona(int ample, int alt);

void linea(float x_ini, float y_ini, float x_fin, float y_fin);
void rectangulo(float esq, float dalt, float dre, float baix);
void circulo(float x_cen, float y_cen, float radi);
void pinta_texto(float x, float y, const std::string& texto);

int tecla();

enum { 
  IZQUIERDA, 
  DERECHA, 
  ARRIBA, 
  ABAJO, 
  ESCAPE, 
  NINGUNA 
};


#endif
