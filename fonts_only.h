#ifndef FONTS_ONLY_H
#define FONTS_ONLY_H

#include "surface.h"

void font_init(void);
void draw_text(struct surface *s, const char *text, int x, int y, const char *hex_color, int scale);
void draw_char(struct surface *s, int x, int y, char c, uint32_t color, int scale);

#endif
