#include "engine.h"
#include "SDL2/SDL_events.h"
#include "stdbool.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <stdlib.h>

#include "image.h"
#include "globals.h"

int x, y;
int time_since_last_tick;

Image test;

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
    
    set_block(0+x, 0+y, 40, 40, (Color){255, 0, 0});
    set_block(40+x, 0+y, 40, 40, (Color){0, 255, 0});
    set_block(80+x, 0+y, 40, 40, (Color){0, 0, 255});
    
    render_image(0, 0, &test);
    
    //int uvx = (int)(WIDTH / x);
    //int uvy = (int)(HEIGHT / y);
}



void input_callback(SDL_Event event) {
    
}

void init() {
    test = load_image("src/assets/block.bin");
    scale_image(&test, 1);
}

void free_images() {
    free_image(&test);
}

int main() {
    register_tick_callback(tick);
    init();
    start();
    free_images();
}