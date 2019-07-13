#ifndef ARBOL_H
#define ARBOL_H

#include "Tabla3R.h"
#include "Nodo.h"

#include <iostream>
using namespace std;

class Arbol {
	private:

		Nodo * raiz;

        void crearArbolMinimax(Tabla3R * tabla);

        void recorrerArbol();
        void recorridoPreorden(Nodo * n);

		Nodo * minimax(); // funcion MiniMax que calcula la combinacion del tablero que mejor le viene a MAX
        int MAX_valor(Nodo * n);
        int MIN_valor(Nodo * n);
        int maximo(int a, int b);
        int minimo(int a, int b);

	public:
		Arbol (Tabla3R * tabla);
		~Arbol ();

		Nodo * getMejorNodo();


};

#endif // ARBOL_H
