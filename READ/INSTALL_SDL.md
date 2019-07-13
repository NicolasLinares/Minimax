
################################ EN UNIX ################################



	*** Ejecutar los siguientes comandos en la raiz de la carpeta del proyecto que va a emplear la librería SDL




# ------------------------------------- DESCARGAR SDL -------------------------------------

	hg clone https://hg.libsdl.org/SDL SDL

# ------------------------------------- INSTALAR SDL -------------------------------------

	cd SDL
	mkdir build
	cd build
	../configure
	make
	sudo make install


# ------------------------------------- USAR LA LIBRERÍA SDL-------------------------------------

	# Para usar la librería gráfica SDL incluir en el programa "main" la librería '#include "SDL/include/SDL.h"'


# ------------------------------------- COMPILAR EL PROGRAMA -------------------------------------

	Usar el Makefile, ejecutando en la terminal "make"


# ------------------------------------- EJECUTAR PROGRAMA -------------------------------------

	Abrir una terminal y ejecutar:	./programa
