#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef uint32_t U32;
typedef uint8_t U8;
typedef int32_t I32;

typedef struct
{
    U32 width;
    U32 height;
    U8 *pixels;
} Image;

void    sdl_context_init(SDL_Window **window, SDL_Renderer **renderer, U32 width, U32 height);
void    sdl_exit(SDL_Window *window, SDL_Renderer *renderer);
Image   load_image(const char *path); 
void    draw_image(SDL_Renderer *renderer, Image *img);
void    free_image(Image *img);

// helper functions

void *hmalloc(size_t size);

int
main(int argc, char **argv)
{
    if (argc != 2) { fprintf(stderr, "usage.\n"); exit(EXIT_FAILURE); }
    int flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        exit(EXIT_FAILURE);
    if ((IMG_Init(flags) & flags) != flags)
        exit(EXIT_FAILURE);

    SDL_Window *window;
    SDL_Renderer *renderer;

    Image img = load_image(argv[1]);
    sdl_context_init(&window, &renderer, img.width, img.height);
    if (!img.pixels) { fprintf(stderr, "failed to laod image\n"); 
        sdl_exit(window, renderer); }

    bool running = true;
    SDL_Event e;
    while (running)
    {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                running = false;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        draw_image(renderer, &img);
        SDL_RenderPresent(renderer);
    }

    free_image(&img);
    sdl_exit(window, renderer);
}

void
sdl_context_init(SDL_Window **window, SDL_Renderer **renderer, U32 width, U32 height)
{
    *window = SDL_CreateWindow(
        "SDL2 Image Viewer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN
    );

    if (!*window)
    {
        fprintf(stderr, "could not create SDL2 window...\n");
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    *renderer = SDL_CreateRenderer(
        *window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    
    if (!*renderer)
    {
        fprintf(stderr, "could not create the SDL2 renderer...\n");
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void
sdl_exit(SDL_Window *window, SDL_Renderer *renderer)
{
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    exit(EXIT_SUCCESS);
}

void *
hmalloc(size_t size)
{
    void *p = malloc(size);
    if (!p)
    {
        fprintf(stderr, "panic: not enough memory!\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

Image
load_image(const char *path)
{
    Image img = {0};
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
        return img;

    SDL_Surface *converted = SDL_ConvertSurfaceFormat(
        surface,
        SDL_PIXELFORMAT_RGBA32,
        0
    );

    SDL_FreeSurface(surface);
    if (!converted) return img;
    img.width = converted->w;
    img.height = converted->h;

    size_t size = (size_t)img.width * img.height * 4;
    img.pixels = hmalloc(size);
    memcpy(img.pixels, converted->pixels, size);

    SDL_FreeSurface(converted);
    return img;
}

void
draw_image(SDL_Renderer *renderer, Image *img)
{
    for (I32 y = 0; y < img->height; y++)
        for (I32 x = 0; x < img->width; x++)
        {
            I32 i = (y * img->width + x) * 4;

            I32 r = img->pixels[i + 0];
            I32 g = img->pixels[i + 1];
            I32 b = img->pixels[i + 2];
            I32 a = img->pixels[i + 3];
            
            SDL_SetRenderDrawColor(renderer, r, g, b, a);    
            SDL_RenderDrawPoint(renderer, x, y);
        }
}

void
free_image(Image *img)
{
    if (img && img->pixels)
    {
        free(img->pixels);
        img->pixels = NULL;
        img->width = img->height = 0U;
    }
}
