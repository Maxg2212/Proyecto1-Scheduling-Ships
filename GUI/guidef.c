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
void drawBoat(struct Node* boat) {
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
    points[1] = (SDL_Point){boat->x + 80, boat->y};       // Esquina inferior derecha
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

void init_gui() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Scheduling Ships",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            // Chequeo de teclas
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_a) { // Tecla A
                    //if (num_barcos_izquierda < MAX_BARCO) {
                        printf("Tecla A"); //barcos_izquierda[num_barcos_izquierda++] = createRandomBoat(1);
                    //}
                }
                if (event.key.keysym.sym == SDLK_d) { // Tecla D
                    //if (num_barcos_derecha < MAX_BARCO) {
                        printf("Tecla D"); //barcos_derecha[num_barcos_derecha++] = createRandomBoat(0);
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
        //for (int i = 0; i < num_barcos_izquierda; i++) {
            //updateBoat(&barcos_izquierda[i], LEFT_LIMIT); // Mover el barco si no ha alcanzado el límite
            //if (!barcos_izquierda[i].reached_limit) { // Solo dibujar si no ha alcanzado el límite
                //drawBoat(renderer, barcos_izquierda[i]); // Dibujar el barco
            //}
        //}

        // Dibujar y mover todos los barcos de la derecha que no han alcanzado el límite
        //for (int i = 0; i < num_barcos_derecha; i++) {
            //updateBoat(&barcos_derecha[i], RIGHT_LIMIT); // Mover el barco si no ha alcanzado el límite
            //if (!barcos_derecha[i].reached_limit) { // Solo dibujar si no ha alcanzado el límite
                //drawBoat(renderer, barcos_derecha[i]); // Dibujar el barco
            //}
        //}

        // Actualizar la pantalla
        SDL_RenderPresent(renderer);

        // Control de velocidad
        SDL_Delay(16); // Aproximadamente 60 FPS
    }
}

void destroy_gui() {
    if (renderer != nullptr && window != nullptr) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void render_gui(struct Node* left_queue, struct Node* right_queue, int channel_length) {
    // Limpiar el renderer con color celeste
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Celeste
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Azul
    SDL_Rect canal = {600, 200, 600, 200};
    SDL_RenderFillRect(renderer, &canal);

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marrón
    SDL_Rect up_border = {600, 200, 600, 10}; // Borde arriba
    SDL_Rect down_border = {600, 400, 600, 10}; // Borde abajo
    SDL_RenderFillRect(renderer, &up_border);
    SDL_RenderFillRect(renderer, &down_border);

    for (struct Node* currentBoat = left_queue; currentBoat != nullptr; currentBoat = currentBoat->next) {
        drawBoat(currentBoat);
    }

    for (struct Node* currentBoat = right_queue; currentBoat != nullptr; currentBoat = currentBoat->next) {
        drawBoat(currentBoat);;
    }

    // Actualizar la pantalla
    SDL_RenderPresent(renderer);
}

uint32_t get_event() {
    SDL_Event event;
    uint32_t type = 0;
    while (SDL_PollEvent(&event)) {
        type = event.type;
    }
    return type;
}
