#include "Arbol.h"
#include <iostream>
using namespace std;

const int INFINITO = 99999;
//-------------------------CONSTRUCTOR-------------------------
Arbol::Arbol(Tabla3R * tabla) {
	raiz = NULL;

    //insertamos los nodos: el numero de nodos es proporcional al numero de huecos libres en el tablero
	raiz = new Nodo();
    raiz->setParametros(tabla);//tabla es la tabla actual, sin insertar nuevos elementos
    crearArbolMinimax(tabla);  //inserta las posibles jugadas de MAX y de MIN

    //recorridoPreorden(raiz);
}

//-------------------------DESTRUCTOR-------------------------
Arbol::~Arbol() {
    delete raiz;
}


//-------------------------METODOS-------------------------


void Arbol::recorridoPreorden(Nodo * n){
    if (n != NULL) {
        cout << "------------------" <<  endl;
        n->getTabla()->mostrarTabla();
        cout << "Beneficio: " << n->getBeneficio() << endl;

        for (int i = 0; i < n->getTabla()->getHuecosLibres(); i++)
            recorridoPreorden(n->getHijo(i));
    }
}


// Va a crear el arbol minimax
void Arbol::crearArbolMinimax(Tabla3R * tablaRaiz){

    // Cada nodo representa una jugada para MAX, por lo que vamos a recorrer la tabla actual
    // para ver que huecos tiene libres MAX para marcar y crear una tabla nueva con cada una,
    // que sera insertada como un nuevo nodo hijo

    int posh = 0; // lleva la cuenta de los hijos del nodo raiz
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (tablaRaiz->getElemento(i,j) == 'H'){ // H es el valor de inicializacion que indica que hay un hueco, por tanto, si hay hueco creamos su nodo

                //---MAX---
                Nodo * n = new Nodo();
                n->setTablaMAX(tablaRaiz, i, j, 'X');
                raiz->setHijo(posh, n);

                // Ahora vamos a insertar todos los hijos del nodo que acabamos de crear
                int posh2 = 0;  // lleva la cuenta de los hijos de cada nodo hijo de la raiz

                Tabla3R * nuevaTabla = n->getTabla();

                //---MIN---
                for (int k = 0; k < 3; k++){
                    for (int l = 0; l < 3; l++){
                        if (nuevaTabla->getElemento(k,l) == 'H' ){

                            Nodo * hijo = raiz->getHijo(posh);
                            Nodo * terminal = new Nodo();

                            terminal->setTablaMAX(nuevaTabla, k, l, 'O');
                            terminal->setUtilidad();
                            hijo->setHijo(posh2, terminal);

                            posh2++;
                        }
                    }
                }
                posh++;
            }
        }
    }
}

Nodo * Arbol::getMejorNodo(){
	 return minimax();
}

Nodo * Arbol::minimax(){

    // CALCULA LA POSICION DEL MEJOR CANDIDATO
    int candidato = MAX_valor(raiz);

    Nodo * movimiento = raiz->getHijo(candidato);

    return movimiento; //nodo elegido como movimiento para MAX
}

int Arbol::MAX_valor(Nodo * raiz){

    int candidato =0; // me va a devolver la posicion del nodo con maximo valor
    int v = -INFINITO;
    int vaux = v;

    // recorremos buscando el valor maximo y apuntando en que posicion esta
    for (int i = 0; i < raiz->getTabla()->getHuecosLibres()  && raiz->getHijo(i) != NULL; i++){

        vaux = maximo(v, MIN_valor(raiz->getHijo(i)));
        if (v < vaux){
            candidato = i;
            v = vaux;
        }
    }

    return candidato;
}

int Arbol::MIN_valor(Nodo * n){
// va a buscar de los hijos el que tiene menor beneficio, ya que corresponde a la profundidad
// de MIN. Este valor se almacena como beneficio del padre

    int v = INFINITO;


    for (int i = 0; i < n->getTabla()->getHuecosLibres()   && n->getHijo(i) != NULL ; i++){
         v = minimo(v, n->getHijo(i)->getBeneficio());
    }

    n->setBeneficio(v); // se almacena en el padre

    return v;

}

int Arbol::maximo(int a, int b){
	if (a >= b) return a;
	else return b;
}

int Arbol::minimo(int a, int b){
	if (a <= b) return a;
	else return b;
}

