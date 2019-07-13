#include "Tabla3R.h"
#include <iostream>
using namespace std;

//-------------------------CONSTRUCTOR-------------------------
Tabla3R::Tabla3R(void){
    huecos = 9;


    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            t[i][j] = 'H';
        }
    }

}

//-------------------------METODOS-------------------------

void  Tabla3R::setElemento(int x, int y, char c){
    t[x][y] = c;
}

char Tabla3R::getElemento(int x, int y){
    return t[x][y];
}

void Tabla3R::setHuecosLibres(int h){
    huecos = h;
}

int Tabla3R::getHuecosLibres(){
    return huecos;
}

void Tabla3R::marcarTabla(int x, int y, char marca){
    if (huecos > 0)
        huecos--;
    else
        huecos = 0;

    t[x][y] = marca;
}

bool Tabla3R::isMarcada(int x, int y){
    return t[x][y] != 'H';
}

void Tabla3R::clonarTabla(Tabla3R * t1, Tabla3R * t2){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){

                t2->setElemento(i,j, t1->getElemento(i,j));
        }
    }

    t2->setHuecosLibres(t1->getHuecosLibres());
}


void Tabla3R::mostrarTabla(){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            cout << " " << t[i][j];
        }
        cout << endl;
    }
}


// calcula el numero de lineas que podrian haber para la marca indicada
// este valor se necesita para calcular el beneficio en minimax
int Tabla3R::calcularLineas(char marca){

    int n3enraya = 0; // numero de 3 en rayas que se pueden formar en el tablero actual para la marca indicada

//comprobamos filas
    for (int i = 0; i < 3; i++){
        int j = 0;
        while (j < 3 && (t[i][j] == marca || t[i][j] == 'H')) {   //recorremos buscando huecos o valores iguales a marca
            j++;
        }
        if (j == 3)  //si llega al final, toda la fila es una posible solucion y la contamos
            n3enraya++;
    }

//comprobamos columnas
    for (int j = 0; j < 3; j++){
        int i = 0;
        while (i < 3 && (t[i][j] == marca || t[i][j] == 'H')) {
            i++;
        }
        if (i == 3)
            n3enraya++;
    }

//comprobamos diagonales

    // diagonal de izq a der
    int d = 0;
    while (d < 3 && (t[d][d] == marca || t[d][d] == 'H')) {
        d++;
    }
    if (d == 3)
            n3enraya++;

    // diagonal de der a izq
    int di = 0; // se encarga de la fila
    int dj = 2; //se encarga de la columna
    while (di < 3 && (t[di][dj] == marca || t[di][dj] == 'H')) {
        dj--;
        di++;
    }
    if (di == 3)
            n3enraya++;

    return n3enraya;
}



bool Tabla3R::comprobarFilas(int fila, char marca){

    int j = 0;
    while (j < 3 && t[fila][j] == marca) {   //recorremos buscando huecos o valores iguales a marca
        j++;
    }

    if (j == 3)  //si llega al final, toda la fila es una posible solucion y la contamos
        return true;
    else
        return false;
}

bool Tabla3R::comprobarColumnas(int columna, char marca){

    int i = 0;
    while (i < 3 && (t[i][columna] == marca )) {
        i++;
    }
    if (i == 3)
        return true;
    else
        return false;
    
}

bool Tabla3R::comprobarDiagonales(int fila, int columna, char marca){

    // solo en la casilla del centro se comprueban ambas diagonales
    if (fila == 1 && columna == 1)
        return diagonal1(marca) || diagonal2(marca);
        
    if (fila == columna)
        return diagonal1(marca);

    if (fila != columna)
        return diagonal2(marca);

    
    return false;
}

bool Tabla3R::diagonal1(char marca){
    // diagonal de izq a der

    //  X
    //      X
    //          X

    int d = 0;
    while (d < 3 && (t[d][d] == marca)) {
        d++;
    }
    if (d == 3)
        return true;    
    else
        return false;
}

bool Tabla3R::diagonal2(char marca){
    // diagonal de der a izq

    //          X
    //      X
    //  X

    int di = 0; // se encarga de la fila
    int dj = 2; // se encarga de la columna
    while (di < 3 && (t[di][dj] == marca )) {
        dj--;
        di++;
    }
    if (di == 3)
        return true;
    else
        return false;
}


// COMPRUEBA TODA LA TABLA PARA UNA MARCA
// Va a comprobar si existe una sola linea de 3 marcas (es decir si hay 3 en raya)
// utiliza el mismo mecanismo que el metodo anterior, pero ahora si hay un hueco no lo toma como valido.
bool Tabla3R::comprobarJugada(char marca){

//comprobamos filas
    for (int i = 0; i < 3; i++){
        if (comprobarFilas(i, marca))
            return true;
    }

//comprobamos columnas
    for (int j = 0; j < 3; j++){
        if (comprobarColumnas(j, marca))
            return true;
    }

//comprobamos diagonales
    return diagonal1(marca) || diagonal2(marca);
}

