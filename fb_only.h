#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <unistd.h>
#include <stdlib.h>

//#define FB_OFF(fbx,fby) ((fbx+vinfo.xoffset)*pinc+(fby+vinfo.yoffset)*finfo.line_length)
#define FB_OFF(fbx,fby) ((fbx+vinfo.xoffset)*pinc+(fby+back_buffer_y)*finfo.line_length)



//void put_pixel(int x, int y, const char *hex_rgb);
//void put_pixel2(struct surface *surf, int x, int y, uint32_t color);
//uint32_t color_from_hex(const char *hex_rgb);
void surface_from_the_front(struct surface *s);
void surface_from_the_back(struct surface *s);

void fb_swap();
int getFbXres();
int getFbYres();
int getStride();
int getBpp();
uint8_t* getBuffer_F();
uint8_t* getBuffer_B();
int init_fb();
void close_fb();

#endif
