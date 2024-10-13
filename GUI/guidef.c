#include <SDL.h>
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

// GUI pointers
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

// Función para dibujar el barco según su tipo
void drawBoat(struct Node* boat, int direction) {
    SDL_Color color;
    if (strcmp(boat->boat_type, "Normal") == 0) {
        color = (SDL_Color){255, 0, 0}; // Rojo
    } else if (strcmp(boat->boat_type, "Pesquero") == 0) {
        color = (SDL_Color){0, 255, 0}; // Verde
    } else if (strcmp(boat->boat_type, "Patrulla") == 0) {
        color = (SDL_Color){255, 165, 0}; // Naranja
    }

    // Dibuja el casco del barco
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_Rect boat_body = {boat->x, boat->y, 80, 30}; // Tamaño del barco
    SDL_RenderFillRect(renderer, &boat_body);

    // Dibuja la vela (triángulo)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Color amarillo
    SDL_Point points[4];
    points[0] = (SDL_Point){boat->x + 40, boat->y - 30}; // Punta de la vela
    points[1] = (SDL_Point){boat->x + ((direction) ? 0 : 80), boat->y};       // Esquina inferior derecha
    points[2] = (SDL_Point){boat->x + 40, boat->y};       // Esquina inferior izquierda
    points[3] = points[0];                                // Cerrar el triángulo

    SDL_RenderDrawLines(renderer, points, 4);  // Dibuja el triángulo
}

// Función para actualizar la posición del barco, si no ha alcanzado su límite
//void updateBoat(Barco *barco, int limit) {
    //if (!barco->reached_limit) {
        //barco->x += barco->direction * RECT_SPEED;
        //if (barco->tipo == PATRULLA) {
            //barco->x += barco->direction * RECT_SPEED_PATROL;
        //}

        // Si el barco ha alcanzado el límite, detener su movimiento
        //if ((barco->direction == 1 && barco->x >= limit) ||
            //(barco->direction == -1 && barco->x <= limit)) {
            //barco->reached_limit = true;
        //}
    //}
//}

void init_gui() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Scheduling Ships",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void destroy_gui() {
    if (renderer != nullptr && window != nullptr) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void render_gui(struct Node** left_queue, struct Node** right_queue, int channel_length) {
    // Limpiar el renderer con color celeste
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Celeste
    SDL_RenderClear(renderer);

    // Dibujar el canal
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Azul
    SDL_Rect canal = {900 - channel_length / 2, 260, channel_length, 80};
    SDL_RenderFillRect(renderer, &canal);

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marrón
    SDL_Rect up_border = {900 - channel_length / 2, 260, channel_length, 10}; // Borde arriba
    SDL_Rect down_border = {900 - channel_length / 2, 340, channel_length, 10}; // Borde
    SDL_Rect wall1 = {900 - channel_length / 2, 0, 10, 260};
    SDL_Rect wall2 = {890 + channel_length / 2, 0, 10, 260};
    SDL_Rect wall3 = {900 - channel_length / 2, 340, 10, 260};
    SDL_Rect wall4 = {890 + channel_length / 2, 340, 10, 260};

    SDL_RenderFillRect(renderer, &up_border);
    SDL_RenderFillRect(renderer, &down_border);
    SDL_RenderFillRect(renderer, &wall1);
    SDL_RenderFillRect(renderer, &wall2);
    SDL_RenderFillRect(renderer, &wall3);
    SDL_RenderFillRect(renderer, &wall4);

    for (struct Node* currentBoat = *left_queue; currentBoat != nullptr; currentBoat = currentBoat->next) {
        drawBoat(currentBoat, 0);
    }

    for (struct Node* currentBoat = *right_queue; currentBoat != nullptr; currentBoat = currentBoat->next) {
        drawBoat(currentBoat, 1);
    }

    // Actualizar la pantalla
    SDL_RenderPresent(renderer);
}

void get_event(uint32_t* event_pair) {
    SDL_Event event;
    event_pair[0] = 0;  // Default to no event
    event_pair[1] = 0;  // Default to no key pressed

    // Loop to check for events
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN || event.type == SDL_QUIT) {
            event_pair[0] = event.type;
            if (event.type == SDL_KEYDOWN) {
                event_pair[1] = event.key.keysym.sym;
            }
            return;  // Return on first relevant event
        }
    }
}



