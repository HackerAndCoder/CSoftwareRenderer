#include "engine.h"
#include "SDL2/SDL_events.h"
#include "stdbool.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>

#include "image.h"

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
    
    set_block(0, 0, 40, 40, (Color){255, 0, 0});
    set_block(40, 0, 40, 40, (Color){0, 255, 0});
    set_block(80, 0, 40, 40, (Color){0, 0, 255});
    render_image(100, 100, &test);
    //set_pixel(x, y, (Color){0, 0, 0});
    //set_pixel(x, y+1, (Color){0, 0, 0});
}



void input_callback(SDL_Event event) {
    
}

void init() {
    test = load_image("src/assets/block.bin");
}

int main() {
    register_tick_callback(tick);
    init();
    start();
}