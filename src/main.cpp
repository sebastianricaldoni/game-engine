#include <SDL3/SDL.h>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

constexpr int BUFFER_WIDTH = 960;
constexpr int BUFFER_HEIGHT = 540;

struct PixelBuffer {
    int width;
    int height;
    std::vector<std::uint32_t> pixels;
};

void process_input(bool& running)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN &&
            event.key.key == SDLK_ESCAPE) {
            running = false;
        }
    }
}

void update_game()
{
    // Game simulation will go here.
}

void clear_pixel_buffer(PixelBuffer& buffer)
{
    constexpr std::uint32_t background_color = 0x001B263B;
    std::fill(buffer.pixels.begin(), buffer.pixels.end(), background_color);
}

void put_pixel(PixelBuffer& buffer, int x, int y, std::uint32_t color)
{
    if (x < 0 || x >= buffer.width || y < 0 || y >= buffer.height) {
        return;
    }

    const int index = y * buffer.width + x;
    buffer.pixels[index] = color;
}

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

void render_into_pixel_buffer(PixelBuffer& buffer)
{
    constexpr std::uint32_t yellow = 0x00FFD42D;
    constexpr std::uint32_t cyan = 0x003DFFF3;

    draw_rectangle(
        buffer,
        BUFFER_WIDTH / 2 - 50,
        BUFFER_HEIGHT / 2 - 30,
        100,
        60,
        yellow
    );

    const int top_x = BUFFER_WIDTH / 2;
    const int top_y = 100;
    const int left_x = top_x - 140;
    const int right_x = top_x + 140;
    const int bottom_y = 350;

    draw_line(buffer, top_x, top_y, left_x, bottom_y, cyan);
    draw_line(buffer, left_x, bottom_y, right_x, bottom_y, cyan);
    draw_line(buffer, right_x, bottom_y, top_x, top_y, cyan);
}

bool display_pixel_buffer(
    const PixelBuffer& buffer,
    SDL_Texture* texture,
    SDL_Renderer* renderer
)
{
    const int bytes_per_row =
        buffer.width * static_cast<int>(sizeof(std::uint32_t));

    if (!SDL_UpdateTexture(texture, nullptr, buffer.pixels.data(), bytes_per_row)) {
        return false;
    }

    if (!SDL_RenderClear(renderer)) {
        return false;
    }

    if (!SDL_RenderTexture(renderer, texture, nullptr, nullptr)) {
        return false;
    }

    return SDL_RenderPresent(renderer);
}

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "My First Game Window",
        960,
        540,
        SDL_WINDOW_RESIZABLE
    );

    if (window == nullptr) {
        std::cerr << "The window could not be created: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        std::cerr << "The renderer could not be created: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_XRGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        BUFFER_WIDTH,
        BUFFER_HEIGHT
    );

    if (texture == nullptr) {
        std::cerr << "The texture could not be created: " << SDL_GetError() << '\n';
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    PixelBuffer pixel_buffer{
        BUFFER_WIDTH,
        BUFFER_HEIGHT,
        std::vector<std::uint32_t>(BUFFER_WIDTH * BUFFER_HEIGHT)
    };

    bool running = true;
    while (running) {
        process_input(running);
        update_game();

        clear_pixel_buffer(pixel_buffer);
        render_into_pixel_buffer(pixel_buffer);

        if (!display_pixel_buffer(pixel_buffer, texture, renderer)) {
            std::cerr << "The pixel buffer could not be displayed: "
                      << SDL_GetError() << '\n';
            running = false;
        }

        SDL_Delay(1);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
