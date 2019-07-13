#include "Nodo.h"
#include <iostream>
using namespace std;

const int INFINITO = 99999;


//-------------------------CONSTRUCTOR-------------------------
Nodo::Nodo(Tabla3R * tab, int t){

    tabla = new Tabla3R();
    tab->clonarTabla(tab, tabla);

    nHijos = 0;
    turno = t;
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

void Nodo::inicializarHijos(){
    
    //inicializa los hijos (se debe hacer después de que la tabla se marca con la nueva jugada)
    hijos[tabla->getHuecosLibres()];  // El tamanio de la tabla que almacena el nodo (es decir el no de nodos hijos que tiene) viene condicionado por el no de huecos libres de la tabla actual
    for (int i = 0; i < tabla->getHuecosLibres(); i++)
        hijos[i] = NULL;
}


void Nodo::marcarTabla(int fila, int columna, char marca ) {

    //le insertamos la marca en esa posicion
    tabla->marcarTabla(fila, columna, marca);
    
    int valorJugada = tabla->comprobarJugada(fila,columna, marca); // esta comprobacion permite podar el arbol si MAX gana en ese nodo o si lo pierde, al que se le asigna un beneficio positivo
    
    setBeneficio(valorJugada);
}

Nodo * Nodo::getHijo(int i){
    return hijos[i];
}

void Nodo::setHijo(Nodo * n){
    
    int i = 0;

    if (i == 9) cout << "no quedan huecos en el nodo para añadir al hijo" << endl;

    while (i< 9){
        if (hijos[i] == NULL){
            hijos[i] = n;
            nHijos++;
            break;
        }
        i++;
    }
    
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

int Nodo::getTurno(){
    return turno;
}

int Nodo::getNumeroHijos(){
    return nHijos;
}

bool Nodo::isTerminal(){
    return (hijos[0] == NULL); // si uno de los hijos es NULL entonces es terminal
}
