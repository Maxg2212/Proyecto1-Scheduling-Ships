//
// Created by max-garro on 10/12/24.
//

#ifndef GUIDEF_H
#define GUIDEF_H

#include "../scheduling/linked_list.h"

void init_gui();
void destroy_gui();
void render_gui(struct Node** left_queue, struct Node** right_queue, int channel_length);
void get_event(uint32_t* event_pair);

#endif //GUIDEF_H
