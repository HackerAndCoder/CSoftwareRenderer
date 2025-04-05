#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include "stdbool.h"
#include "stdio.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_stdinc.h>
#include "engine.h"

#define WIDTH 1000
#define HEIGHT 800
#define DEBUGFPS false

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* framebuffer;

const Uint8* state;

Uint32 pixel_buffer[WIDTH * HEIGHT];

SDL_Event event;
Uint32 current_time;
Uint32 last_time;
int frame_count;

Uint32 delta;

static TickCallbackFunc tick_func = NULL;
static InputCallbackFunc input_func = NULL;

bool running = true;

Uint32 _encode_color(Color color) {
    return (255 << 24) | (color.b << 16) | (color.g << 8) | color.r;
}

void set_pixel(int x, int y, Color color) {
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT) {
        return;
    }
    pixel_buffer[WIDTH*y + x] = _encode_color(color);
}

void set_block(int x, int y, int width, int height, Color color) {
    for (int a = x; a < x + width; a++) {
        for (int b = y; b < y + height; b++) {
            set_pixel(a, b, color);
        }
    }
}


void register_tick_callback(TickCallbackFunc func) {
    tick_func = func;
}

void register_input_callback(InputCallbackFunc func) {
    input_func = func;
}

bool is_key_down(int scancode) {
    return state[scancode];
}

void clear() {
    Color white = {255, 255, 255};
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        pixel_buffer[i] = _encode_color(white);
    }
}

void update() {
    // fps stuff
    last_time = current_time;
    current_time = SDL_GetTicks();
    delta = current_time - last_time;
    if (DEBUGFPS) {
        frame_count++;
        printf("%d", delta);
    }
   
    while (SDL_PollEvent(&event)) {
        if (input_func) {
            input_func(event);
        }
        
        if (event.type == SDL_QUIT) {
            quit();
            running = false;
            return;
        }
    }
    
    if (tick_func) {
        tick_func(delta);
    }
}

void quit() {
    SDL_DestroyTexture(framebuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void start() {
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow("Test game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    framebuffer = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT);
    
    state = SDL_GetKeyboardState(NULL);
   
    while (running) {
       update();
       
       SDL_UpdateTexture(framebuffer, NULL, pixel_buffer, WIDTH * sizeof(Uint32));
      
       SDL_RenderClear(renderer);
       SDL_RenderCopy(renderer, framebuffer, NULL, NULL);
       SDL_RenderPresent(renderer);
    }
}