#ifndef SURFACE_H
#define SURFACE_H

#include <stdint.h>
#include <stddef.h>

struct surface {
    uint8_t *pixels;   // base of the pixel buffer
    int      stride;   // bytes per row
    int      bpp;      // bytes per pixel: 2 or 4
    int      width;    // visible pixels per row
    int      height;
};

enum {
    FRONT,
    BACK,
    ION,
};

static inline uint32_t color_from_hex(const char *hex_rgb) {
    if (hex_rgb[0] == '#') hex_rgb++;
    uint32_t v = (uint32_t)strtol(hex_rgb, NULL, 16) & 0xFFFFFF;
    return 0xFF000000u | v;
}

static inline void put_pixel(struct surface *surf, int x, int y, uint32_t color) {
   // uint8_t *p = (fbp + (size_t)y * finfo.line_length + (size_t)x * 8);
 //FB_OFF(fbx,fby) ((fbx+vinfo.xoffset)*pinc+(fby+back_buffer_y)*finfo.line_length)
    //uint8_t *p = fbp+((x+vinfo.xoffset)*4+(y+back_buffer_y)*finfo.line_length);
    //uint8_t *p = surf->pixels +((x+vinfo.xoffset)*4+(y)*finfo.line_length);
    uint8_t *p = surf->pixels +((x)* surf->bpp +(y)*surf->stride);
    p[0] = (color >> 16) & 0xFF;
    p[1] = (color >> 8) & 0xFF;
    p[2] = (color >> 0) & 0xFF;
    p[3] = (color >> 24) & 0xFF;

}

void set_drawning_surface(int num);
#endif
