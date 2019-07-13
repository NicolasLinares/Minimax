#include "Tabla3R.h"
#include "Arbol.h"
#include "Nodo.h"

#include "Pantalla.h"

#include <iostream>
using namespace std;


#define SIZE_X 400  //tamaño de la ventana eje X
#define SIZE_Y 400  //tamaño de la ventana eje Y


// ----- Las siguientes dos funciones, sirven de adaptador del tablero logico al dibujo del tablero de la pantalla -----


// Dibuja la marca que se encuentra en la posicion (x,y) del tablero logico 
// en el dibujo del tablero de la pantalla.
void setImagen(Imagen * img, int x, int y){

    int img_x = 0;
    int img_y = 0;

    // posicion eje Y
    if (x == 0)
        img_y = 60 + 10; // columna 0
    if (x == 1)
        img_y = 150 + 20; // columna 1
    if (x == 2)
        img_y = 260 + 10; // columna 2

    // posicion eje X
    if (y == 0)
        img_x = 60 + 10; // columna 0
    if (y == 1)
        img_x = 150 + 20; // columna 1
    if (y == 2)
        img_x = 260 + 10; // columna 2

    Pantalla_DibujaImagen(img, img_x, img_y, 70, 70);
}

// Segun las coordenadas del click del raton traduce del dibujo del tablero al tablero logico.
int determinarPosicionTablero(int pos){

    if (pos >= 60 && pos < 150)
        return 0; // columna 0

    if (pos >= 150 && pos < 260)
        return 1; // columna 1

    if (pos >= 260 && pos < 340)
        return 2; // columna 2
    
    return -1; // error no ha seleccionado ningun cuadrante del tablero
}



// Dibuja la tabla logica en la pantalla recorriendola desde el principio.
void dibujarJugada(Tabla3R * tabla, Imagen * img_circulo, Imagen * img_cruz){

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){

            if (tabla->getElemento(i, j) == 'X'){
                setImagen(img_cruz, i, j);
            }
                
            if (tabla->getElemento(i, j) == 'O'){
                setImagen(img_circulo, i, j);
            }
        }
    }
}


// TURNO DE LA IA (MAQUINA): aqui se lleva a cabo el algoritmo MINIMAX.
Tabla3R * jugarPartidaMAX(Tabla3R * tabla, Imagen *img){

    Arbol * arbol = new Arbol(tabla);
    return arbol->getJugada()->getTabla();  // me devuelve el nodo que contiene la tabla con el movimiento mas favorable para MAX  
}


// TURNO DEL JUGADOR "HUMANO", esta funcion se encarga de traducir la eleccion del humano para 
// en la pantalla a la tabla logica del juego.
Tabla3R * jugarPartidaMIN(Tabla3R * tabla, Imagen *img){
    
    int mouse_x = 0;
    int mouse_y = 0;

    int fila = 0;
    int columna = 0;

    //  Va a obtener las coordenadas de la casilla seleccionada para traducirlas al tablero logico
    while(Pantalla_Activa()){

        if (Pantalla_RatonBotonPulsado(SDL_BUTTON_LEFT)){
            
            Pantalla_RatonCoordenadas(&mouse_y, &mouse_x); // se debe invertir x e y porque SDL toma x=0 y y=0 la esquina superior izquierda, por lo que los valores crecen al reves
            fila = determinarPosicionTablero(mouse_x);
            columna = determinarPosicionTablero(mouse_y);

            if (fila != -1 && columna != -1 && !tabla->isMarcada(fila, columna)) // solo si es una casilla y si no está elegida ya
                break;
        }
    }
    
    tabla->marcarTabla(fila, columna, 'O');

    return tabla;
}

// Comprobara si la jugada realizada conlleva un ganador y en ese caso actualiza la ventana con un mensaje,
// para ello tiene que volver a cargar las imagenes y escribir encima los nuevos mensajes.
bool comprobarTabla(Tabla3R * tabla, Imagen * img_tablero, Imagen * img_circulo, Imagen * img_cruz ){

    Pantalla_LimpiarRender(); //limpia el renderer por si habia otras imagenes
    Pantalla_DibujaImagen(img_tablero, 0, 0, Pantalla_Anchura(), Pantalla_Altura());

    if (tabla->comprobarJugada('X')){
        Pantalla_ColorRelleno(200, 60, 60, 150);
        Pantalla_DibujaRectangulo(135, 10, 140, 30);
        Pantalla_DibujaTexto("HAS PERDIDO!", 150, 20);
        dibujarJugada(tabla, img_circulo, img_cruz); //vuelve a dibujarlo todo
        Pantalla_DibujaTexto("Click derecho para nueva partida...", 40, 375);
        Pantalla_Actualiza();
        return true;

    }else if (tabla->comprobarJugada('O')){
        Pantalla_ColorRelleno(60, 200, 60, 150);
        Pantalla_DibujaRectangulo(135, 10, 135, 30);
        Pantalla_DibujaTexto("HAS GANADO!", 155, 20);
        dibujarJugada(tabla, img_circulo, img_cruz);
        Pantalla_DibujaTexto("Click derecho para nueva partida...", 40, 375);
        Pantalla_Actualiza();
        return true;

    }else if (tabla->getHuecosLibres() == 0){ // si no quedan mas huecos en la tabla es empate porque no ha dado un ganador
        Pantalla_ColorRelleno(200, 200, 60, 150);
        Pantalla_DibujaRectangulo(135, 10, 135, 30);
        Pantalla_DibujaTexto("EMPATE!", 175, 20);
        dibujarJugada(tabla, img_circulo, img_cruz);
        Pantalla_DibujaTexto("Click derecho para nueva partida...", 40, 375);
        Pantalla_Actualiza();
        return true;
    }

    return false;
}



int main(int argc, char **argv)
{

    Pantalla_Crea("3 EN RAYA", 400, 400);


    Imagen *img_tablero = Pantalla_ImagenLee("Images/tablero.bmp", 0);
    Imagen *img_circulo = Pantalla_ImagenLee("Images/circulo.bmp", 50);
    Imagen *img_cruz = Pantalla_ImagenLee("Images/cruz.bmp", 50);
   
    while(Pantalla_Activa()){

        //------------ ELECCION DE TURNO ------------
        
        // Panel inicial para elegir quien empieza primero
        Pantalla_LimpiarRender(); //limpia el renderer por si habia otras imagenes
        Pantalla_ColorRelleno(255, 255, 150, 200);
        Pantalla_DibujaRectangulo(0, 0, Pantalla_Anchura(), Pantalla_Altura()/2);
        Pantalla_DibujaTexto("Click aqui para que empiece la IA", 55, 100);

        Pantalla_ColorRelleno(150, 150, 255, 200);
        Pantalla_DibujaRectangulo(0, Pantalla_Altura()/2, Pantalla_Anchura(), Pantalla_Altura()/2);
        Pantalla_DibujaTexto("Click aqui para empezar primero", 55, 300);

        Pantalla_Actualiza();


        int mouse_y = 0;
        int turno = 1;

        while(Pantalla_Activa()){

            if (Pantalla_RatonBotonPulsado(SDL_BUTTON_LEFT)){
                Pantalla_RatonCoordenadas(NULL, &mouse_y);
                if (mouse_y > Pantalla_Altura()/2) { 
                    turno = 1;  // turno del humano
                } else{
                    turno = 0;  // turno de la IA
                }

                Pantalla_Espera(500);
                break;
            }

            Pantalla_Espera(40);
        }

        

        //------------ COMIENZA EL JUEGO ------------

        Pantalla_LimpiarRender(); //limpia el renderer por si habia otras imagenes
        Pantalla_DibujaImagen(img_tablero, 0, 0, Pantalla_Anchura(), Pantalla_Altura());
        if (turno == 1) 
            Pantalla_DibujaTexto("- Tu turno -", 150, 20);
        else 
            Pantalla_DibujaTexto("- Turno de la IA -", 130, 20);
            
        Pantalla_Actualiza();

        Tabla3R * tabla = new Tabla3R();

        // bucle principal del juego
        while (Pantalla_Activa() && !comprobarTabla(tabla, img_tablero, img_circulo, img_cruz) ){
            
            // Logica del juego
            if (turno == 1){
                Pantalla_DibujaTexto("- Turno de la IA -", 130, 20);
                tabla = jugarPartidaMIN(tabla, img_circulo); // turno humano
                turno = 2;
            }else{
                Pantalla_DibujaTexto("- Tu turno -", 150, 20);
                tabla = jugarPartidaMAX(tabla, img_cruz); // turno maquina
                Pantalla_Espera(500);
                turno = 1;
                
            }         

            dibujarJugada(tabla, img_circulo, img_cruz);
            Pantalla_Actualiza();
        }

        //------------ ELECCION DE VOLVER A EMPEZAR ------------

        // para comenzar nueva partida al finalizar
        while (Pantalla_Activa()){
            if (Pantalla_RatonBotonPulsado(SDL_BUTTON_RIGHT))
                break;
            
            Pantalla_Espera(40);
        }
            
        delete tabla;
    }

    Pantalla_ImagenLibera(img_tablero);
    Pantalla_ImagenLibera(img_circulo);
    Pantalla_ImagenLibera(img_cruz);
    Pantalla_Libera();

    return 0;
}
