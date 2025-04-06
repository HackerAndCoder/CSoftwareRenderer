#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    fclose(file);
    //printf("Loaded %d bytes from file\n", img.width * img.height * 3 * (int)sizeof(int));
    
    Color *colors = malloc(img.width * img.height * sizeof(Color));
    
    for (int i = 0; i < img.width * img.height; i++) {
        colors[i].r = data[i * 3 + 2];
        colors[i].g = data[i * 3 + 1];
        colors[i].b = data[i * 3 + 0];
    }
    
    free(data);
    
    img.data = colors;
    
    return img;
}

void scale_image(Image *img, float scale) {
    if (!img || !img->data) {
        return;
    }
    int nwidth = (int)ceil(img->width*scale);
    int nheight = (int)ceil(img->height*scale);
    
    if (nwidth <= 0 || nheight <= 0) {
        return;
    }
    
    Color *new_data = malloc(nwidth * nheight * sizeof(Color));
    
    if (!new_data) {
        printf("Failed to allocate memorying scale_img!\n");
        exit(1);
    }
    
    
    for (int y = 0; y < nheight; y++) {
        for (int x = 0; x < nwidth; x++) {
            int oldx = x / scale;
            int oldy = y / scale;
            
            if (oldx >= img->width) {oldx = img->width-1;}
            if (oldy >= img->height) {oldy = img->height-1;}
            
            new_data[nwidth * y + x] = img->data[img->width*oldy+oldx];
        }
    }
    
    free(img->data);
    
    img->data = new_data;
    img->width = nwidth;
    img->height = nheight;
    
}

void free_image(Image *img) {
    free(img->data);
    img->data = NULL;
}