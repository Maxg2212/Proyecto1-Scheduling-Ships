#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "guidef.h"

#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 600
#define RECT_SPEED 5
#define LEFT_LIMIT 1250
#define RIGHT_LIMIT 450

// Coordenadas fijas para los barcos
#define LEFT_START_X 350
#define LEFT_START_Y 230
#define RIGHT_START_X 1250
#define RIGHT_START_Y 370
#define SPACING 100 // Espacio entre barcos

// Tipos de barcos
typedef enum {
    NORMAL,
    PESQUERA,
    PATRULLA
} BarcoTipo;

// Estructura que representa un barco
typedef struct {
    BarcoTipo tipo;
    int x, y;   // Coordenadas
    int direction; // 1 para derecha, -1 para izquierda
    bool reached_limit; // Bandera para marcar si ha llegado al límite
} Barco;

// Función para dibujar el barco según su tipo
void drawBoat(SDL_Renderer *renderer, Barco barco) {
    SDL_Color color;
    switch (barco.tipo) {
        case NORMAL:
            color = (SDL_Color){255, 0, 0}; // Rojo
            break;
        case PESQUERA:
            color = (SDL_Color){0, 255, 0}; // Verde
            break;
        case PATRULLA:
            color = (SDL_Color){255, 165, 0}; // Naranja
            break;
    }

    // Dibuja el casco del barco
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_Rect boat_body = {barco.x, barco.y, 80, 30}; // Tamaño del barco
    SDL_RenderFillRect(renderer, &boat_body);

    // Dibuja la vela (triángulo)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Color amarillo
    SDL_Point points[4];
    points[0] = (SDL_Point){barco.x + 40, barco.y - 30}; // Punta de la vela
    points[1] = (SDL_Point){barco.x + 80, barco.y};       // Esquina inferior derecha
    points[2] = (SDL_Point){barco.x + 40, barco.y};       // Esquina inferior izquierda
    points[3] = points[0];                                // Cerrar el triángulo

    SDL_RenderDrawLines(renderer, points, 4);  // Dibuja el triángulo
}

// Función para actualizar la posición del barco, si no ha alcanzado su límite
void updateBoat(Barco *barco, int limit) {
    if (!barco->reached_limit) {
        barco->x += barco->direction * RECT_SPEED;

        // Si el barco ha alcanzado el límite, detener su movimiento
        if ((barco->direction == 1 && barco->x >= limit) ||
            (barco->direction == -1 && barco->x <= limit)) {
            barco->reached_limit = true;
        }
    }
}

// Dibujar el canal en el centro de la pantalla
void drawCanal(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Azul
    SDL_Rect canal = {600, 200, 600, 200};
    SDL_RenderFillRect(renderer, &canal);

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marrón
    SDL_Rect up_border = {600, 200, 600, 10}; // Borde arriba
    SDL_Rect down_border = {600, 400, 600, 10}; // Borde abajo
    SDL_RenderFillRect(renderer, &up_border);
    SDL_RenderFillRect(renderer, &down_border);
}

void start_gui() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Schedulers",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Lista de barcos que inician del lado izquierdo (coordenadas fijas en x e y)
    Barco barcos_izquierda[3] = {
        {NORMAL, LEFT_START_X, LEFT_START_Y, 1, false},      // Barco normal
        {PESQUERA, LEFT_START_X - SPACING, LEFT_START_Y, 1, false},    // Barco pesquero
        {PATRULLA, LEFT_START_X - 2 * SPACING, LEFT_START_Y, 1, false}     // Barco patrulla
    };
    int num_barcos_izquierda = 3;

    // Lista de barcos que inician del lado derecho (coordenadas fijas en x e y)
    Barco barcos_derecha[2] = {
        {NORMAL, RIGHT_START_X, RIGHT_START_Y, -1, false},   // Barco normal
        {NORMAL, RIGHT_START_X + SPACING, RIGHT_START_Y, -1, false}    // Barco normal
    };
    int num_barcos_derecha = 2;

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Limpiar el renderer con color celeste
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Celeste
        SDL_RenderClear(renderer);

        // Dibujar el canal
        drawCanal(renderer);

        // Dibujar y mover todos los barcos de la izquierda que no han alcanzado el límite
        for (int i = 0; i < num_barcos_izquierda; i++) {
            updateBoat(&barcos_izquierda[i], LEFT_LIMIT); // Mover el barco si no ha alcanzado el límite
            if (!barcos_izquierda[i].reached_limit) { // Solo dibujar si no ha alcanzado el límite
                drawBoat(renderer, barcos_izquierda[i]); // Dibujar el barco
            }
        }

        // Dibujar y mover todos los barcos de la derecha que no han alcanzado el límite
        for (int i = 0; i < num_barcos_derecha; i++) {
            updateBoat(&barcos_derecha[i], RIGHT_LIMIT); // Mover el barco si no ha alcanzado el límite
            if (!barcos_derecha[i].reached_limit) { // Solo dibujar si no ha alcanzado el límite
                drawBoat(renderer, barcos_derecha[i]); // Dibujar el barco
            }
        }

        // Actualizar la pantalla
        SDL_RenderPresent(renderer);

        // Control de velocidad
        SDL_Delay(16); // Aproximadamente 60 FPS
    }

    // Limpiar recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
