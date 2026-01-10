#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define PNG_SIGNATURE_BYTES 8
#define IHDR_SIZE_BYTES 25
#define IDAT_CHUNKS_NUM 1028 * 1028 
#define IDAT_BEGINNING_BYTE (PNG_SIGNATURE_BYTES + IHDR_SIZE_BYTES)
#define LENGTH_BYTES 4

typedef uint8_t     U8;
typedef uint32_t    U32;
typedef size_t      U64;
typedef int         I32;

typedef struct {
        U32 width;
        U32 height;
        U32 crc;
        U8 bit_depth;
        U8 color_type;
        U8 compression_method;
        U8 filter_method;
        U8 interlace_method;
} png_ihdr;

typedef struct {
	U8 type[LENGTH_BYTES];
	U8 *data;
	U32 length;
	U32 crc;
} png_idat;

typedef enum {
	IDAT,
	IEND
} chunk_type;

const char *idat = "IDAT";
const char *iend = "IEND";

void            report_invalid_file(const char *filename, FILE *fp);
void            verify_png_sig(FILE *fp, I32 *f_size);
bool            is_png_sig(U8 *buf);
U32 	        to_little_endian(U8 *u);
png_ihdr        get_png_ihdr(FILE *fp);
void 		    read_idat_chunks(FILE *fp, png_idat **idat_chunks, U64 *out_size);
I32 		    is_idat_or_iend(U8 *chunk);
char*           load_png(FILE *fp);
void            quit(SDL_Window *window, SDL_Renderer *renderer);

SDL_Window*     window;
SDL_Renderer*   renderer;
static bool     running;
static I32 	    window_width = 800, window_height = 600;
static char*    filename;

int main(int argc, char **argv) {
        if (argc != 2) {
		fprintf(stderr, "usage: ./png \"image.png\"\n");
		return EXIT_FAILURE;
        }

        else if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "failed to initialize SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
        }

        filename = argv[1];
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "failed to open file \"%s\", error occurred or file non-existent\n", filename);
		return EXIT_FAILURE;
	}
        
        I32 f_size;
        verify_png_sig(fp, &f_size);
	
	window = SDL_CreateWindow(
		"PNG Image Viewer",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height,
		SDL_WINDOW_SHOWN
	);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	running = 1;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event))
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
			}
	}
	
	return EXIT_SUCCESS;
}

void report_invalid_file(const char *filename, FILE *fp) {
	fprintf(stderr, "ERROR: file \"%s\" is not png\n", filename);
	fclose(fp);
	exit(EXIT_FAILURE);
}

void verify_png_sig(FILE *fp, I32 *f_size) {
        fseek(fp, 0, SEEK_END);
	I32 f_len = ftell(fp);
	if (f_len < PNG_SIGNATURE_BYTES || f_len < 0)
		report_invalid_file(filename, fp);
	rewind(fp);
	
	U8 png_sig[PNG_SIGNATURE_BYTES];
	fread(png_sig, 1, PNG_SIGNATURE_BYTES, fp);
	if (!is_png_sig(png_sig))
		report_invalid_file(filename, fp);
                
        *f_size = f_len;
}

bool is_png_sig(U8 *buf) {
	return buf[0] == 0x89 && buf[1] == 0x50 && buf[2] == 0x4E && buf[3] == 0x47
		&& buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A;
}

U32 to_little_endian(U8 *u) {
	return (u[0] << 24) | (u[1] << 16) |
		(u[2] << 8) | (u[3]);
}

png_ihdr get_png_ihdr(FILE *fp) {
        png_ihdr ihdr;
	U8 ihdr_buf[IHDR_SIZE_BYTES]; 

	fseek(fp, PNG_SIGNATURE_BYTES, SEEK_SET);
	fread(ihdr_buf, 1, IHDR_SIZE_BYTES, fp);

	ihdr.width = to_little_endian(&ihdr_buf[0]);
	ihdr.height = to_little_endian(&ihdr_buf[4]);
	ihdr.bit_depth = ihdr_buf[8];
	ihdr.color_type = ihdr_buf[9];
	ihdr.compression_method = ihdr_buf[10];
	ihdr.filter_method = ihdr_buf[11];
	ihdr.interlace_method = ihdr_buf[12];
        ihdr.crc = to_little_endian(&ihdr_buf[13]);

	return ihdr;
}

I32 is_idat(U8 *chunk) {
        I32 i;

        for (i = 0; idat[i] != '\0' && chunk[i] == idat[i]; i++)
                ;

        if (idat[i] == '\0')
                return 0;
        else return 1;
}

I32 is_iend(U8 *chunk) {
	I32 i;
	
	for (i = 0; iend[i] != '\0' && chunk[i] == iend[i]; i++)
		;
	
	if (iend[i] == '\0')
		return 1;
	else return 0;
}

void read_idat_chunks(FILE *fp, png_idat **idat_chunks, U64 *out_size) {
	*idat_chunks = malloc(IDAT_CHUNKS_NUM * sizeof(png_idat));
	if (!*idat_chunks) {
		*out_size = 0;
		return;
	}

	U8 type[LENGTH_BYTES];
	U8 length[LENGTH_BYTES];
	chunk_type ch_type;
		
	fseek(fp, IDAT_BEGINNING_BYTE, SEEK_SET);
	fread(length, 1, LENGTH_BYTES, fp);

	fseek(fp, LENGTH_BYTES, SEEK_CUR);
	fread(type, 1, LENGTH_BYTES, fp);

	// loop through the chunks	
}
