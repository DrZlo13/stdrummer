#include <core.h>
#include <stdint.h>
#include "fonts/font.h"
#include "fonts/fonts_basic.h"
#include <string_view>

typedef struct {
    const uint16_t width;
    const uint16_t height;
    const uint8_t* data;
} BitmapBW;

typedef struct {
    int32_t x;
    int32_t y;
} Point;

template <class Display> class GFX {
private:
    Display display;
    const Font* current_font = NULL;
    int32_t current_text_x;
    int32_t current_text_y;

public:
    typedef Display::Color Color;

    template <typename... Args>
    GFX(Args&&... args)
        : display(args...) {
    }

    ~GFX() {
    }

    /**
     * Init display
     * @param mirror_v 
     * @param mirror_h 
     * @param inverse 
     */
    void start(void) {
        display.start();
    }

    /**
     * Fill with color
     * @param color 
     */
    void fill(Color color) {
        display.fill(color);
    }

    /**
     * Send data to display
     */
    void flush() {
        display.flush();
    }

    /**
     * Check that sending is complete
     * @return true 
     * @return false 
     */
    bool flush_completed() {
        return display.flush_completed();
    }

    /**
     * Set display brightness
     * @param contrast 
     */
    void set_brightness(uint8_t contrast) {
        display.set_brightness(contrast);
    }

    /**
     * On\off display
     * @param on 
     */
    void set_display_on(bool on) {
        display.set_display_on(on);
    }

    /**
     * Set pixel
     * @param x 
     * @param y 
     * @param color 
     */
    void draw_pixel(int32_t x, int32_t y, Color color) {
        display.set_pixel(x, y, color);
    }

    /**
     * Draw line
     * @param x1 
     * @param y1 
     * @param x2 
     * @param y2 
     * @param color 
     */
    void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Color color) {
        int32_t deltaX = abs(x2 - x1);
        int32_t deltaY = abs(y2 - y1);
        int32_t signX = ((x1 < x2) ? 1 : -1);
        int32_t signY = ((y1 < y2) ? 1 : -1);
        int32_t error = deltaX - deltaY;
        int32_t error2;

        display.set_pixel(x2, y2, color);
        while((x1 != x2) || (y1 != y2)) {
            display.set_pixel(x1, y1, color);
            error2 = error * 2;
            if(error2 > -deltaY) {
                error -= deltaY;
                x1 += signX;
            }

            if(error2 < deltaX) {
                error += deltaX;
                y1 += signY;
            }
        }
    }

    /**
     * Draw rectangle
     * @param x1 
     * @param y1 
     * @param x2 
     * @param y2 
     * @param color 
     */
    void draw_rectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Color color) {
        draw_line(x1, y1, x2, y1, color);
        draw_line(x2, y1, x2, y2, color);
        draw_line(x2, y2, x1, y2, color);
        draw_line(x1, y2, x1, y1, color);
    }

    /**
     * Draw circle
     * @param x 
     * @param y 
     * @param r 
     * @param color 
     */
    void draw_circle(int32_t x, int32_t y, int32_t r, Color color) {
        int32_t tx = -r;
        int32_t ty = 0;
        int32_t err = 2 - 2 * r;
        int32_t e2;

        do {
            display.set_pixel(x - tx, y + ty, color);
            display.set_pixel(x + tx, y + ty, color);
            display.set_pixel(x + tx, y - ty, color);
            display.set_pixel(x - tx, y - ty, color);
            e2 = err;
            if(e2 <= ty) {
                ty++;
                err = err + (ty * 2 + 1);
                if(-tx == ty && e2 <= tx) {
                    e2 = 0;
                }
            }
            if(e2 > tx) {
                tx++;
                err = err + (tx * 2 + 1);
            }
        } while(tx <= 0);
    }

    /**
     * Draw bitmap
     * @param x 
     * @param y 
     * @param bitmap 
     * @param color 
     */
    void draw_bitmap_bw(int32_t x, int32_t y, const BitmapBW* bitmap, Color color) {
        int16_t byteWidth = (bitmap->width + 7) / 8;
        uint8_t byte = 0;

        for(uint8_t j = 0; j < bitmap->height; j++, y++) {
            for(uint8_t i = 0; i < bitmap->width; i++) {
                if(i & 7)
                    byte <<= 1;
                else
                    byte = bitmap->data[j * byteWidth + i / 8];
                if(byte & 0x80) display.set_pixel(x + i, y, color);
            }
        }
        return;
    }

    void draw_polyline(
        int32_t x,
        int32_t y,
        const Point* vertices,
        size_t vertex_count,
        Color color) {
        if(vertices != NULL) {
            for(size_t i = 1; i < vertex_count; i++) {
                draw_line(
                    x + vertices[i - 1].x,
                    y + vertices[i - 1].y,
                    x + vertices[i].x,
                    y + vertices[i].y,
                    color);
            }
        }
    }

    void set_font(const Font* font) {
        current_font = font;
    }

    void draw_char(char ch, Color color_fg, Color color_bg) {
        uint32_t i, b, j;

        if(current_font == NULL) return;
        if(ch < ' ' || ch > '~') return;

        for(i = 0; i < current_font->height; i++) {
            b = current_font->data[(ch - 32) * current_font->height + i];
            for(j = 0; j < current_font->width; j++) {
                if((b << j) & 0x8000) {
                    display.set_pixel(current_text_x + j, (current_text_y + i), color_fg);
                } else {
                    display.set_pixel(current_text_x + j, (current_text_y + i), color_bg);
                }
            }
        }

        current_text_x += current_font->width;
    }

    void draw_string(const char* str, Color color_fg, Color color_bg) {
        while(*str) {
            draw_char(*str, color_fg, color_bg);
            str++;
        }
    }

    void draw_string(const std::string_view str, Color color_fg, Color color_bg) {
        for(size_t i = 0; i < str.size(); i++) {
            draw_char(str[i], color_fg, color_bg);
        }
    }

    // Position the cursor
    void set_cursor(int32_t x, int32_t y) {
        current_text_x = x;
        current_text_y = y;
    }
};
