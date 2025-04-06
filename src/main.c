#include "engine.h"
#include "SDL2/SDL_events.h"
#include "stdbool.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>

#include "image.h"
#include "globals.h"
#include "vector.h"

int x, y;
int time_since_last_tick;

Image test;

Image buffer;

#define LEVELSIZE 10
int level[LEVELSIZE][LEVELSIZE];

int scale = HEIGHT / LEVELSIZE;

Player player;

void cast_rays(Player player) {
    int fov = 60;
    int num_rays = WIDTH;
    float ray_angle_step = (fov * (M_PI / 180)) / num_rays;
    float ray_angle = player.angle - (fov * (M_PI / 180)) / 2;

    for (int i = 0; i < num_rays; i++) {
        float dx = cos(ray_angle);
        float dy = sin(ray_angle);

        float rx = player.x;
        float ry = player.y;
        
        int hit_side = -1; // 0 = vertical, 1 = horizontal
        float step = 0.01;

        while (1) {
            rx += dx * 0.01;
            ry += dy * 0.01;

            if (level[(int)ry][(int)rx] == 1) {
                float x_offset = rx - (int)rx;
                float y_offset = ry - (int)ry;
                
                if (fabs(x_offset) > fabs(y_offset)) {
                    hit_side = 0; // vertical wall
                } else {
                    hit_side = 1; // horizontal wall
                }
                break;
            }
        }

        float dist = sqrtf((rx - player.x) * (rx - player.x) + (ry - player.y) * (ry - player.y));
        float line_height = HEIGHT / (dist + 0.01); // avoid div by 0

        int draw_start = (HEIGHT - line_height) / 2;
        int draw_end = draw_start + line_height;
        
        float hit_x;
        if (hit_side == 0) {
            // vertical wall hit → use ry
            hit_x = ry - floorf(ry);
        } else {
            // horizontal wall hit → use rx
            hit_x = rx - floorf(rx);
        }
        
        int texture_width = test.width;
        int texture_height = test.height;
        
        int tex_x = (int)(hit_x * texture_width);
        
        if (tex_x > texture_width) {
            tex_x = 1;
        }

        for (int y = draw_start; y < draw_end; y++) {
            float texture_pos = (float)(y - draw_start) / (draw_end - draw_start);
            int tex_y = (int)(texture_pos * texture_width);
            
            // Clamp just in case
            if (tex_y < 0) tex_y = 0;
            if (tex_y >= texture_height) tex_y = texture_height - 1;
            
            // Sample from the texture
            Color texel = test.data[tex_y * texture_width + tex_x];
            _set_pixel(y * WIDTH + i, _encode_color(texel));
        }

        ray_angle += ray_angle_step;
    }
}

void game_tick() {
    //printf("%i", time_since_last_tick);
    if (time_since_last_tick < 10) {
        return;
    }
    time_since_last_tick = 0;
    
    float move_speed = 0.05f;
    
    if (is_key_down(SDL_SCANCODE_W)) {
        float dx = cosf(player.angle);
        float dy = sinf(player.angle);
        player.x += dx * move_speed;
        player.y += dy * move_speed;
    }
    if (is_key_down(SDL_SCANCODE_S)) {y++;}
    if (is_key_down(SDL_SCANCODE_A)) {player.angle -= 0.01f;}
    if (is_key_down(SDL_SCANCODE_D)) {player.angle += 0.01f;}
    
}

void tick(int delta) {
    clear();
    if (delta == 0) {
        delta = 1;
    }
    time_since_last_tick += delta;
    game_tick();
    cast_rays(player);
}

void input_callback(SDL_Event event) {}

void init_level() {
    for (int y = 0; y < LEVELSIZE; y++) {
        for (int x = 0; x < LEVELSIZE; x++) {
            if (x == 0 || x == LEVELSIZE-2 || y == 0 || y == LEVELSIZE-2) {
                level[x][y] = 1;
            }
        }
    }
}

void init() {
    player = (Player){5.0f, 5.0f, 0.0f};
    buffer = (Image) {WIDTH, HEIGHT, NULL};
    
    test = load_image("src/assets/block.bin");
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