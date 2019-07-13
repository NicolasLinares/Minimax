CC := g++
CFLAGS := -w 
LINKER_FLAGS =  `sdl2-config --cflags --libs`

test:	
	make build
	make run	

build:
	$(CC) $(CFLAGS) -o programa main.cpp Pantalla.cpp Arbol.cpp Nodo.cpp Tabla3R.cpp $(LINKER_FLAGS)  

run:
	./programa
	
