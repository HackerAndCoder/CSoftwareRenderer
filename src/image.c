#include "image.h"
#include <stdio.h>
#include <stdlib.h>

Image load_image(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        printf("No image at path %s", path);
        exit(1);
    }
    
    Image img;
    int size[2];
    fread(size, sizeof(int), 2, file);
    img.width = size[0];
    img.height = size[1];
    
    
    int *data = malloc(img.width * img.height * 3 * sizeof(int));
    
    if (!data) {
        printf("Error allocating image data");
        exit(1);
    }
    
    fread(data, sizeof(int), img.width*img.height*3, file);
    //printf("Loaded %d bytes from file\n", img.width * img.height * 3 * (int)sizeof(int));
    
    Color *colors = malloc(img.width * img.height * sizeof(Color));
    
    for (int i = 0; i < img.width * img.height; i++) {
        colors[i].r = data[i * 3 + 0];
        colors[i].g = data[i * 3 + 1];
        colors[i].b = data[i * 3 + 2];
    }
    
    free(data);
    
    img.data = colors;
    
    return img;
}

void free_image(Image *img) {
    free(img->data);
    img->data = NULL;
}