//
// Created by max-garro on 10/3/24.
//

#ifndef CHANNEL_SETTINGS_H
#define CHANNEL_SETTINGS_H

// Metodo control de flujo
#define EQUIDAD 1;
#define LETRERO 2;
#define TICO 3;

const int channel_lenght = 700;
const int boat_velocity = 20;
int cant_sorted_ready_boats_left;
int cant_sorted_ready_boats_right;
double letrero_time = 10;
extern int W_input; //Es declarada por el usuario.



#endif //CHANNEL_SETTINGS_H
