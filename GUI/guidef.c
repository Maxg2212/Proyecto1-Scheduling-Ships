// Autor: Max Garro

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h> // Para rand() y srand()
#include <time.h>   // Para time()
#include "guidef.h" // Definiciones adicionales específicas del proyecto

// Definición de constantes
#define WINDOW_WIDTH 1800            // Ancho de la ventana
#define WINDOW_HEIGHT 600            // Alto de la ventana
#define RECT_SPEED 5                 // Velocidad de los barcos normales
#define RECT_SPEED_PATROL 8          // Velocidad de los barcos de patrulla
#define RECT_SPEED_FISHER 6          // Velocidad de los barcos pesqueros
#define LEFT_LIMIT 1250              // Límite izquierdo de movimiento
#define RIGHT_LIMIT 450              // Límite derecho de movimiento
#define LEFT_START_X 350             // Coordenada X inicial de los barcos del lado izquierdo
#define LEFT_START_Y 230             // Coordenada Y inicial de los barcos del lado izquierdo
#define RIGHT_START_X 1250           // Coordenada X inicial de los barcos del lado derecho
#define RIGHT_START_Y 370            // Coordenada Y inicial de los barcos del lado derecho
#define SPACING 100                  // Espacio entre barcos
#define MAX_BARCO 90                 // Número máximo de barcos en pantalla

// Punteros de la GUI
SDL_Window *window = nullptr;        // Ventana de SDL
SDL_Renderer *renderer = nullptr;    // Renderizador de SDL

/**
 * Dibuja un barco en pantalla según su tipo.
 * Dependiendo del tipo de barco (Normal, Pesquero o Patrulla), asigna un color diferente para el casco,
 * y dibuja una vela triangular de color amarillo.
 *
 * @param boat Puntero a la estructura que representa al barco a dibujar.
 * @param direction Dirección en la que se mueve el barco (0 para izquierda a derecha, 1 para derecha a izquierda).
 */
void drawBoat(struct Node* boat, int direction) {
    SDL_Color color;

    // Asignar color dependiendo del tipo de barco
    if (strcmp(boat->boat_type, "Normal") == 0) {
        color = (SDL_Color){255, 0, 0}; // Rojo para barcos normales
    } else if (strcmp(boat->boat_type, "Pesquero") == 0) {
        color = (SDL_Color){0, 255, 0}; // Verde para barcos pesqueros
    } else if (strcmp(boat->boat_type, "Patrulla") == 0) {
        color = (SDL_Color){255, 165, 0}; // Naranja para barcos de patrulla
    }

    // Dibuja el casco del barco
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255); // Establece el color del barco
    SDL_Rect boat_body = {boat->x, boat->y, 80, 30};  // Define el tamaño del barco
    SDL_RenderFillRect(renderer, &boat_body);         // Renderiza el barco en pantalla

    // Dibuja la vela (triángulo)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Establece el color amarillo para la vela
    SDL_Point points[4]; // Puntos para formar la vela en triángulo

    // Definir los puntos de la vela
    points[0] = (SDL_Point){boat->x + 40, boat->y - 30};              // Punta de la vela
    points[1] = (SDL_Point){boat->x + ((direction) ? 0 : 80), boat->y};  // Base de la vela
    points[2] = (SDL_Point){boat->x + 40, boat->y};                   // Base central
    points[3] = points[0];                                            // Cerrar el triángulo

    SDL_RenderDrawLines(renderer, points, 4);  // Renderiza la vela
}

/**
 * Inicializa la GUI de SDL.
 * Crea una ventana con el tamaño especificado y un renderizador acelerado por hardware.
 */
void init_gui() {
    SDL_Init(SDL_INIT_VIDEO); // Inicializa SDL para video

    // Crea una ventana SDL con título, tamaño y posición
    window = SDL_CreateWindow("Scheduling Ships", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    // Crea un renderizador acelerado por hardware para la ventana
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

/**
 * Libera los recursos de la GUI de SDL.
 * Destruye la ventana y el renderizador creados previamente, y cierra SDL.
 */
void destroy_gui() {
    if (renderer != nullptr && window != nullptr) {
        SDL_DestroyRenderer(renderer);  // Destruye el renderizador
        SDL_DestroyWindow(window);      // Destruye la ventana
    }
    SDL_Quit();  // Cierra SDL
}

/**
 * Renderiza la GUI de SDL con los barcos y el canal.
 * Limpia la pantalla, dibuja el canal y los barcos en su respectiva posición según las listas de barcos.
 *
 * @param left_queue Puntero a la cola de barcos que se mueven de izquierda a derecha.
 * @param right_queue Puntero a la cola de barcos que se mueven de derecha a izquierda.
 * @param channel_length Largo del canal a renderizar.
 */
void render_gui(struct Node** left_queue, struct Node** right_queue, int channel_length) {
    // Limpia la pantalla y la llena de color celeste
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Celeste
    SDL_RenderClear(renderer);

    // Dibuja el canal en azul
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Azul
    SDL_Rect canal = {900 - channel_length / 2, 260, channel_length, 80};
    SDL_RenderFillRect(renderer, &canal);

    // Dibuja las paredes del canal en marrón
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marrón
    SDL_Rect up_border = {900 - channel_length / 2, 260, channel_length, 10};  // Borde superior
    SDL_Rect down_border = {900 - channel_length / 2, 340, channel_length, 10}; // Borde inferior
    SDL_Rect wall1 = {900 - channel_length / 2, 0, 10, 260};  // Pared izquierda superior
    SDL_Rect wall2 = {890 + channel_length / 2, 0, 10, 260};  // Pared derecha superior
    SDL_Rect wall3 = {900 - channel_length / 2, 340, 10, 260}; // Pared izquierda inferior
    SDL_Rect wall4 = {890 + channel_length / 2, 340, 10, 260}; // Pared derecha inferior

    SDL_RenderFillRect(renderer, &up_border);    // Renderiza el borde superior
    SDL_RenderFillRect(renderer, &down_border);  // Renderiza el borde inferior
    SDL_RenderFillRect(renderer, &wall1);        // Renderiza la primera pared
    SDL_RenderFillRect(renderer, &wall2);        // Renderiza la segunda pared
    SDL_RenderFillRect(renderer, &wall3);        // Renderiza la tercera pared
    SDL_RenderFillRect(renderer, &wall4);        // Renderiza la cuarta pared

    // Dibuja los barcos de la cola izquierda
    for (struct Node* currentBoat = *left_queue; currentBoat != nullptr; currentBoat = currentBoat->next) {
        drawBoat(currentBoat, 0);  // Dibuja barco que se mueve de izquierda a derecha
    }

    // Dibuja los barcos de la cola derecha
    for (struct Node* currentBoat = *right_queue; currentBoat != nullptr; currentBoat = currentBoat->next) {
        drawBoat(currentBoat, 1);  // Dibuja barco que se mueve de derecha a izquierda
    }

    SDL_RenderPresent(renderer); // Actualiza la pantalla con lo dibujado
}

/**
 * Maneja los eventos de teclado y de cierre de ventana de SDL.
 * Detecta eventos de teclas presionadas o de cierre de ventana, y almacena los resultados en un array.
 *
 * @param event_pair Array donde se almacenan el tipo de evento y la tecla presionada (si aplica).
 */
void get_event(uint32_t* event_pair) {
    SDL_Event event;
    event_pair[0] = 0;  // Inicializa el evento con valor predeterminado (sin evento)
    event_pair[1] = 0;  // Inicializa la tecla presionada con valor predeterminado (sin tecla)

    // Bucle para verificar eventos
    while (SDL_PollEvent(&event)) {
        // Detecta si se presionó una tecla o se cerró la ventana
        if (event.type == SDL_KEYDOWN || event.type == SDL_QUIT) {
            event_pair[0] = event.type;  // Almacena el tipo de evento
            if (event.type == SDL_KEYDOWN) {
                event_pair[1] = event.key.keysym.sym;  // Almacena la tecla presionada
            }
        }
    }
}
