#include <stdio.h>
#include <complex.h>
#include <stdint.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image_write.h"

#define MAX_ITERATION 1000

struct RGBA {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

static size_t width,height = 800;
static char* output_path  = "mandelbrot.png";
static int verbose = 0;

// scalling a float color [0..1] to rgba;
struct RGBA get_rgba_from_float(float color){
	return (struct RGBA) {
		.r = color * 255,
		.g = color * 255,
		.b = color * 255,
		.a = 255,
	};
}

struct RGBA hsv_to_rgba(float h, float s, float v)
{
    float r, g, b;
    int i = (int)(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }
    return (struct RGBA) {
        .r = (uint8_t)(r * 255),
        .g = (uint8_t)(g * 255),
        .b = (uint8_t)(b * 255),
        .a = 255 // Fully opaque
    };
}

struct RGBA map_color(float t)
{
    float h = fmod(t * 10, 1.0);  // Hue cycles every 10 iterations
    float s = 1.0;               // Full saturation
    float v = t < 0.5 ? 2 * t : 1.0; // Brightness increases with t
    return hsv_to_rgba(h, s, v);
}

char* shift(int* argc, char*** argv)
{
	char* result = *argv[0];
	++*argv, --*argc;
	return result;
}

void usage(char* program)
{
	printf("Usage: %s [OPTION].. [OUTPUT_PATH]\nGenerate mandelbrot set png file\n"
	"\t-w, --width\t\t png image width\n"
	"\t-h, --height\t\t png image height\n"
	"\t-v, --verbose\t\t enable logging\n"
	"\t-h, --help\t\t print this help message and exit\n"
	,program);
}

int main(int argc, char** argv)
{
	char* program = shift(&argc, &argv);
	int is_output_path_set = 0;
	while (argc > 0){
		char* arg = shift(&argc, &argv);
		if (!strcmp(arg,"-w") || !strcmp(arg,"--width")){
			if (argc <= 0){
				fprintf(stderr,"missing value for --width\n");
				exit(1);
			}
			arg = shift(&argc, &argv);
			width = atoll(arg);
			if (!width) fprintf(stderr,"--width expects an integer\n"),exit(1);

		}else if (!strcmp(arg,"-h") || !strcmp(arg,"--height")){
			if (argc <= 0){
				fprintf(stderr,"missing value for --height\n");
				exit(1);
			}
			arg = shift(&argc, &argv);
			height = atoll(arg);
			if (!height) fprintf(stderr,"--height expects an integer\n"),exit(1);
		}else if (!strcmp(arg,"-v") || !strcmp(arg,"--verbose")){
			verbose = 1;
		}
		else if (!strcmp(arg,"--help")){
			usage(program);
			exit(0);
		}else if(arg[0] == '-'){
			fprintf(stderr,"Unknown Option: %s\n" "Try '%s --help' for more information.",arg,program);
			exit(1);
		}else{
			if (!is_output_path_set){
				output_path = arg;
				is_output_path_set = 1;
			}else {
				fprintf(stderr,"Unknown Option: %s\n" "Try '%s --help' for more information.\n",arg,program);
				exit(1);
			}
		}
	}
	struct RGBA pixels[width*height];
	const double complex z0 = 0+I*0;
	const float re_min = -2.0f;
	const float re_max = 1.0f;
	const float im_min = -1.5f;
	const float im_max = 1.5f;
	double complex zn;
	double complex	z_next;
	double complex c;
	int i;
	int px,py;
	float x,y;
	float color;
	if (verbose) fprintf(stdout,"[INFO]: Generating Png Image %lux%lupx\n",width,height);
	for (py = 0; py < (int)height; ++py){
		for (px = 0; px < (int)width; ++px) {
			x = re_min + ((float)(px) / width) * (re_max - re_min);
			y = im_min + ((float)(py) / height) * (im_max - im_min);
			c = x + y * I;
			i = 0;
			zn = z0;
			while(cabs(zn) < 2 && i < MAX_ITERATION){
				z_next = zn * zn + c;
				zn = z_next;
				i++;
			}
			color = (float)i / MAX_ITERATION;
			pixels[py * height + px] = map_color(color);
		}
	}
	if (verbose) fprintf(stdout,"[INFO]: Writing Png Image to %s\n",output_path);
	stbi_write_png(output_path, width, height, 4, &pixels, sizeof(struct RGBA) * width);
	if (verbose) fprintf(stdout,"Done.\n");
	return 0;
}
