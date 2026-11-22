#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include "fb_only.h"
#include "surface.h"


void draw_v_line(struct surface *surf, int x, int y, int h, uint32_t color)
{
	int c;
	for (c = 0; c < h; c++)
		put_pixel(surf, x, y+c, color);
}

void draw_h_line(struct surface *surf, int x, int y, int w, uint32_t color)
{
	int c;
	for (c = 0; c < w; c++) {
		put_pixel(surf, x+c, y, color);
    }
}

void draw_rect(struct surface *surf, int x, int y, int w, int h, int thick, const char *rgb)
{
    uint32_t color = color_from_hex(rgb);
	draw_v_line	(surf, x, y, h, color);
	draw_v_line	(surf, x+w-1, y, h, color);
	draw_h_line	(surf, x, y, w, color);
	draw_h_line	(surf, x, y+h-1, w, color);
}

void fill_rect(struct surface *surf, int x, int y, int w, int h, const char *rgb)
{
    uint32_t color = color_from_hex(rgb);
	int c;
	for (c = 0; c < h; c++)
		draw_h_line(surf, x, y+c, w, color);
}

void drawline(struct surface *surf, int x0, int y0, int x1, int y1, const char *hex_rgb) {
    uint32_t color = color_from_hex(hex_rgb);
    int dx, dy, p, x, y;
	dx=x1-x0;
	dy=y1-y0;
	x=x0;
	y=y0;
	p=2*dy-dx;
	while(x<x1) {
		if(p>=0){ 
			y=y+1;
			p=p+2*dy-2*dx;
		} else {
			p=p+2*dy;
		}
	    put_pixel(surf, x,y,color);
		x=x+1;
	}
}

// This verson creates rectangles with border widths.
void draw_rect_frame(struct surface *surf, int x, int y, int w, int h, int b, const char *hex_rgb) {
    uint32_t color = color_from_hex(hex_rgb);
    unsigned short cx, cy;
    for (cy = 0; cy < h; cy++) {
        for (cx = 0; cx < w; cx++) {
			if (((cx < b) || ((w-cx) <= b))  || 
				 ((cy < b) || ((h-cy) <= b))){
				put_pixel(surf, x + cx, y + cy, color);
			}
		}
    }
}

// circle
void plot4points(struct surface *surf, int cx, int cy, int x, int y, uint32_t color) {
    put_pixel(surf, cx + x, cy + y, color);
    put_pixel(surf, cx - x, cy + y, color);
    put_pixel(surf, cx + x, cy - y, color);
    put_pixel(surf, cx - x, cy - y, color);
    usleep(1000);
}

void draw_circle(struct surface *surf, int cx, int cy, int radius, const char *pixel) {
    uint32_t color = color_from_hex(pixel);
	int error = -radius;
	double x = radius;
	double y = 0;
	while (x >= y) {
        plot4points(surf, cx, cy, x, y, color);
        plot4points(surf, cx, cy, y, x, color);
        error += y;
        y++;
        error += y;
        if (error >= 0) {
            error += -x;
            x--;
            error += -x;
        }
    }
}


void draw_rounded_rect(struct surface *surf, int x, int y, int w, int h, int r, const char *color) {
//    printf("(%s) x=%d y=%d w=%d h=%d r=%d\n", __func__, x, y, w, h, r);
  uint32_t rgb = color_from_hex(color);
  if (r <= 0) {
    draw_rect(surf, x, y, w, h,6, color);
  }
  if (r * 2 > w) r = w / 2;
  if (r * 2 > h) r = h / 2;

  int r2 = r * r;
  for (int yy = 0; yy < h; yy++) {
    int left = 0;
    int right = w;
    int dy;
    if (yy < r)
      dy = r - yy - 1;
    else if (yy >= h - r)
      dy = yy - (h - r);
    else
      dy = -1;
    if (dy >= 0) {
      /* dx = sqrt(r^2 - dy^2), integer floor — pixels with x in
       * [r-dx, r+dx) are inside the quarter circle. */
      int dx = 0;
      while ((dx + 1) * (dx + 1) + dy * dy <= r2)
        dx++;
      left = r - dx;
      right = w - r + dx;
    }
    int yabs = y + yy;
    int x_start = x + left;
    int x_len = right - left;
    draw_h_line(surf, x_start, yabs, x_len, rgb);
  }
}


/*
draw_rect(620,90,88,55,8, "#0FE60B");
draw_rounded_rect(599, 150, 200, 200, 40, "#ff0000");
draw_circle(400,800,160,"#ffffff");
fill_rect(420,690,488,500, "#a80fb1");
drawline(300,300, 600, 600, "#ff0000");
drawline(600,600, 922, 1555, "#00ff00");
drawline(700,1170, 1222, 1920, "#0000ff");
draw_rect(620,90,88,55,8, "#0FE60B");
draw_v_line(300, 800, 60, "#ff003b");
draw_v_line(301, 800, 60, "#ff003b");
draw_v_line(302, 800, 60, "#ff003b");
draw_h_line(300, 800, 60, "#ff003b");
*/
