
test:
	make build
	make clean



build: Nodo.o Arbol.o Tabla3R.o main.o
	g++ -o programa Nodo.o Arbol.o Tabla3R.o main.o

main.o: main.cpp Nodo.h Arbol.h Tabla3R.h
	g++ -c main.cpp
	
Nodo.o: Nodo.cpp Nodo.h Arbol.h Tabla3R.h 
	g++ -c Nodo.cpp

Arbol.o: Arbol.cpp Arbol.h Nodo.h Tabla3R.h 
	g++ -c Arbol.cpp

Tabla3R.o: Tabla3R.cpp Tabla3R.h Nodo.h Arbol.h 
	g++ -c Tabla3R.cpp



clean:
	rm *.o 
