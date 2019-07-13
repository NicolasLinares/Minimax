#ifndef NODO_H
#define NODO_H

#include "Tabla3R.h"

#include <iostream>
using namespace std;


class Nodo {

	private:

		int beneficio;  // beneficio usado para saber si sera escogido como opcion
        Tabla3R * tabla;
        Nodo * hijos[9]; // poniendo el 9 ha dejado de fallar en cuanto a memoria basura

	public:

		Nodo();
		~Nodo();

        void setParametros(Tabla3R * tab);

        void setTablaMAX(Tabla3R * tablaRaiz, int fila, int columna, char marca);

        void setHijo(int i, Nodo *n);
        Nodo * getHijo(int i);
        Tabla3R * getTabla();

        int getBeneficio();
        void setBeneficio(int b);

        bool isTerminal();
        void setUtilidad();

};

#endif // NODO_H
