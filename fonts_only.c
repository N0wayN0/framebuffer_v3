#include "fonts_only.h"
#include <string.h>
#include <stdlib.h>
#include "roboto_23x41.h"   /* font struct */
//#include "surface.h"

#define FONT_COUNT 96          /* ASCII 32..127 */

/* Decoded glyphs: 96 characters, each cwidth cheight bytes.
 * 0 = background, 1 = foreground. ~90 KB for a font. */
//static uint8_t glyphs[FONT_COUNT][font.cwidth * font.cheight];
static uint8_t glyphs[FONT_COUNT][32 * 41];

/* Decode the whole font strip once. Walk the RLE data
 * a single time, splitting pixels into per-character slots. */
void font_init(void)
{
    memset(glyphs, 0, sizeof(glyphs));

    const unsigned char *in = font.rundata;
    unsigned char data;
    int pixel_pos = 0;

    while ((data = *in++)) {
        int run      = data & 0x7f;
        uint8_t val  = (data & 0x80) ? 1 : 0;

        for (int i = 0; i < run; i++) {
            int row = pixel_pos / font.width;
            int col = pixel_pos % font.width;

            if (row < font.cheight) {
                int char_idx = col / font.cwidth;
                int char_col = col % font.cwidth;
                if (char_idx < FONT_COUNT) {
                    glyphs[char_idx][row * font.cwidth + char_col] = val;
                }
            }
            pixel_pos++;
        }
    }
}

/* Draw one character at (x, y). color is already uint32_t.
 * Scaling is done inline: each foreground pixel becomes
 * a scale × scale block, written with put_pixel. */
void draw_char(struct surface *s,
               int x, int y,
               char c,
               uint32_t color,
               int scale)
{
    if (!s || !s->pixels) return;
    if (c < 32 || c > 127) return;
    if (scale < 1) scale = 1;

    /* Early-out if the glyph is entirely off-screen */
    if (x >= s->width  || y >= s->height) return;
    if (x + font.cwidth  * scale <= 0) return;
    if (y + font.cheight * scale <= 0) return;

    const uint8_t *g = glyphs[c - 32];

    for (int row = 0; row < font.cheight; row++) {
        for (int col = 0; col < font.cwidth; col++) {
            if (!g[row * font.cwidth + col]) continue;

            /* foreground pixel: expand to scale × scale block */
            for (int sy = 0; sy < scale; sy++) {
                for (int sx = 0; sx < scale; sx++) {
                    put_pixel(s,
                              x + col * scale + sx,
                              y + row * scale + sy,
                              color);
                }
            }
        }
    }
}

/* Parse the hex color once, then pass uint32_t to draw_char.
 * Handles '\n' for newlines. Advances x by cwidth × scale. */
void draw_text(struct surface *s,
               const char *text,
               int x, int y,
               const char *hex_color,
               int scale)
{
    if (!s || !text || scale < 1) return;

    uint32_t color = color_from_hex(hex_color);   /* once */
    int cx = x;

    for (int i = 0; text[i]; i++) {
        if (text[i] == '\n') {
            y  += font.cheight * scale;
            cx  = x;
        } else {
            draw_char(s, cx, y, text[i], color, scale);
            cx += font.cwidth * scale;
        }
    }
}

