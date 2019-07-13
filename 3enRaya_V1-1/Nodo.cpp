#include "Nodo.h"
#include <iostream>
using namespace std;

const int INFINITO = 99999;


//-------------------------CONSTRUCTOR-------------------------
Nodo::Nodo(){

    tabla = NULL;
    beneficio = 0;
}

//-------------------------DESTRUCTOR-------------------------
Nodo::~Nodo() {
    delete tabla;

    for (int i = 0; i < 9; i++){
        delete[] hijos[i];
    }
}

//-------------------------METODOS-------------------------

void Nodo::setParametros(Tabla3R * tab){

    tabla = tab;
    beneficio = 0;
    hijos[tab->getHuecosLibres()];  // El tamanio de la tabla que almacena el nodo (es decir el no de nodos hijos que tiene) viene condicionado por el no de huecos libres de la tabla actual

    for (int i = 0; i < tab->getHuecosLibres(); i++)
        hijos[i] = NULL;

}

void Nodo::setTablaMAX(Tabla3R * tablaRaiz, int fila, int columna, char marca ) {

    //creamos una copia de la tabla padre
    Tabla3R * nuevaTabla = new Tabla3R();
    tablaRaiz->clonarTabla(tablaRaiz, nuevaTabla);
    //le insertamos la marca en esa posicion
    nuevaTabla->marcarTabla(fila, columna, marca);

    setParametros(nuevaTabla);
}

Nodo * Nodo::getHijo(int i){
    return hijos[i];
}

void Nodo::setHijo(int i, Nodo * n){
    hijos[i] = n;
}

Tabla3R * Nodo::getTabla(){
    return tabla;
}


int Nodo::getBeneficio() {
    return beneficio;
}

void Nodo::setBeneficio(int b) {
    beneficio = b;
}

bool Nodo::isTerminal(){
    return (hijos[0] == NULL); // si uno de los hijos es NULL entonces es terminal
}


// HEURISTICA UTILIZADA PARA ESTE TIPO DE MINIMAX DONDE SOLO SE REALIZA UN ARBOL DE PROFUNDIDAD 2
void Nodo::setUtilidad(){

    // esta comprobacion es necesaria puesto que si no ponemos -INFINITO cuando el jugador MIN tiene posibilidad de ganar
    // entonces el jugador MAX (la IA) es posible que no lo tenga en cuenta y se le pase bloquear este movimiento ganador
    if (tabla->getGanador('O')) {
        setBeneficio(-INFINITO);
    }else
        setBeneficio(tabla->calcularLineas('X') - tabla->calcularLineas('O')); //MAX - MIN
}


