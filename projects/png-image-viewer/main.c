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

typedef struct
{
    U32 width;
    U32 height;
    U8 *pixels;
} Image;

void    sdl_context_init(SDL_Window **window, SDL_Renderer **renderer);
void    sdl_exit(SDL_Window *window, SDL_Renderer *renderer);
Image   load_image(const char *path); 
void    free_image(Image *img);

// helper functions

void *hmalloc(size_t size);

int
main(int argc, char **argv)
{
    if (argc != 2) { fprintf(stderr, "usage.\n"); exit(EXIT_FAILURE); }

    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_context_init(&window, &renderer);
   
    sdl_exit(window, renderer);
}

void
sdl_context_init(SDL_Window **window, SDL_Renderer **renderer)
{
    int flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        exit(EXIT_FAILURE);
    if ((IMG_Init(flags) & flags) != flags)
        exit(EXIT_FAILURE);
    
    *window = SDL_CreateWindow(
        "SDL2 Image Viewer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
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
free_image(Image *img)
{
    if (img && img->pixels)
    {
        free(img->pixels);
        img->pixels = NULL;
        img->width = img->height = 0U;
    }
}
