#include "Tabla3R.h"
#include "Arbol.h"
#include "Nodo.h"


#include <iostream>
using namespace std;



// TURNO DE LA IA (MAQUINA)
Tabla3R * jugarPartidaMAX(Tabla3R * tabla){

    cout << endl << "   Turno de la IA [X]" << endl << endl;
    Arbol * arbol = new Arbol(tabla);

    return arbol->getJugada()->getTabla(); // me devuelve el nodo que contiene la tabla con el movimiento mas favorable para MAX  
}


// TURNO DEL JUGADOR "HUMANO"
Tabla3R * jugarPartidaMIN(Tabla3R * tabla){

    cout << endl << "   *TU TURNO [O]*" << endl << endl;
    cout << "Elige la posicion que deseas marcar <fila [0,1,2]>  <columna [0,1,2]> :" ;

    int fila =0;
    int columna = 0;
    cin >> fila;
    cin >> columna;

    cout << endl << endl;
    tabla->marcarTabla(fila, columna, 'O');
    
    return tabla;
}


bool comprobarTabla(Tabla3R * tabla){

    if (tabla->comprobarJugada('X')){
        cout << endl << endl << "**************" << endl;
        cout << "* Gana la IA *" << endl;
        cout << "**************" << endl;
        return true;
    }else if (tabla->comprobarJugada('O')){
        cout << endl << endl << "**************" << endl;
        cout << "*  Tu ganas  *" << endl;
        cout << "**************" << endl;
        return true;
    }else if (tabla->getHuecosLibres() == 0){ // si no quedan mas huecos en la tabla es empate porque no ha dado un ganador
        cout << endl << endl << "**************" << endl;
        cout << "*   EMPATE   *" << endl;
        cout << "**************" << endl;
        return true;
    }

    return false;
}



int main(int argc, char **argv)
{
    Tabla3R * tabla = new Tabla3R();
    tabla->mostrarTabla();

    int turno = 1;

    while (!comprobarTabla(tabla)) {
        if (turno == 1){
            tabla = jugarPartidaMIN(tabla);
            turno = 2;
        }else{
            tabla = jugarPartidaMAX(tabla);
            turno = 1;
        }         

        tabla->mostrarTabla();   
    }

    delete tabla;
}
