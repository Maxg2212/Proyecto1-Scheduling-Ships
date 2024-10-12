//
// Created by max-garro on 10/12/24.
//
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "guidef.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define RECT_SPEED 5
#define LEFT_LIMIT 750
#define RIGHT_LIMIT 50

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

// Función para actualizar la posición del barco actual
void updateBoat(Barco *barco, int limit, bool *reached_limit) {
    barco->x += barco->direction * RECT_SPEED;

    // Si el barco ha alcanzado su límite, marcarlo como alcanzado
    if ((barco->direction == 1 && barco->x >= limit) || (barco->direction == -1 && barco->x <= limit)) {
        *reached_limit = true;
    }
}

// Dibujar el canal en el centro de la pantalla
void drawCanal(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Azul
    SDL_Rect canal = {200, 300, 600, 200};
    SDL_RenderFillRect(renderer, &canal);

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marrón
    SDL_Rect up_border = {200, 300, 600, 10}; // Borde arriba
    SDL_Rect down_border = {200, 500, 600, 10}; // Borde abajo
    SDL_RenderFillRect(renderer, &up_border);
    SDL_RenderFillRect(renderer, &down_border);
}

void start_gui() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Botes en Movimiento",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Lista de barcos que inician del lado izquierdo (coordenadas fijas en y)
    Barco barcos_izquierda[3] = {
        {NORMAL, 0, 100, 1},      // Barco normal
        {PESQUERA, 0, 100, 1},    // Barco pesquero
        {PATRULLA, 0, 100, 1}     // Barco patrulla
    };
    int num_barcos_izquierda = 3;
    int current_left_boat = 0; // Índice del barco que se está moviendo en la izquierda

    // Lista de barcos que inician del lado derecho (coordenadas fijas en y)
    Barco barcos_derecha[2] = {
        {NORMAL, 800, 100, -1},   // Barco normal
        {NORMAL, 800, 100, -1}    // Barco normal
    };
    int num_barcos_derecha = 2;
    int current_right_boat = 0; // Índice del barco que se está moviendo en la derecha

    // Banderas para verificar si el barco ha alcanzado el límite
    bool left_reached = false;
    bool right_reached = false;

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

        // Mover el barco actual de la izquierda
        if (current_left_boat < num_barcos_izquierda) {
            if (!left_reached) {
                updateBoat(&barcos_izquierda[current_left_boat], LEFT_LIMIT, &left_reached);
            } else {
                current_left_boat++; // Pasar al siguiente barco
                left_reached = false; // Reiniciar bandera
            }

            // Dibujar el barco que está en movimiento
            if (current_left_boat < num_barcos_izquierda) {
                drawBoat(renderer, barcos_izquierda[current_left_boat]);
            }
        }

        // Mover el barco actual de la derecha
        if (current_right_boat < num_barcos_derecha) {
            if (!right_reached) {
                updateBoat(&barcos_derecha[current_right_boat], RIGHT_LIMIT, &right_reached);
            } else {
                current_right_boat++; // Pasar al siguiente barco
                right_reached = false; // Reiniciar bandera
            }

            // Dibujar el barco que está en movimiento
            if (current_right_boat < num_barcos_derecha) {
                drawBoat(renderer, barcos_derecha[current_right_boat]);
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
