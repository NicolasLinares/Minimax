#include "Pantalla.h"
#include <stdlib.h>
#include <signal.h>
#include <math.h>

#include <iostream>
using namespace std;

// TIPOS PÚBLICOS

// La estructura Imagen es la implementación interna del TDA Imagen ofrecido de forma pública por la biblioteca.
struct Imagen
{
    SDL_Surface *surface;
};


// TIPOS PRIVADOS

// El tipo Pantalla (que es privado al módulo de la biblioteca) representa todos los datos que mantienen el estado de los distintos componentes que permiten dibujar en la pantalla gráfica. Desde la ventana hasta los colores usados para dibujar las líneas o rellenar las figuras, pasando por las texturas y mecanismos de renderizado utilizados.
typedef struct Pantalla
{
    int width;
    int height;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Color strokeColor, fillColor;
	int active;
} Pantalla;


// VARIABLES PRIVADAS

// La variable pantalla (que es privada al módulo de la biblioteca) mantiene el estado de la ventana donde se mostrarán todas las primitivas gráficas que se lleven a cabo.
static Pantalla pantalla;


// FUNCIONES PRIVADAS

// Función privada para gestión de errores SDL.
void Pantalla_Error()
{
    cout << SDL_GetError() << endl;
    exit(EXIT_FAILURE);
}

// Esta función privada se encarga de capturar el evento de cierre de la ventana
static int manejador(void *userdata, const SDL_Event *event)
{
    if ( (event->type == SDL_APP_TERMINATING || event->type == SDL_QUIT) && pantalla.active )
    {
		pantalla.active = 0;
        return(0);
    }
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_CLOSE)
	{
		pantalla.active = 0;
		return(0);
	}
    return(1);
}


// FUNCIONES PÚBLICAS

void Pantalla_Crea(char titulo[], int w, int h)
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) Pantalla_Error();

	// Crea la ventana donde se dibujará
	pantalla.window = SDL_CreateWindow(titulo, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);
    if (pantalla.window==NULL) Pantalla_Error();

    // Obtenemos la superficie asociada a la ventana
    pantalla.surface = SDL_GetWindowSurface(pantalla.window);
    if (pantalla.surface==NULL) Pantalla_Error();

	// Crea el renderizador que utilizaremos para dibujar en la superficie
	pantalla.renderer = SDL_CreateSoftwareRenderer(pantalla.surface);
	if (pantalla.renderer==NULL) Pantalla_Error();

	// Activamos el modo de dibujo con transparencias en el renderizador
    if (SDL_SetRenderDrawBlendMode(pantalla.renderer, SDL_BLENDMODE_BLEND)==-1) Pantalla_Error();

	pantalla.strokeColor.r = 0;
	pantalla.strokeColor.g = 0;
	pantalla.strokeColor.b = 0;
	pantalla.strokeColor.a = 255;

	pantalla.fillColor.r = 255;
	pantalla.fillColor.g = 255;
	pantalla.fillColor.b = 255;
	pantalla.fillColor.a = 255;
    pantalla.width = w;
    pantalla.height = h;

	pantalla.active = 1;
	SDL_SetEventFilter((SDL_EventFilter)manejador, NULL);
}

void Pantalla_Libera()
{
	pantalla.active = 0;
    SDL_DestroyWindow(pantalla.window);
    SDL_DestroyRenderer(pantalla.renderer);
	SDL_Quit();
}

int Pantalla_Anchura()
{
    return pantalla.width;
}

int Pantalla_Altura()
{
    return pantalla.height;
}

int Pantalla_Activa()
{
    SDL_PumpEvents();
	return pantalla.active;
}

void Pantalla_Espera(int ms)
{
	 SDL_Delay(ms);
}

void Pantalla_Actualiza()
{
    if (SDL_UpdateWindowSurface(pantalla.window)==-1) Pantalla_Error();
}

int Pantalla_TeclaPulsada(SDL_Scancode scancode) //SDL_Keycode keycode)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	return state[scancode];
}

int Pantalla_RatonBotonPulsado(int boton)
{
    int estado = SDL_GetMouseState(NULL, NULL);
    return estado & SDL_BUTTON(boton);
}

void Pantalla_RatonCoordenadas(int *x, int *y)
{
    SDL_GetMouseState(x, y);
}

void Pantalla_ColorTrazo(int r, int g, int b, int a)
{
	pantalla.strokeColor.r = r;
	pantalla.strokeColor.g = g;
	pantalla.strokeColor.b = b;
	pantalla.strokeColor.a = a;
}

void Pantalla_ColorRelleno(int r, int g, int b, int a)
{
	pantalla.fillColor.r = r;
	pantalla.fillColor.g = g;
	pantalla.fillColor.b = b;
	pantalla.fillColor.a = a;
}

void Pantalla_DibujaRellenoFondo(int r, int g, int b, int a)
{
	if (SDL_SetRenderDrawColor(pantalla.renderer, r,g,b, a)==-1) Pantalla_Error();
	if (SDL_RenderClear(pantalla.renderer)==-1) Pantalla_Error();
}

void Pantalla_DibujaPunto(double x, double y)
{
	if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) Pantalla_Error();
	if (SDL_RenderDrawPoint(pantalla.renderer, x,y)==-1) Pantalla_Error();
}

void Pantalla_DibujaLinea(double x1, double y1, double x2, double y2)
{
	if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) Pantalla_Error();
	if (SDL_RenderDrawLine(pantalla.renderer, x1,y1, x2,y2)==-1) Pantalla_Error();
}

void Pantalla_DibujaTriangulo(double x1, double y1, double x2, double y2, double x3, double y3)
{
    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.fillColor.r, pantalla.fillColor.g, pantalla.fillColor.b, pantalla.fillColor.a)==-1) Pantalla_Error();
    // Reordenar puntos para que el 1 sea el más alto y poder dibujar líneas horizontales desde 1 hacia 2 y 3
    if (y1 > y2)
    {
        double t = x1;
        x1 = x2;
        x2 = t;
        t = y1;
        y1 = y2;
        y2 = t;
    }
    if (y2 > y3)
    {
        double t = x2;
        x2 = x3;
        x3 = t;
        t = y2;
        y2 = y3;
        y3 = t;
        if (y1 > y2)
        {
            double t = x1;
            x1 = x2;
            x2 = t;
            t = y1;
            y1 = y2;
            y2 = t;
        }
    }
    double xl = x1; // x izquierda
    double xr = x1;
    double dxl = (x2-x1)/(y2-y1); // Incremento iquierdo
    double dxr = (x3-x1)/(y3-y1);
    int y = y1; // altura inicial
    // Primera mitad del triángulo
    y2 = floor(y2); // Para no pasarnos de la mitad exacta en pixels
    while ( y < y2 )
    {
        if (SDL_RenderDrawLine(pantalla.renderer,xl, y, xr, y)==-1) Pantalla_Error();
        xl += dxl;
        xr += dxr;
        y = y + 1;
    }
    xl = x2;
    // Segunda mitad del triángulo
    dxl = (x3-xl)/(y3-y);
    dxr = (x3-xr)/(y3-y);
    while ( y < y3 )
    {
        if (SDL_RenderDrawLine(pantalla.renderer,xl, y, xr, y)==-1) Pantalla_Error();
        xl += dxl;
        xr += dxr;
        y = y + 1;
    }
    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) Pantalla_Error();
    if (SDL_RenderDrawLine(pantalla.renderer, x1,y1, x2,y2)==-1) Pantalla_Error();
    if (SDL_RenderDrawLine(pantalla.renderer, x2,y2, x3,y3)==-1) Pantalla_Error();
    if (SDL_RenderDrawLine(pantalla.renderer, x3,y3, x1,y1)==-1) Pantalla_Error();
}

void Pantalla_DibujaRectangulo(double x, double y, double w, double h)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.fillColor.r, pantalla.fillColor.g, pantalla.fillColor.b, pantalla.fillColor.a)==-1) Pantalla_Error();
    if (SDL_RenderFillRect(pantalla.renderer, &r)==-1) Pantalla_Error();
	if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) Pantalla_Error();
	if (SDL_RenderDrawRect(pantalla.renderer, &r)==-1) Pantalla_Error();
}

void Pantalla_DibujaCirculo(double x, double y, double r)
{
    int dy;
    r=round(r);
	// Dibujar círculo
    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.fillColor.r, pantalla.fillColor.g, pantalla.fillColor.b, pantalla.fillColor.a)==-1) Pantalla_Error();
    for (dy = 0; dy <= r; dy++)
    {
        double dx = floor(sqrt((2.0 *r * dy) - (double)(dy * dy)));
        for (int px = x - dx; px <= x + dx; px++)
        {
            if (SDL_RenderDrawPoint(pantalla.renderer, px,y + r - dy)==-1) Pantalla_Error();
            if (SDL_RenderDrawPoint(pantalla.renderer, px,y - r + dy-1)==-1) Pantalla_Error();
         }
    }
	// Dibujar circunferencia
    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) Pantalla_Error();
    double error = -r;
    double px = r - 0.5;
    double py = 0.5;
    double cx = x - 0.5;
    double cy = y - 0.5;

    while (px >= py)
    {
        if (SDL_RenderDrawPoint(pantalla.renderer, cx + px, cy + py)==-1) Pantalla_Error();
        if (SDL_RenderDrawPoint(pantalla.renderer, cx + py, cy + px)==-1) Pantalla_Error();

        if (px != 0)
        {
            if (SDL_RenderDrawPoint(pantalla.renderer, cx - px, cy + py)==-1) Pantalla_Error();
            if (SDL_RenderDrawPoint(pantalla.renderer, cx + py, cy - px)==-1) Pantalla_Error();
        }

        if (py != 0)
        {
            if (SDL_RenderDrawPoint(pantalla.renderer, cx + px, cy - py)==-1) Pantalla_Error();
            if (SDL_RenderDrawPoint(pantalla.renderer, cx - py, cy + px)==-1) Pantalla_Error();
        }

        if (px != 0 && py != 0)
        {
            if (SDL_RenderDrawPoint(pantalla.renderer, cx - px, cy - py)==-1) Pantalla_Error();
            if (SDL_RenderDrawPoint(pantalla.renderer, cx - py, cy - px)==-1) Pantalla_Error();
        }

        error += py;
        ++py;
        error += py;

        if (error >= 0)
        {
            --px;
            error -= px;
            error -= px;
        }
    }
}


static unsigned short int letters[95][13] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},// space :32
    {0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},// ! :33
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36},
    {0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18},
    {0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70},
    {0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e},
    {0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c},
    {0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30},
    {0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00},
    {0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03},
    {0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c},
    {0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e},
    {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e},
    {0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c},
    {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
    {0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
    {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
    {0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06},
    {0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60},
    {0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e},
    {0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
    {0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
    {0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
    {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
    {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
    {0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
    {0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
    {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
    {0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},
    {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},
    {0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
    {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3,0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
    {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff},
    {0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c},
    {0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60},
    {0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18},
    {0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70},
    {0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
    {0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03},
    {0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e},
    {0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0},
    {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00},
    {0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00},
    {0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0},
    {0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78},
    {0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00},
    {0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00},
    {0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00},
    {0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
    {0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00}  // :126
};

void Pantalla_DibujaTexto(char *st, double x, double y)
{
    int i = 0;
    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) Pantalla_Error();
    while (st[i])
    {
        int letra = st[i]-' ';
        for ( int fila = 0 ; fila < 13 ; fila++ )
        {
            int mascara = 128;
            for ( int columna = 0 ; columna < 8 ; columna++ )
            {
                if (mascara & letters[letra][fila])
                {
                    if (SDL_RenderDrawPoint(pantalla.renderer, x+i*9+columna,y+13-fila)==-1) Pantalla_Error();
                }
                mascara /= 2;
            }
        }
        i++;
    }
}

void Pantalla_DibujaImagen(Imagen *imagen, double x, double y, double w, double h)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
    if (SDL_BlitScaled(imagen->surface, NULL, pantalla.surface, &dest)==-1) Pantalla_Error();
}

double Pantalla_ImagenAnchura(Imagen imagen)
{
    return imagen.surface->w;
}

double Pantalla_ImagenAltura(Imagen imagen)
{
    return imagen.surface->h;
}

void Pantalla_ImagenLibera(Imagen *imagen)
{
    SDL_FreeSurface(imagen->surface);
    delete imagen;
    //free(imagen);
}

void Pantalla_LimpiarRender(){
    SDL_RenderClear(pantalla.renderer);
}


Imagen * Pantalla_ImagenLee(char *fichero, int transparencia)
{
    SDL_Surface *surface = SDL_LoadBMP(fichero);
    if (surface == NULL) return NULL;

    if (transparencia)
    {
        if (surface->format->palette)
        {
            if (SDL_SetColorKey(surface, SDL_TRUE, *(Uint8 *) surface->pixels)==-1) Pantalla_Error();
        }
        else
        {
            switch (surface->format->BitsPerPixel)
            {
                case 15: if (SDL_SetColorKey(surface, SDL_TRUE, (*(Uint16 *) surface->pixels) & 0x00007FFF)==-1) Pantalla_Error();
                         break;
                case 16: if (SDL_SetColorKey(surface, SDL_TRUE, *(Uint16 *) surface->pixels)==-1) Pantalla_Error();
                         break;
                case 24: if (SDL_SetColorKey(surface, SDL_TRUE, (*(Uint32 *) surface->pixels) & 0x00FFFFFF)==-1) Pantalla_Error();
                         break;
                case 32: if (SDL_SetColorKey(surface, SDL_TRUE, *(Uint32 *) surface->pixels)==-1) Pantalla_Error();
                         break;
            }
        }
    }

    Imagen *img = (Imagen *)calloc(1, sizeof(struct Imagen));

    // Guardamos la superficie pero en convirtiÃ©ndola primero al formato que hemos usado para crear la ventana
    SDL_PixelFormat *pixelformat = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
    if (pixelformat==NULL) Pantalla_Error();

    img->surface = SDL_ConvertSurface(surface, pixelformat, 0);
    if (img->surface==NULL) Pantalla_Error();

    SDL_FreeFormat(pixelformat);

    // Liberamos la superficie original
    SDL_FreeSurface(surface);
    return img;
}
