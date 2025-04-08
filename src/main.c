#include "engine.h"
#include "SDL2/SDL_events.h"
#include "stdbool.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <math.h>

#include "image.h"
#include "globals.h"

int time_since_last_tick;

Image test, yellow;

typedef struct {
    int x, y;
    Image *img;
} Sprite;

typedef struct {
    Sprite sprite;
} Player;

typedef struct {
    int x, y;
} Camera;

Player player;

Camera camera;

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
    
    if (is_key_down(SDL_SCANCODE_W)) {camera.y--;}
    if (is_key_down(SDL_SCANCODE_S)) {camera.y++;}
    if (is_key_down(SDL_SCANCODE_A)) {camera.x--;}
    if (is_key_down(SDL_SCANCODE_D)) {camera.x++;}
    
}

void render_level(int xoffset, int yoffset) {
    for (int y = 0; y < LEVELSIZE; y++) {
        for (int x = 0; x < LEVELSIZE; x++) {
            if (level[x][y] == 1) {
                int blockx = floor(x * scale);
                int blocky = floor(y * scale);
                
                int fx = xoffset + blockx;
                int fy = yoffset + blocky;
                
                render_image(fx, fy, &test);
            }
        }
    }
    
    render_image(player.sprite.x + xoffset, player.sprite.y + yoffset, player.sprite.img);
}

void tick(int delta) {
    clear();
    if (delta == 0) {
        delta = 1;
    }
    time_since_last_tick += delta;
    game_tick();
    
    player.sprite.x = camera.x + WIDTH/2-player.sprite.img->width/2;
    player.sprite.y = camera.y + HEIGHT/2-player.sprite.img->height/2;
    
    // TODO CONVERT PLAYER COORDS TO LEVEL SPACE AND CHECK FOR BLOCK AT COORDS
    
    render_level(-camera.x, -camera.y);
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
    yellow = load_image("src/assets/yellow.bin");
    scale_image(&test, scale/test.width);
    
    player.sprite.img = &yellow;
}

void free_images() {
    free_image(&test);
    free_image(&yellow);
}

int main() {
    register_tick_callback(tick);
    init_level();
    init();
    start();
    free_images();
}