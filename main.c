#include "engine.h"
#include "SDL2/SDL_events.h"
#include "stdbool.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>

int x, y;
int time_since_last_tick;

void game_tick() {
    //printf("%i", time_since_last_tick);
    if (time_since_last_tick < 10) {
        return;
    }
    time_since_last_tick = 0;
    
    if (is_key_down(SDL_SCANCODE_W)) {y--;}
    if (is_key_down(SDL_SCANCODE_S)) {y++;}
    if (is_key_down(SDL_SCANCODE_A)) {x--;}
    if (is_key_down(SDL_SCANCODE_D)) {x++;}
}

void tick(int delta) {
    clear();
    if (delta == 0) {
        delta = 1;
    }
    time_since_last_tick += delta;
    game_tick();
    
    set_block(x, y, 4, 4, (Color){0, 0, 0});
    //set_pixel(x, y, (Color){0, 0, 0});
    //set_pixel(x, y+1, (Color){0, 0, 0});
}



void input_callback(SDL_Event event) {
    
}

void init() {
    
}

int main() {
    register_tick_callback(tick);
    start();
}