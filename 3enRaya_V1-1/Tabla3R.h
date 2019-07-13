#ifndef TABLA3R_H
#define TABLA3R_H


class Tabla3R
{
    private:

        char  t[3][3];
        int huecos;

    public:
        Tabla3R(void);

        void setElemento(int x, int y, char c);
        char getElemento(int x, int y);

        void setHuecosLibres(int h);
        int getHuecosLibres();

        void marcarTabla(int x, int y, char marca);
	bool isMarcada(int x, int y);

        void clonarTabla(Tabla3R * t1, Tabla3R * t2); //copia t1 en t2
        void mostrarTabla();

        int calcularLineas(char marca);
        bool getGanador(char marca);

};


#endif // TABLA3R_H
