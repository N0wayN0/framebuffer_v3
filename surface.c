#include "surface.h"
#include "overlay.h"
#include "fb_only.h"

struct surface drawning_surface = {0};

void print_surface_info(struct surface *s) {
    printf("(%s) surface->pixels     %p \n", __func__, s->pixels);
    printf("(%s) surface->stride     %d \n", __func__, s->stride);
    printf("(%s) surface->bpp        %d \n", __func__, s->bpp);
    printf("(%s) surface->width      %d \n", __func__, s->width);
    printf("(%s) surface->height     %d \n", __func__, s->height);
}

void surface_from_the_front(struct surface *s) {
    s->pixels = getBuffer_F();
    s->stride = getStride();
    s->bpp = getBpp();
    s->width = getFbXres();
    s->height = getFbYres();
    print_surface_info(s);
}

void surface_from_the_back(struct surface *s) {
    s->pixels = getBuffer_B();
    s->stride = getStride();
    s->bpp = getBpp();
    s->width = getFbXres();
    s->height = getFbYres();
    print_surface_info(s);
}
        
void surface_from_ion() {
    drawning_surface.pixels = getCanvas_mem();
    drawning_surface.stride = getCanvas_width() * 4;
    drawning_surface.bpp = 4;
    drawning_surface.width = getCanvas_width();
    drawning_surface.height = getCanvas_height();
    //print_surface_info(&drawning_surface);
}

void set_drawning_surface(int num) {
    if (num == FRONT) surface_from_the_front(&drawning_surface);
    if (num == BACK) surface_from_the_back(&drawning_surface);
    if (num == ION) surface_from_ion();
}
