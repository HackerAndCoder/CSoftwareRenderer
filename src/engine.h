#ifndef ENGINE_H_
#define ENGINE_H_

#include <SDL2/SDL_events.h>
#include "stdbool.h"
#include "image.h"

typedef void (*TickCallbackFunc)(int);

typedef void (*InputCallbackFunc)(SDL_Event);

void register_tick_callback(TickCallbackFunc func);

void register_input_callback(InputCallbackFunc func);

bool is_key_down(int);

void _set_pixel(int i, Uint32 color);

void set_pixel(int x, int y, Color color);

void set_block(int x, int y, int width, int height, Color color);

void render_image(int x, int y, Image *img);

Uint32 _encode_color(Color color);

void clear();

void start();

void update();

void quit();

#endif