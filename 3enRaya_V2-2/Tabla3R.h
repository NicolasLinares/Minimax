#ifndef TABLA3R_H
#define TABLA3R_H


class Tabla3R
{
    private:

        char  t[3][3];
        int huecos;


        bool comprobarFilas(int fila, char marca);
        bool comprobarColumnas(int columna, char marca);
        bool comprobarDiagonales(int fila, int columna, char marca);
        bool diagonal1(char marca);
        bool diagonal2(char marca);

    public:
        Tabla3R(void);

        void setElemento(int x, int y, char c);
        char getElemento(int x, int y);

        void setHuecosLibres(int h);
        int getHuecosLibres();

        void marcarTabla(int x, int y, char marca);
        bool isMarcada(int x, int y);

        int comprobarJugada(int fila, int columna, char marca);
        bool comprobarJugada(char marca);

        void clonarTabla(Tabla3R * t1, Tabla3R * t2); //copia t1 en t2
        
        void mostrarTabla();
};


#endif // TABLA3R_H
