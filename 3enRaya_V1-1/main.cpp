#include "Tabla3R.h"
#include "Arbol.h"
#include "Nodo.h"


#include <iostream>
using namespace std;



bool comprobarHuecosLibres(Tabla3R * tabla){
    if (tabla->getHuecosLibres() > 0) return true;
    else return false;
}

// TURNO DE LA IA (MAQUINA)
Tabla3R * jugarPartidaMAX(Tabla3R * tabla){

    cout << endl << "   Turno de la IA [X]" << endl << endl;
    Arbol * arbol = new Arbol(tabla);

    Nodo * nodo = arbol->getMejorNodo();
 
    return nodo->getTabla(); // me devuelve el nodo que contiene la tabla con el movimiento mas favorable para MAX 
}


// TURNO DEL JUGADOR "HUMANO"
Tabla3R * jugarPartidaMIN(Tabla3R * tabla){

    cout << endl << "   *TU TURNO [O]*" << endl << endl;
    
    int fila =0;
	int columna = 0;
	
    while (true) {
		cout << "Elige la posicion que deseas marcar <fila [0,1,2]>  <columna[0,1,2]> :" ;

		cin >> fila;
		cin >> columna;
		
		if (!tabla->isMarcada(fila, columna))
			break;
		else
			cout << "Esa casilla esta ocupada, elige otra..." << endl;
	}

    cout << endl << endl;
    tabla->marcarTabla(fila, columna, 'O');

    return tabla;
}


bool comprobarGanador(Tabla3R * tabla){

    if (tabla->getGanador('X')){
        cout << endl << endl << "**************" << endl;
        cout << "* Gana la IA *" << endl;
        cout << "**************" << endl;
        return true;
    }else if (tabla->getGanador('O')){
        cout << endl << endl << "**************" << endl;
        cout << "*  Tu ganas  *" << endl;
        cout << "**************" << endl;
        return true;
    }else if (!comprobarHuecosLibres(tabla)){ // si no quedan mas huecos en la tabla es empate porque no ha dado un ganador
        cout << endl << endl << "**************" << endl;
        cout << "*   EMPATE   *" << endl;
        cout << "**************" << endl;
        return true;
    }

    return false;
}



int main(int argc, char **argv)
{
	
	
	cout << "----------------------------  ELECCION DE TURNO  ----------------------------" << endl;
	cout << "Teclea [1] para ser el primero en jugar o [2] para que empiece la maquina: ";
	
	int turno = 1;
	cin >> turno;
	
	if (turno != 1 || turno != 2)
		turno = 2; // si se ha escrito mal le damos el turno a la maquina
		
	cout << "---------------------------------------------------------------------------" << endl;
	
    Tabla3R * tabla = new Tabla3R();

    while (!comprobarGanador(tabla)) {
		
		if (turno == 1){
			tabla = jugarPartidaMIN(tabla); // turno humano
			turno = 2;
		}else{
			tabla = jugarPartidaMAX(tabla); // turno maquina
			turno = 1;
		}        
		
		tabla->mostrarTabla(); 
		cout << "--------------------------" << endl;
    }

    delete tabla;
}
