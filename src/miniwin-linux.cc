
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <queue>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
using namespace std;

#define MINIWIN_SOURCE
#include "miniwin.h"

// Globals ///////////////////////////////////////////////////////////

int             _width  = 400;
int             _height = 300;
Display        *_dsp;
Window          _win;
XEvent          _report;
GC              _bufgc;
Pixmap          _buffer;
std::queue<int> _teclas;
bool            _end = false;
pthread_t       _thread;
pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;

//////////////////////////////////////////////////////////////////////

int rgbs[][3] = {
   {   0,   0,   0 }, // NEGRO
   { 255,   0,   0 }, // ROJO
   {   0, 255,   0 }, // VERDE
   {   0,   0, 255 }, // AZUL
   { 255, 255,   0 }, // AMARILLO
   { 255,   0, 255 }, // MAGENTA
   {   0, 255, 255 }, // CYAN
   { 255, 255, 255 }, // BLANCO
};

inline void _color_rgb(int r, int g, int b) {
   unsigned long pixel;
   pixel = (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF);
   XSetForeground(_dsp, _bufgc, pixel);
}

inline int _color(int col) {
   static int _curr = 0;
   int ret = _curr;
   if (col >= 0 && col < 8) {
      _color_rgb(rgbs[col][0], rgbs[col][1], rgbs[col][2]);
      _curr = col;
   }
   return ret;
}

inline void _fill() {
   XFillRectangle(_dsp, _buffer, _bufgc, 0, 0, _width, _height);
}

inline void _refresh() {
   XCopyArea(_dsp, _buffer, _win, _bufgc,
             0, 0, _width, _height, 0, 0);
   XFlush(_dsp);
}

inline void _lock()   { pthread_mutex_lock(&_mutex); }
inline void _unlock() { pthread_mutex_unlock(&_mutex); }

void _handlekey(KeySym key) {
   switch (key) {
   case XK_Escape: _teclas.push(miniwin::ESCAPE); break;
   case XK_space:  _teclas.push(miniwin::ESPACIO); break;
   case XK_Return: _teclas.push(miniwin::RETURN); break;
   case XK_Left:   _teclas.push(miniwin::IZQUIERDA); break;
   case XK_Right:  _teclas.push(miniwin::DERECHA); break;
   case XK_Up:     _teclas.push(miniwin::ARRIBA); break;
   case XK_Down:   _teclas.push(miniwin::ABAJO); break;
   default: {
      if ((key >= int('0') && key <= int('9')) ||
          (key >= int('A') && key <= int('Z'))) {
         _teclas.push(key);
      } else if (key >= int('a') && key <= int('z')) {
         _teclas.push(key - 32);
      } else if (key >= XK_F1 && key <= XK_F10) {
         int dif = key - XK_F1;
         _teclas.push(miniwin::F1 + dif);
      }
   }
   }
}

void _change_width_height(int w, int h) {
   XWindowChanges changes;
   changes.width  = w;
   changes.height = h;
   XConfigureWindow(_dsp, _win, CWWidth | CWHeight, &changes);
}

struct _mouse_state_t {
   float x, y;
   bool in_window, left_button, right_button;
} _mouse_state;

void _open_display() {
   // "XInitThreads must be the first Xlib function a multi-threaded program calls"
   // http://tronche.com/gui/x/xlib/display/XInitThreads.html
   /* XInitThreads(); */
	_dsp = XOpenDisplay(NULL);
}

void _new_window() {
	_win = XCreateSimpleWindow(_dsp, RootWindow(_dsp, 0),
                              0, 0, _width, _height,
                              0, BlackPixel (_dsp, 0), BlackPixel(_dsp, 0));
	XSelectInput(_dsp, _win,
                StructureNotifyMask |
                ExposureMask |
                KeyPressMask |
                ButtonPressMask |
                PointerMotionMask |
                ButtonPressMask |
                ButtonReleaseMask |
                EnterWindowMask |
                LeaveWindowMask
                );
	XMapWindow(_dsp, _win);

   // To know when is closed:
   // http://cboard.cprogramming.com/linux-programming/60466-xwindows-close-window-event.html
   Atom wmDelete = XInternAtom(_dsp, "WM_DELETE_WINDOW", True);
   XSetWMProtocols(_dsp, _win, &wmDelete, 1);
}

void _close_window() {
   XDestroyWindow(_dsp, _win);
}

void _new_buffer(bool free = false) {
   if (free) {
      XFreePixmap(_dsp, _buffer);
      XFreeGC(_dsp, _bufgc);
   }
   XWindowAttributes attrs;
   XGetWindowAttributes(_dsp, _win, &attrs);
   _buffer = XCreatePixmap(_dsp, _win, _width, _height, attrs.depth);
	_bufgc = XCreateGC(_dsp, _buffer, 0, 0);
   _color(miniwin::NEGRO);
   _fill();
   _color(miniwin::BLANCO);
}

void *_invoke_main(void *) {
   _main_();
   pthread_exit(NULL);
}

void _maybe_call_main() {
   static bool started = false;
   if (!started) {
      started = true;
      pthread_create(&_thread, NULL, _invoke_main, NULL);
   }
}

void _process_event() {
   switch  (_report.type) {
   case Expose: {
      _refresh();
      break;
   }
   case KeyPress: {
      KeySym key = XLookupKeysym(&_report.xkey, 0);
      _handlekey(key);
      break;
   }
   case MapNotify: {
      _maybe_call_main();
      break;
   }
   case ClientMessage: {
      // WM_DELETE_WINDOW
      _end = true;
      break;
   }
   case MotionNotify: {
      _mouse_state.x = _report.xmotion.x;
      _mouse_state.y = _report.xmotion.y;
      break;
   }
   case ButtonPress: {
      switch (_report.xbutton.button) {
      case 1: _mouse_state.left_button  = true; break;
      case 3: _mouse_state.right_button = true; break;
      }
      break;
   }
   case ButtonRelease: {
      switch (_report.xbutton.button) {
      case 1: _mouse_state.left_button = false; break;
      case 3: _mouse_state.right_button = false; break;
      }
      break;
   }
   case EnterNotify:
   case LeaveNotify: {
      _mouse_state.in_window = (_report.type == EnterNotify);
      break;
   }
   }
}

int main() {
   _open_display();
   _new_window();
   _new_buffer();

	while (!_end)  {
       _lock();
       if (XPending(_dsp) > 0) {
          XNextEvent(_dsp, &_report); // this blocks
          _process_event();
       }
       _unlock();
       usleep(100);
	}
   pthread_cancel(_thread);
   XDestroyWindow(_dsp, _win);
   XCloseDisplay(_dsp);
   pthread_exit(NULL);
}


////////////////////////////////////////////////////////////////////////////////
//
//   Funciones del API
//
////////////////////////////////////////////////////////////////////////////////

namespace miniwin {

void vredimensiona(int ancho, int alto) {
   _lock();
   _change_width_height(ancho, alto);
   _unlock();
   _width  = ancho;
   _height = alto;
   _lock();
   _new_buffer(true);
   _unlock();
}

int vancho() {
  return _width;
}

int valto() {
  return _height;
}

void vcierra() {
   _lock();
   _end = true;
   XUnmapWindow(_dsp, _win);
   XFlush(_dsp);
   _unlock();
}

void punto(float x, float y) {
   _lock();
   XDrawPoint(_dsp, _buffer, _bufgc, x, y);
   _unlock();
}

void linea(float x_ini, float y_ini, float x_fin, float y_fin) {
   // cerr << "linea(" << x_ini << ", " << y_ini << ", " << x_fin << ", " << y_fin << ")" << endl;
   _lock();
   XDrawLine(_dsp, _buffer, _bufgc, x_ini, y_ini, x_fin, y_fin);
   _unlock();
}

void rectangulo(float izq, float arr, float der, float aba) {
   _lock();
   XDrawRectangle(_dsp, _buffer, _bufgc, izq, arr, der - izq, aba - arr);
   _unlock();
}

void rectangulo_lleno(float izq, float arr, float der, float aba) {
   _lock();
   XFillRectangle(_dsp, _buffer, _bufgc, izq, arr, der - izq, aba - arr);
   _unlock();
}

void circulo(float x_cen, float y_cen, float radio) {
   float x = x_cen - radio, y = y_cen - radio;
   _lock();
   XDrawArc(_dsp, _buffer, _bufgc, x, y, 2*radio, 2*radio, 0, 360 * 64);
   _unlock();
}

void circulo_lleno(float x_cen, float y_cen, float radio) {
   float x = x_cen - radio, y = y_cen - radio;
   _lock();
   XFillArc(_dsp, _buffer, _bufgc, x, y, 2*radio, 2*radio, 0, 360 * 64);
   _unlock();
}

void texto(float x, float y, const std::string& texto) {
   _lock();
   XDrawString(_dsp, _buffer, _bufgc, x, y, texto.c_str(), texto.size());
   _unlock();
}

void color_rgb(int r, int g, int b) {
   _lock();
   _color_rgb(r, g, b);
   _unlock();
}

void color(int col) {
   _lock();
   _color(col);
   _unlock();
}


void refresca() {
   XEvent event;
   event.type = Expose;
   event.xexpose.window = _win;
   // repintar sin borrar (evitar XClearArea...)
   _lock();
   XSendEvent(_dsp, _win, False, ExposureMask, &event);
   XFlush(_dsp);
   _unlock();
}

void borra() {
   int prev = _color(miniwin::NEGRO);
   _lock();
   _fill();
   _color(prev);
   _unlock();
}

int tecla() {
   if (_teclas.empty()) {
      return NINGUNA;
   } else {
      int t = _teclas.front();
      _lock();
      _teclas.pop();
      _unlock();
      return t;
   }
}

bool raton(float& x, float& y) {
   x = _mouse_state.x;
   y = _mouse_state.y;
   return _mouse_state.in_window;
}

bool raton_dentro() {
   return _mouse_state.in_window;
}

float raton_x() {
   return _mouse_state.x;
}

float raton_y() {
   return _mouse_state.x;
}

bool raton_boton_izq() {
   return _mouse_state.left_button;
}

bool raton_boton_der() {
   return _mouse_state.right_button;
}

void raton_botones(bool& izq, bool& der) {
   izq = _mouse_state.left_button;
   der = _mouse_state.right_button;
}


void mensaje(string msj) {
   cerr << "Mensaje:" << msj << endl;
}

bool pregunta(string msj) {
   cerr << "Pregunta: " << msj << endl;
   return false;
}

void espera(int miliseg) {
   usleep(miliseg * 1000);
}

} // namespace miniwin

