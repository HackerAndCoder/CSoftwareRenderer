#include "engine.h"
#include "SDL2/SDL_events.h"
#include "stdbool.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "globals.h"

#define PIPENUM 2
#define PIPEWIDTH 70

#define PIPECLEARANCE 200

#define PIPESPACE WIDTH / PIPENUM

#define JUMP_STRENGTH 10
#define GRAVITY 0.001

int time_since_last_tick;

Image test, yellow;

typedef struct {
    int x, y;      // Top-left corner
    int width, height;
} Rectangle;

typedef struct {
    int x, space_height, width;
} Pipe;

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

typedef struct {
    int y;
    float velocity;
} Bird;

Camera camera;

Bird bird;

bool jumped;

Pipe pipes[PIPENUM];

Pipe gen_pipe(int x) {
    Pipe pipe;
    
    pipe.x = x;
    
    pipe.width = PIPEWIDTH;
    
    int space = random() % (HEIGHT - PIPECLEARANCE * 2) + PIPECLEARANCE;
    
    pipe.space_height = space;
    return pipe;
}

void game_tick() {
    //printf("%i", time_since_last_tick);
    if (time_since_last_tick < 10) {
        return;
    }
    time_since_last_tick = 0;
    
    for (int i = 0; i < PIPENUM; i++) {
        pipes[i].x -= 1;
        if (pipes[i].x < -PIPEWIDTH) {
            pipes[i] = gen_pipe(WIDTH);
        }
    }
    
    bird.velocity -= GRAVITY;
    bird.y -= floor(bird.velocity);
    
    if (bird.y < 0) {
        bird.y = 0;
    }
    
    printf("%d\n", bird.y);
    fflush(stdout);
    
    if (is_key_down(SDL_SCANCODE_SPACE) && !jumped) {
        jumped = true;
        bird.y -= JUMP_STRENGTH;
        //bird.y = 10;
        printf("Jumped\n");
    } else {
        jumped = false;
    }
    
    
}

void render_level(int xoffset, int yoffset) {
    for (int i = 0; i < PIPENUM; i++) {
        Pipe current = pipes[i];
        set_block(current.x, 0, current.width, current.space_height - (PIPECLEARANCE / 2), (Color){0, 170, 0});
        set_block(current.x, current.space_height + (PIPECLEARANCE / 2), current.width, HEIGHT, (Color){0, 170, 0});
    }
    
    set_block(WIDTH / 2, bird.y, 100, 100, (Color){255, 0, 0});
}

void tick(int delta) {
    clear();
    if (delta == 0) {
        delta = 1;
    }
    time_since_last_tick += delta;
    
    render_level(-camera.x, -camera.y);
    game_tick();
}

void input_callback(SDL_Event event) {}

void init() {
    for (int i = 0; i < PIPENUM; i++) {
        pipes[i] = gen_pipe(i * PIPESPACE);
    }
}

void free_images() {
    free_image(&test);
    free_image(&yellow);
}

int main() {
    register_tick_callback(tick);
    init();
    start();
    free_images();
}