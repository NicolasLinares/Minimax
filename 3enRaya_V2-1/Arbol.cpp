#include "Arbol.h"
#include <iostream>
using namespace std;

const int INFINITO = 99999;
//-------------------------CONSTRUCTOR-------------------------
Arbol::Arbol(Tabla3R * tabla) {
	raiz = NULL;

    //insertamos los nodos: el numero de nodos es proporcional al numero de huecos libres en el tablero

    int turno = 1; //turno 1 para MAX, turno 2 para MIN
	raiz = new Nodo(tabla, turno); //tabla es la tabla actual, sin insertar nuevos elementos
    raiz->inicializarHijos();

    crearArbolMinimax(raiz);  //inserta las posibles jugadas de MAX y de MIN

    //cout << "----------PREORDEN-----------" <<  endl;
    //recorridoPreorden(raiz);
}

//-------------------------DESTRUCTOR-------------------------
Arbol::~Arbol() {
    delete raiz;
}


//-------------------------METODOS-------------------------


void Arbol::recorridoPreorden(Nodo * n){

    if (n != NULL) {
        
        n->getTabla()->mostrarTabla();
        cout << "Beneficio: " << n->getBeneficio() << endl;
        cout << "Hijos: " << n->getNumeroHijos() << endl;
        cout << "----------------------" <<  endl;

        for (int i = 0; i < n->getTabla()->getHuecosLibres(); i++)
            recorridoPreorden(n->getHijo(i));
    }
    
}


// Va a crear el arbol minimax
void Arbol::crearArbolMinimax(Nodo * raiz){

    // Cada nodo representa una jugada para MAX, por lo que vamos a recorrer la tabla actual
    // para ver que huecos tiene libres MAX para marcar y crear una tabla nueva con cada una,
    // que sera insertada como un nuevo nodo hijo y así sucesivamente

    Tabla3R * tablaPadre = raiz->getTabla();


    if (tablaPadre->getHuecosLibres() > 0){

        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (tablaPadre->getElemento(i,j) == 'H'){ // H es el valor de inicializacion que indica que hay un hueco, por tanto, si hay hueco creamos su nodo


                    // Los turnos se van alternando en cada nivel del arbol y 
                    // permiten saber qué marca usar en un determinado nivel
                    char m;
                    int turno = raiz->getTurno();
                    if (turno == 1) {
                        m = 'X';
                        turno = 2; //cambia para que el turno siguiente sea 'O'
                    } else {
                        m = 'O';
                        turno = 1; //cambia para que el turno siguiente sea 'X'
                    }

                    // Creamos el nuevo nodo y lo añadimos como hijo del nodo anterior

                    Nodo * nuevoNodo = new Nodo(tablaPadre, turno);
                    nuevoNodo->marcarTabla(i, j, m); //en este paso comprueba si ha habido un ganador por marcar esa casilla
                    nuevoNodo->inicializarHijos();
                    raiz->setHijo(nuevoNodo);

                    // solo si no hay ganador todavía y quedan huecos por comprobar se continuará creando hijos (generando el arbol)
                    if (nuevoNodo->getBeneficio() == 0 && tablaPadre->getHuecosLibres() > 0)    
                        crearArbolMinimax(nuevoNodo);
                }
            }
        }
    }
    
}

Nodo * Arbol::getJugada(){
	 return minimax();
}

Nodo * Arbol::minimax(){

    // CALCULA LA POSICION DEL MEJOR CANDIDATO
    MAX_valor(raiz);


    // OBTENEMOS AL CANDIDATO
    int candidato = 0;
    int v = -INFINITO;
    int vaux = v;


    // AQUÍ SE PUEDE CREAR ALEATORIEDAD SI EXISTEN VARIOS HIJOS CON BENEFICIO +1

    // el candidato será el hijo de la raiz con el beneficio a +1
    for (int i = 0; i < raiz->getNumeroHijos() ; i++){
        vaux = maximo(v, raiz->getHijo(i)->getBeneficio());
        if (v < vaux){
            v = vaux;        
            candidato = i;
        }
        
    }

    return raiz->getHijo(candidato);  //nodo elegido como movimiento para MAX (debe ser un hijo de la raiz, ya que sería el movimiento que sigue a la tabla orginal)
}

int Arbol::MAX_valor(Nodo * n){

    int v = -INFINITO;

    for (int i = 0; i < n->getNumeroHijos() ; i++)
        v = maximo(v, MIN_valor(n->getHijo(i)));
    

    if (!n->isTerminal()) {
        n->setBeneficio(v + 1); // al sumarle +1  se van poniendo prioridades por nivel, de manera que no sea igual ganar ahora que después, si no que priorice ganar cuanto antes
        return v;
    }else
        return n->getBeneficio();
    
}

int Arbol::MIN_valor(Nodo * n){
// va a buscar de los hijos el que tiene menor beneficio, ya que corresponde a la profundidad
// de MIN. Este valor se almacena como beneficio del padre

    int v = INFINITO;

    for (int i = 0; i < n->getNumeroHijos() ; i++)
        v = minimo(v, MAX_valor(n->getHijo(i)));

    if (!n->isTerminal()) {
        n->setBeneficio(v - 1); // al restarle -1  se van poniendo prioridades por nivel, de manera que no sea igual ganar ahora que después, si no que priorice ganar cuanto antes
        return v;
    }else
        return n->getBeneficio();
}

int Arbol::maximo(int a, int b){
	if (a >= b) return a;
	else return b;
}

int Arbol::minimo(int a, int b){
	if (a <= b) return a;
	else return b;
}

