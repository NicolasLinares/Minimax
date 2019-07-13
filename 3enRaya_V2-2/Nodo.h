#ifndef NODO_H
#define NODO_H

#include "Tabla3R.h"

#include <iostream>
using namespace std;


class Nodo {

	private:

		int beneficio;  // beneficio usado para saber si sera escogido como opcion
        Tabla3R * tabla;
        Nodo * hijos[9]; // poniendo el 9 ha dejado de fallar en cuanto a memoria basura (9 es el maximo numero de hijos que puede tener)
        int nHijos;
        int turno; //sirve para que a la hora de crear el arbol se pueda recordar el turno al retroceder a otro nodo



	public:

		Nodo(Tabla3R * tab, int t);
		~Nodo();


        void inicializarHijos();
        
        void marcarTabla(int fila, int columna, char marca);

        void setHijo(Nodo *n);
        Nodo * getHijo(int i);
        Tabla3R * getTabla();

        int getTurno();
        int getNumeroHijos();

        int getBeneficio();
        void setBeneficio(int b);

        bool isTerminal();
};

#endif // NODO_H
