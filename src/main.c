#include "engine.h"
#include "SDL2/SDL_events.h"
#include "stdbool.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>

#include "image.h"
#include "globals.h"

int camera_x, camera_y;
int time_since_last_tick;

Image test;


#define LEVELSIZE 10
int level[LEVELSIZE][LEVELSIZE];

float scale = HEIGHT / LEVELSIZE;

void game_tick() {
    //printf("%i", time_since_last_tick);
    if (time_since_last_tick < 10) {
        return;
    }
    time_since_last_tick = 0;
    
    float move_speed = 0.05f;
    
    if (is_key_down(SDL_SCANCODE_W)) {camera_y++;}
    if (is_key_down(SDL_SCANCODE_S)) {camera_y--;}
    if (is_key_down(SDL_SCANCODE_A)) {camera_x++;}
    if (is_key_down(SDL_SCANCODE_D)) {camera_x--;}
    
}

void render_level(int xoffset, int yoffset) {
    for (int y = 0; y < LEVELSIZE; y++) {
        for (int x = 0; x < LEVELSIZE; x++) {
            if (level[x][y] == 1) {
                float fx = xoffset + x * scale;
                float fy = yoffset + y * scale;
                
                int finalx = (int)(fx);
                int finaly = (int)(fy);
                
                render_image(finalx, finaly, &test);
            }
        }
    }
}

void tick(int delta) {
    clear();
    if (delta == 0) {
        delta = 1;
    }
    time_since_last_tick += delta;
    game_tick();
    
    int player_size = 20;
    set_block(WIDTH/2-player_size, HEIGHT/2-player_size, player_size, player_size, (Color){255, 0, 0});
    
    render_level(camera_x, camera_y);
}

void input_callback(SDL_Event event) {}

void init_level() {
    for (int y = 0; y < LEVELSIZE; y++) {
        for (int x = 0; x < LEVELSIZE; x++) {
            if (x == 0 || x == LEVELSIZE-1 || y == 0 || y == LEVELSIZE-1) {
                level[x][y] = 1;
            } else {
                level[x][y] = 0;
            }
        }
    }
}

void init() {
    test = load_image("src/assets/block.bin");
    scale_image(&test, scale/test.width);
}

void free_images() {
    free_image(&test);
}

int main() {
    register_tick_callback(tick);
    init_level();
    init();
    start();
    free_images();
}