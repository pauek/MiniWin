
#include <fstream>
#include <sstream>
#include <queue>
#include <math.h>
#include <process.h>
#include <windows.h>
#include <windowsx.h>

#define MINIWIN_SOURCE
#include "miniwin.h"

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

char szClassName[ ] = "MiniWin";

// Variables globales //////////////////////////////////////////////////////////

HWND            hWnd;              // ventana principal
HBITMAP         hBitmap;           // bitmap para pintar off-screen
int             iWidth  = 400;     // ancho de la ventana
int             iHeight = 300;     // alto de la ventana
HDC             hDCMem = NULL;     // Device Context en memoria
std::queue<int> _teclas;           // cola de teclas
bool            _raton_dentro;     // el raton está dentro del 'client area'
int             _xraton, _yraton;  // posicion del raton
bool            _bot_izq, _bot_der;// botones izquierdo y derecho

////////////////////////////////////////////////////////////////////////////////

std::ostream& log() {
#if defined(DEBUG)
   static std::ofstream _log("log.txt");
#else
   static std::stringstream _log;
   _log.str(""); // lo borra
#endif
   return _log;
}

VOID Thread(PVOID pvoid) {
   Sleep(50); // FIXME
   _main_();
}

void maybe_call_main() {
   static bool started = false;
   if (!started) {
      _beginthread(Thread, 0, NULL); // Llama a 'main' (realmente  '_main_')
      started = true;
   }
}

void frame_real(int w, int h, int& rw, int &rh) {
   RECT frame = { 0, 0, w, h };
   AdjustWindowRect(&frame, WS_OVERLAPPEDWINDOW, FALSE);
   rw = frame.right - frame.left;
   rh = frame.bottom - frame.top;
}

void newMemDC(int w, int h) {
   if (hDCMem != NULL) {
      DeleteObject(hBitmap);
      DeleteDC(hDCMem);
   }
   log() << "New MemDC\n";
   HDC hDC = GetDC(hWnd);
   hDCMem  = CreateCompatibleDC(hDC);
   hBitmap = CreateCompatibleBitmap (hDC, w, h);
   SelectObject(hDCMem, hBitmap);
   SetBkMode(hDCMem, TRANSPARENT);
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)
{
    static WNDCLASSEX wincl;
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (!RegisterClassEx (&wincl))
       return 0;

    int w, h;
    frame_real(iWidth, iHeight, w, h);

    hWnd = CreateWindowEx (
      0,                   /* Extended possibilites for variation */
      szClassName,         /* Classname */
      "MiniWin",           /* Title Text */
      WS_OVERLAPPEDWINDOW, /* default window */
      CW_USEDEFAULT,       /* Windows decides the position */
      CW_USEDEFAULT,       /* where the window ends up on the screen */
      w,                   /* The programs width */
      h,                   /* and height in pixels */
      HWND_DESKTOP,        /* The window is a child-window to desktop */
      NULL,                /* No menu */
      hThisInstance,       /* Program Instance handler */
      NULL                 /* No Window Creation data */
    );

    hBitmap = NULL;

    ShowWindow (hWnd, nFunsterStil);

    MSG messages;
    while (GetMessage (&messages, NULL, 0, 0)) {
       TranslateMessage(&messages);
       DispatchMessage(&messages);
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hWnd,
                                  UINT message,
                                  WPARAM wParam,
                                  LPARAM lParam)
{
   switch (message) {
   case WM_SIZE: {
      log() << "WM_SIZE\n";
      RECT R;
      GetClientRect(hWnd, &R);
      int w = R.right - R.left;
      int h = R.bottom - R.top;
      log() << w << ' ' << h << ' ' << '\n';
      if (w == 0 && h == 0) break; // Al minimizar envia WM_SIZE(0,0)

      if (hDCMem == NULL || w != iWidth || h != iHeight) {
         newMemDC(w, h);
         maybe_call_main();
      }
      break;
   }
   case WM_SIZING: {
      RECT* pRECT = (RECT*)lParam;
      log() << pRECT->top << ' ' << pRECT->left << ' '
            << pRECT->bottom << ' ' << pRECT->right << '\n';
      log() << iHeight << '\n';
      int w, h;
      frame_real(iWidth, iHeight, w, h);
      switch (wParam) {
      case WMSZ_BOTTOM:
         pRECT->bottom = pRECT->top + h;
         break;
      case WMSZ_TOP:
         pRECT->top = pRECT->bottom - h;
         break;
      case WMSZ_RIGHT:
         pRECT->right = pRECT->left + w;
         break;
      case WMSZ_LEFT:
         pRECT->left = pRECT->right - w;
         break;
      case WMSZ_TOPLEFT:
         pRECT->top = pRECT->bottom - h;
         pRECT->left = pRECT->right - w;
         break;
      case WMSZ_TOPRIGHT:
         pRECT->top = pRECT->bottom - h;
         pRECT->right = pRECT->left + w;
         break;
      case WMSZ_BOTTOMLEFT:
         pRECT->bottom = pRECT->top + h;
         pRECT->left = pRECT->right - w;
         break;
      case WMSZ_BOTTOMRIGHT:
         pRECT->bottom = pRECT->top + h;
         pRECT->right = pRECT->left + w;
         break;
      }

      return TRUE;
   }
   case WM_PAINT: {
      log() << "WM_PAINT\n";
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      SelectObject(hDCMem, hBitmap);
      if (hBitmap != NULL) {
         BitBlt(hdc, 0, 0, iWidth, iHeight, hDCMem, 0, 0, SRCCOPY);
      }
      EndPaint(hWnd, &ps);
      break;
   }
   case WM_MOUSEMOVE: {
      log() << "WM_MOUSEMOVE\n";
      _raton_dentro = true;
      _xraton = GET_X_LPARAM(lParam);
      _yraton = GET_Y_LPARAM(lParam);
      _bot_izq = wParam & MK_LBUTTON;
      _bot_der = wParam & MK_RBUTTON;
      break;
   }
   case WM_MOUSELEAVE: {
      _raton_dentro = false;
      break;
   }
   case WM_LBUTTONDOWN: {
      _bot_izq = true;
      break;
   }
   case WM_LBUTTONUP: {
      _bot_izq = false;
      break;
   }
   case WM_RBUTTONDOWN: {
      _bot_der = true;
      break;
   }
   case WM_RBUTTONUP: {
      _bot_der = false;
      break;
   }
   case WM_KEYDOWN: {
     bool push_it = false;

     // Escape
     push_it |= (wParam == VK_ESCAPE);

     // Flechas
     push_it |= (wParam == VK_LEFT ||
                 wParam == VK_RIGHT ||
                 wParam == VK_UP ||
                 wParam == VK_DOWN);

     // Barra espaciadora
     push_it |= (wParam == VK_SPACE);

     push_it |= (wParam == VK_RETURN);

     // Números 0-9
     push_it |= (wParam >= 48 && wParam <= 57);

     // Letras A-Z
     push_it |= (wParam >= 65 && wParam <= 90);

     // Teclas de función
     for (unsigned int i = 0; i < 10; i++) {
       push_it |= (wParam == (VK_F1 + i));
     }

     if (push_it) _teclas.push(wParam);

     break;
   }
   case WM_DESTROY: {
      DeleteObject (hBitmap);
      DeleteDC (hDCMem);
      PostQuitMessage(0);
      break;
   }
   default:
      return DefWindowProc (hWnd, message, wParam, lParam);
   }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//   Funciones del API
//
////////////////////////////////////////////////////////////////////////////////

COLORREF _color = RGB(255, 255, 255);

namespace miniwin {

int tecla() {
    if (_teclas.empty()) return NINGUNA;

    int ret = NINGUNA;
    switch(_teclas.front()) {
    case VK_LEFT:   ret = IZQUIERDA; break;
    case VK_RIGHT:  ret = DERECHA; break;
    case VK_UP:     ret = ARRIBA; break;
    case VK_DOWN:   ret = ABAJO; break;
    case VK_ESCAPE: ret = ESCAPE; break;
    case VK_SPACE:  ret = ESPACIO; break;
    case VK_RETURN: ret = RETURN; break;
    case VK_F1:     ret = F1; break;
    case VK_F2:     ret = F2; break;
    case VK_F3:     ret = F3; break;
    case VK_F4:     ret = F4; break;
    case VK_F5:     ret = F5; break;
    case VK_F6:     ret = F6; break;
    case VK_F7:     ret = F7; break;
    case VK_F8:     ret = F8; break;
    case VK_F9:     ret = F9; break;
    case VK_F10:    ret = F10; break;
    default: ret = _teclas.front();
    }
    _teclas.pop();
    return ret;
}

bool raton(float& x, float& y) {
   if (!_raton_dentro) {
      return false;
   }
   x = _xraton;
   y = _yraton;
   return true;
}

bool raton_dentro() {
   return _raton_dentro;
}

float raton_x() {
   return _xraton;
}

float raton_y() {
   return _yraton;
}

void raton_botones(bool& izq, bool& der) {
   izq = _bot_izq;
   der = _bot_der;
}

bool raton_boton_izq() {
   return _bot_izq;
}

bool raton_boton_der() {
   return _bot_der;
}

void espera(int miliseg) {
   Sleep(miliseg);
}

void mensaje(std::string msj) {
   MessageBox(hWnd, msj.c_str(), "Mensaje...", MB_OK);
}

bool pregunta(std::string msj) {
   return MessageBox(hWnd, msj.c_str(), "Pregunta...", MB_OKCANCEL) == IDOK;
}

void borra() {
   RECT R;
   SetRect(&R, 0, 0, iWidth, iHeight);
   HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
   FillRect(hDCMem, &R, hBrush);
   DeleteObject(hBrush);
}

void refresca() {
   InvalidateRect(hWnd, NULL, FALSE);
}

void punto(float x, float y) {
  SetPixel(hDCMem, int(x), int(y), _color);
}

void linea(float x_ini, float y_ini, float x_fin, float y_fin) {
   BeginPath(hDCMem);
   MoveToEx(hDCMem, int(x_ini), int(y_ini), NULL);
   LineTo(hDCMem, int(x_fin), int(y_fin));
   EndPath(hDCMem);
   HPEN hPen = CreatePen(PS_SOLID, 1, _color);
   SelectObject(hDCMem, hPen);
   StrokePath(hDCMem);
   DeleteObject(hPen);
}

inline void _rect(float izq, float arr, float der, float aba) {
   BeginPath(hDCMem);
   MoveToEx(hDCMem, int(izq), int(arr), NULL);
   LineTo(hDCMem, int(izq), int(aba));
   LineTo(hDCMem, int(der), int(aba));
   LineTo(hDCMem, int(der), int(arr));
   LineTo(hDCMem, int(izq), int(arr));
   EndPath(hDCMem);
}

void rectangulo(float izq, float arr, float der, float aba) {
   HPEN hPen = CreatePen(PS_SOLID, 1, _color);
   HGDIOBJ orig = SelectObject(hDCMem, hPen);
   _rect(izq, arr, der, aba);
   StrokePath(hDCMem);
   SelectObject(hDCMem, orig);
   DeleteObject(hPen);
}

void rectangulo_lleno(float izq, float arr, float der, float aba) {
   HBRUSH hBrush = CreateSolidBrush(_color);
   HGDIOBJ orig = SelectObject(hDCMem, hBrush);
   _rect(izq, arr, der, aba);
   FillPath(hDCMem);
   SelectObject(hDCMem, orig);
   DeleteObject(hBrush);
}

inline void _circ(float x_cen, float y_cen, float radio) {
   BeginPath(hDCMem);
   Arc(hDCMem, int(x_cen - radio), int(y_cen - radio),
               int(x_cen + radio), int(y_cen + radio),
               int(x_cen - radio), int(y_cen - radio),
               int(x_cen - radio), int(y_cen - radio));
   EndPath(hDCMem);
}

void circulo(float x_cen, float y_cen, float radio) {
   HPEN hPen = CreatePen(PS_SOLID, 1, _color);
   HGDIOBJ orig = SelectObject(hDCMem, hPen);
   _circ(x_cen, y_cen, radio);
   StrokePath(hDCMem);
   SelectObject(hDCMem, orig);
   DeleteObject(hPen);
}

void circulo_lleno(float x_cen, float y_cen, float radio) {
   HBRUSH hBrush = CreateSolidBrush(_color);
   HGDIOBJ orig = SelectObject(hDCMem, hBrush);
   _circ(x_cen, y_cen, radio);
   FillPath(hDCMem);
   SelectObject(hDCMem, orig);
   DeleteObject(hBrush);
}

void texto(float x, float y, const std::string& texto) {
   SetTextColor(hDCMem, _color);
   TextOut(hDCMem, int(x), int(y), texto.c_str(), int(texto.size()));
}

static COLORREF _colores[] = {
   RGB(0, 0, 0),       // NEGRO
   RGB(255, 0, 0),     // ROJO
   RGB(0, 255, 0),     // VERDE
   RGB(0, 0, 255),     // AZUL
   RGB(255, 255, 0),   // AMARILLO
   RGB(255, 0, 255),   // MAGENTA
   RGB(0, 255, 255),   // CYAN
   RGB(255, 255, 255), // BLANCO
};

void color(int c) {
   _color = _colores[c];
}

void color_rgb(int r, int g, int b) {
   _color = RGB(r, g, b);
}

int vancho() {
   return iWidth;
}

int valto() {
   return iHeight;
}

void vredimensiona(int ample, int alt) {
   iWidth = ample;
   iHeight = alt;
   int w, h;
   frame_real(iWidth, iHeight, w, h);
   SetWindowPos(hWnd, NULL, 0, 0, w, h, SWP_NOMOVE);
   newMemDC(w, h);
}

void vcierra() {
  PostMessage(hWnd, WM_CLOSE, 0, 0);
}

} // namespace miniwin
