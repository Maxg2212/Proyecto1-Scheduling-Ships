#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // Para rand() y srand()
#include <time.h>   // Para time()
#include "guidef.h"

#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 600
#define RECT_SPEED 5
#define RECT_SPEED_PATROL 8
#define RECT_SPEED_FISHER 6
#define LEFT_LIMIT 1250
#define RIGHT_LIMIT 450

// Coordenadas fijas para los barcos
#define LEFT_START_X 350
#define LEFT_START_Y 230
#define RIGHT_START_X 1250
#define RIGHT_START_Y 370
#define SPACING 100 // Espacio entre barcos
#define MAX_BARCO 90 // Número máximo de barcos

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
        if (barco->tipo == PATRULLA) {
            barco->x += barco->direction * RECT_SPEED_PATROL;
        }

        // Si el barco ha alcanzado el límite, detener su movimiento
        if ((barco->direction == 1 && barco->x >= limit) ||
            (barco->direction == -1 && barco->x <= limit)) {
            barco->reached_limit = true;
        }
    }
}

// Función para crear un barco aleatorio
Barco createRandomBoat(int is_left) {
    BarcoTipo tipo = rand() % 3; // Tipo aleatorio (0, 1 o 2)
    Barco barco;
    barco.tipo = tipo;
    barco.reached_limit = false;
    barco.direction = (is_left) ? 1 : -1; // 1 para barcos de izquierda, -1 para barcos de derecha

    // Establecer la posición inicial
    if (is_left) {
        barco.x = LEFT_START_X;
        barco.y = LEFT_START_Y;
    } else {
        barco.x = RIGHT_START_X;
        barco.y = RIGHT_START_Y;
    }

    return barco;
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
    srand(time(NULL)); // Inicializa la semilla aleatoria

    SDL_Window *window = SDL_CreateWindow("Schedulers",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Listas de barcos (inicialmente vacías)
    Barco barcos_izquierda[MAX_BARCO];
    Barco barcos_derecha[MAX_BARCO];
    int num_barcos_izquierda = 0;
    int num_barcos_derecha = 0;

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            // Chequeo de teclas
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_a) { // Tecla A
                    //if (num_barcos_izquierda < MAX_BARCO) {
                        barcos_izquierda[num_barcos_izquierda++] = createRandomBoat(1);
                    //}
                }
                if (event.key.keysym.sym == SDLK_d) { // Tecla D
                    //if (num_barcos_derecha < MAX_BARCO) {
                        barcos_derecha[num_barcos_derecha++] = createRandomBoat(0);
                    //}
                }
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
