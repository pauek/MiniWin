
.. _instalacion:

Instalación
===========

Antes que nada: `descarga MiniWin <https://github.com/pauek/MiniWin/zipball/master>`_.

Verás que dentro del Zip hay dos ficheros: ``miniwin.h`` y
``miniwin.cpp``. Puedes ignorar todo lo demás. Estos dos ficheros
deben formar parte del proyecto que se está desarrollando. En C++, se
pueden hacer programas con más de un fichero fuente (si estás
empezando esto no es evidente). Entornos de programación como `Dev-C++
<http://www.bloodshed.net/devcpp.html>`_, `Code::Blocks
<http://www.codeblocks.org>`_ o `Geany <http://www.geany.org>`_
permiten compilar este tipo de proyectos.

Instalación Rápida para Code::Blocks
------------------------------------

Si usas Code::Blocks en Windows o Linux, descarga el proyecto
`HolaMiniWin-windows
<https://github.com/downloads/pauek/MiniWin/HolaMiniWin-windows.zip>`_
u `HolaMiniWin-linux
<https://github.com/downloads/pauek/MiniWin/HolaMiniWin-linux.tar.gz>`_,
abre el fichero ``HolaMiniWin.cbp`` con Code::Blocks y ya puedes
empezar trabajar.


Creación de un proyecto en Code::Blocks que use MiniWin
-------------------------------------------------------

Esta explicación también está `en vídeo
<https://www.youtube.com/watch?v=FHFz83xXyps>`_.

Para usar MiniWin en un proyecto en Code::Blocks sigue los siguientes
pasos:

- Crea un nuevo proyecto en Code::Blocks (menú "File - New -
  Project..."):

  .. image:: _static/codeblocks/paso1.png
     :align: center

- Escoge el tipo de proyecto como "Win32 GUI":

  .. image:: _static/codeblocks/paso2.png
     :align: center

- Escoge "Frame based" en el tipo de proyecto:

  .. image:: _static/codeblocks/paso3.png
     :align: center

- Pon un nombre al proyecto, como por ejemplo "MiProyecto", y luego
  escoge una carpeta donde ponerlo:

  .. image:: _static/codeblocks/paso4.png
     :align: center

- Marca las dos configuraciones de "Release" y "Debug" (típicamente
  saldrán marcadas por defecto):

  .. image:: _static/codeblocks/paso5.png
     :align: center

- Quita el fichero ``main.cpp`` del proyecto:

  .. image:: _static/codeblocks/paso6.png
     :align: center

- Descarga MiniWin y copia los ficheros ``miniwin.cpp`` y
  ``miniwin.h`` a la carpeta del proyecto.

  .. image:: _static/codeblocks/paso7.png
     :align: center

- Añade ficheros al proyecto:

  .. image:: _static/codeblocks/paso8.png
     :align: center

- Escoge ``miniwin.cpp`` y ``miniwin.h`` para añadirlos al proyecto:

  .. image:: _static/codeblocks/paso9.png
     :align: center

- Marca las configuraciones "Debug" y "Release" (normalmente salen
  marcadas por defecto):

  .. image:: _static/codeblocks/paso10.png
     :align: center

- Crea un nuevo fichero:

  .. image:: _static/codeblocks/paso11.png
     :align: center

- Escoge el tipo C++/source:

  .. image:: _static/codeblocks/paso12.png
     :align: center

- Escoge el lenguaje C++:

  .. image:: _static/codeblocks/paso13.png
     :align: center

- Escoge un nombre para el nuevo fichero (dentro de la misma carpeta
  del proyecto) y marca la opción "Add file to active project",
  marcando también "Debug" y "Release":

  .. image:: _static/codeblocks/paso14.png
     :align: center

- Escribe el programa principal y compílalo:

  .. image:: _static/codeblocks/paso15.png
     :align: center


Creación de un proyecto en Dev-C++ que use MiniWin
--------------------------------------------------

Para crear proyectos usando MiniWin en Dev-C++, debes hacer lo
siguiente:

- Creas un nuevo proyecto en Dev-C++, del tipo "Windows Application",
  y le pones un nombre como por ejemplo "MiJuego":

  .. image:: _static/devcpp1.png
     :align: center

- DevC++ te pedirá guardar el proyecto (un fichero ``.dev``) en un
  directorio. Crea un directorio especial para el proyecto (supongamos
  "MiJuego" también).

- Ahora aparecerá un fichero ``main.cpp`` en la pantalla principal de
  edición con código C++ escrito. Este fichero lo genera Dev-C++ y es
  estándar. Debes cerrar la ventana (observa que el fichero no está
  guardado, porque tiene un asterisco entre corchetes antes del nombre):

   .. image:: _static/devcpp2.png
     :align: center

- DevC++ preguntará si quieres guardar los cambios a ``main.cpp``. Dí
  que no:

   .. image:: _static/devcpp3.png
     :align: center

  El proyecto quedará vacío.

- Copia los ficheros ``miniwin.h`` y ``miniwin.cpp`` en el directorio
  "MiJuego" que has creado anteriormente.

- Añade al proyecto de Dev-C++ el fichero ``miniwin.cpp``. Esto se
  puede hacer clicando con el botón derecho el proyecto y luego
  seleccionando "Añadir al proyecto":

  .. image:: _static/devcpp4.png
     :align: center

- Haz el programa principal (la función ``main``) en un fichero aparte
  (por ejemplo: ``mijuego.cpp``), y lo añades también al proyecto
  (tendrás, entonces, 2 ficheros en el proyecto, el tuyo y
  ``miniwin.cpp``).

- En el fichero ``mijuego.cpp`` añades las líneas::

     #include "miniwin.h"
     using namespace miniwin;

  al principio de todo, tal como pones normalmente otros
  ``#include``\s o el ``using namespace std``.

Ahora puedes compilar el proyecto.

Crear un proyecto en Geany que usa MiniWin
------------------------------------------

Para crear un proyecto que use MiniWin en Geany hay que seguir los
siguientes pasos:

- Crea un directorio (por ejemplo "MiJuego").

- Copia los ficheros ``miniwin.h`` y ``miniwin.cpp`` al directorio
  "Mi Juego".

- Haz el programa principal (la función ``main``) en un fichero aparte
  (por ejemplo: ``mijuego.cpp``), y lo añades también al proyecto
  (tendrás, entonces, 2 ficheros en el proyecto, el tuyo y
  ``miniwin.cpp``).

- Crea un fichero ``Makefile`` en el directorio "MiJuego" (con Geany
  mismo) y escribes lo siguiente (puedes sustituir ``mijuego`` por el nombre
  que hayas escogido)::

     all: mijuego

     mijuego: miniwin.o mijuego.o
        g++ -o mijuego.exe miniwin.o mijuego.o -mwindows

Ahora, para compilar el proyecto, debes seleccionar la opción del menú
"Construir" que pone "Compilar", pero no la primera, sino la que
aparece justo al lado de "Mayúsc + F9" (o si quieres, presiona esa
combinación de teclas). Mira la ventana de mensajes para comprobar que
todo sale bien.
