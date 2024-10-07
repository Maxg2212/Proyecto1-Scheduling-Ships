#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH 1000 // Aumentar el ancho de la ventana
#define WINDOW_HEIGHT 800 // Aumentar la altura de la ventana
#define RECT_SPEED 5

void drawBoat(SDL_Renderer *renderer, int x, int y, SDL_Color color) {
    // Dibuja el casco del bote (rectángulo)
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_Rect boat_body = {x, y, 80, 30}; // Tamaño del bote
    SDL_RenderFillRect(renderer, &boat_body);

    // Dibuja la vela (triángulo)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Color amarillo
    SDL_Point points[4];
    points[0] = (SDL_Point){x + 40, y - 30}; // Punta de la vela
    points[1] = (SDL_Point){x + 80, y};       // Esquina inferior derecha
    points[2] = (SDL_Point){x + 40, y};       // Esquina inferior izquierda
    points[3] = points[0];                     // Cerrar el triángulo

    SDL_RenderDrawLines(renderer, points, 4);  // Dibuja el triángulo
}

void drawCanal(SDL_Renderer *renderer) {
    // Dibuja el agua del canal
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Color azul
    SDL_Rect canal = {100, 300, 600, 200}; // Rectángulo del canal (manteniendo tamaño y posición)
    SDL_RenderFillRect(renderer, &canal);

    // Dibuja los bordes del canal
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Color marrón para los bordes
    SDL_Rect left_border = {100, 300, 10, 200}; // Borde izquierdo
    SDL_Rect right_border = {690, 300, 10, 200}; // Borde derecho
    SDL_RenderFillRect(renderer, &left_border);
    SDL_RenderFillRect(renderer, &right_border);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Botes en Movimiento",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Posición y tamaño de los botes
    int boat_red_x = 0;
    int boat_red_y = (WINDOW_HEIGHT - 30) / 2; // Centrar verticalmente

    int boat_green_x = 300; // Posición inicial del bote verde
    int boat_green_y = 100; // Posición vertical del bote verde

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Mover el bote rojo hacia la derecha
        boat_red_x += RECT_SPEED;

        // Reiniciar posición del bote rojo si alcanza x = 400
        if (boat_red_x >= 400) {
            boat_red_x = -80; // Reiniciar posición fuera de la pantalla
        }

        // Mover el bote verde hacia la izquierda
        boat_green_x -= RECT_SPEED;

        // Reiniciar posición del bote verde si alcanza x = 20
        if (boat_green_x <= 20) {
            boat_green_x = WINDOW_WIDTH; // Reiniciar posición fuera de la pantalla
        }

        // Limpiar el renderer con color celeste
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Color celeste (Sky Blue)
        SDL_RenderClear(renderer);

        // Dibujar el canal
        drawCanal(renderer);

        // Dibujar los botes
        SDL_Color red_boat_color = {255, 0, 0}; // Color rojo
        SDL_Color green_boat_color = {0, 255, 0}; // Color verde
        drawBoat(renderer, boat_red_x, boat_red_y, red_boat_color);
        drawBoat(renderer, boat_green_x, boat_green_y, green_boat_color);

        // Actualizar la pantalla
        SDL_RenderPresent(renderer);

        // Esperar un corto tiempo para controlar la velocidad de movimiento
        SDL_Delay(16); // Aproximadamente 60 FPS
    }

    // Limpiar recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
