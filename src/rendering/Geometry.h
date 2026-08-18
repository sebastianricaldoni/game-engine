#pragma once

#include <cstdint>
#include "PixelBuffer.h"

namespace geometry
{
    void put_pixel(PixelBuffer &buffer, int x, int y, std::uint32_t color);
    void draw_rectangle(PixelBuffer &buffer, int x, int y, int width, int height, std::uint32_t color);
    void draw_line( PixelBuffer &buffer, int x0, int y0, int x1, int y1, std::uint32_t color);
}