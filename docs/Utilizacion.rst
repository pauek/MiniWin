
Funciones
=========

MiniWin es *super*-simple: solo es un miniconjunto de funciones. Para usar
MiniWin solamente hay que seguir 2 pasos importantes:

- Poner arriba del programa principal::

    #include "miniwin.h"
    using namespace miniwin;
  
  Fíjate en que hay comillas dobles y no ángulos alrededor de
  ``miniwin.h`` en el ``#include``. El ``using namespace miniwin`` te
  será familiar por su equivalente con ``std``.

- Hacer la función ``main`` así::

     int main() {
       return 0;
     }

  Es decir, sin parámetros y devolviendo ``int``. El ``return 0`` es
  obligatorio.

Aparte de eso se trata de utilizar las funciones que se comentan a
continuación.

Control de la ventana
---------------------

En MiniWin, al ejecutar el programa principal siempre se creará una
sola ventana, a la que nos referiremos como "la ventana". Algunas
funciones utilizan coordenadas en esta ventana. Las coordenadas son un
par ordenado de valores, donde el primer valor es *x* y el segundo es
*y*:

- La esquina superior-derecha es el orígen, con coordenadas (0,0).

- A medida que nos desplazamos a la derecha la coordenada *x* crece.

- A medida que nos desplazamos hacia abajo la coordenada *y* crece.

El diagrama sería el siguiente:

.. image:: _static/coords.png

Hay 3 funciones relacionadas con las dimensiones de la ventana.

.. cpp:function:: void vredimensiona(int ancho, int alto)

   Cambia las dimensiones (en píxeles) de la ventana. El primer
   parámetro indica la anchura y el segundo la altura, ambos son
   enteros. Por ejemplo::
 
      vredimensiona(800, 600);

   cambiará las dimensiones de la ventana a 800 por 600 píxeles.

.. cpp:function:: int vancho()

   Averigua el ancho de la ventana en píxeles, devolviendo un entero.
   Por ejemplo, el siguiente código utiliza la función :cpp:func:`vancho`::

      int a = vancho();
      if (a > 500) {
        cout << "El ancho de la ventana es mayor que 500" << endl;
      }

.. cpp:function:: int valto()

   Averigua la altura de la ventana en píxeles, devolviendo un
   entero. Es similar a :cpp:func:`vancho`.

.. cpp:function:: void vcierra()

   La acción :cpp:func:`vcierra` cierra la ventana y termina el
   programa.  Si no se llama esta función, cuanda acaba la función
   ``main`` la ventana se queda abierta mostrando el dibujo que
   hayamos hecho, y hay que cerrarla manualmente.


Pintar en la ventana
--------------------

Para pintar en la ventana hay que utilizar alguna de las acciones
:cpp:func:`linea`, :cpp:func:`rectangulo`, :cpp:func:`circulo`, etc.
y luego hay que invocar la acción :cpp:func:`refresca`. Esencialmente,
todo lo que se pinta se acumula en un "buffer" y luego la acción
:cpp:func:`refresca` muestra lo que se ha pintado en la ventana. Esto
permite pintar muchas cosas y luego refrescar solo una vez, que es
importante cuando se hacen juegos.

Para cambiar el color con el que se pinta, hay que llamar a la función
:cpp:func:`color` antes de pintar, es decir, todo lo que se pinta
después de la instrucción::
  
  color(ROJO);

saldrá en color rojo.

Por ejemplo, el siguiente programa

.. literalinclude:: _src/ex1.cpp

muestra el siguiente dibujo:

.. image:: _static/ex1.png

En MiniWin disponemos de las siguiente acciones para pintar objetos:

.. cpp:function:: void linea(float x_ini, float y_ini, float x_fin, float y_fin)

   Dibuja una línea desde el punto (*izq*, *arr*), o sea
   *izquierda-arriba* hasta el punto (*der*, *aba*) o sea,
   *derecha-abajo*.

   Dos líneas paralelas son, por ejemplo::

      linea(0,  0, 100,  0);
      linea(0, 10, 100, 10);

.. cpp:function:: void rectangulo(float izq, float arr, float der, float aba)

   Dibuja el borde de un rectángulo con coordenadas horizontales (las
   *x) *izq* y *der* y verticales *arr* y *aba*.
 
.. cpp:function:: void rectangulo_lleno(float izq, float arr, float der, float aba)

   Dibuja un rectángulo relleno con coordenadas horizontales (las
   *x) *izq* y *der* y verticales *arr* y *aba*.

.. cpp:function:: void circulo(float x_cen, float y_cen, float radio)

   Dibuja una circumferencia con el centro en (*x_cen*, *y_cen*) y con un
   radio de valor *radio*. Si llamamos::

      circulo(50, 100, 20);

   Aparecerá un círculo de radio 20 con centro en el punto (50, 100)

.. cpp:function:: void circulo_lleno(float x_cen, float y_cen, float radi)

   Dibuja un círculo (relleno) con el centro en (*x_cen*, *y_cen*) y
   con un radio de valor *radio*.

.. cpp:function:: void texto(float x, float y, const std::string& texto)

   Pinta un texto a partir de la posición (*x*, *y*) con el contenido
   del parámetro *texto*.





