#include "Geometry.h"

namespace geometry {
    /**
     * Draws a specific pixel in buffer
     */
    void put_pixel(PixelBuffer& buffer, int x, int y, std::uint32_t color)
    {
        if (x < 0 || x >= buffer.width || y < 0 || y >= buffer.height) {
            return;
        }

        const int index = y * buffer.width + x;
        buffer.pixels[index] = color;
    }

    /**
     * Draws a rectangle inside buffer
     */
    void draw_rectangle(
        PixelBuffer& buffer,
        int x,
        int y,
        int width,
        int height,
        std::uint32_t color
    )
    {
        for (int py = y; py < y + height; ++py) {
            for (int px = x; px < x + width; ++px) {
                put_pixel(buffer, px, py, color);
            }
        }
    }

    /**
     * Draws a line from source to destination using Bresenham's algorithm
     */
    void draw_line(
        PixelBuffer& buffer,
        int x0,
        int y0,
        int x1,
        int y1,
        std::uint32_t color
    )
    {
        const int dx = std::abs(x1 - x0);
        const int step_x = x0 < x1 ? 1 : -1;
        const int dy = -std::abs(y1 - y0);
        const int step_y = y0 < y1 ? 1 : -1;
        int error = dx + dy;

        while (true) {
            put_pixel(buffer, x0, y0, color);

            if (x0 == x1 && y0 == y1) {
                break;
            }

            const int doubled_error = 2 * error;

            if (doubled_error >= dy) {
                error += dy;
                x0 += step_x;
            }

            if (doubled_error <= dx) {
                error += dx;
                y0 += step_y;
            }
        }
    }
}