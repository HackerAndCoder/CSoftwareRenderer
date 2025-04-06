#ifndef IMAGE_H_
#define IMAGE_H_

typedef struct {
    int r, g, b;
} Color;

typedef struct {
    int width, height;
    Color *data;
} Image;

Image load_image(const char *path);

void free_image(Image *img);

#endif