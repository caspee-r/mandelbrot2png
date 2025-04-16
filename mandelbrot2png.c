#include <stdio.h>
#include <complex.h>
#include <stdint.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image_write.h"

#define MAX_ITERATION 1000
#define WIDTH 1000
#define HEIGHT 1000

struct RGBA {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

// array of 32bit pixels;
static struct RGBA pixels[WIDTH*HEIGHT];

// scalling a float color [0..1] to rgba;
struct RGBA get_rgba_from_float(float color){
	return (struct RGBA) {
		.r = color * 255,
		.g = color * 255,
		.b = color * 255,
		.a = 255,
	};
}

struct RGBA hsv_to_rgba(float h, float s, float v) {
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

struct RGBA map_color(float t) {
    float h = fmod(t * 10, 1.0);  // Hue cycles every 10 iterations
    float s = 1.0;               // Full saturation
    float v = t < 0.5 ? 2 * t : 1.0; // Brightness increases with t
    return hsv_to_rgba(h, s, v);
}


int main(){
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
	const char* filename = "mandelbrot.png";
	// time complexity is O()
	for (py = 0; py < HEIGHT; ++py){ //HEIGHT*WIDTH
		for (px = 0; px < WIDTH; ++px) {
			x = re_min + ((float)(px) / WIDTH) * (re_max - re_min);
			y = im_min + ((float)(py) / HEIGHT) * (im_max - im_min);
			c = x + y * I;
			i = 0;
			zn = z0;
			while(cabs(zn) < 2 && i < MAX_ITERATION){
				z_next = zn * zn + c;
				zn = z_next;
				i++;
			}
			color = (float)i / MAX_ITERATION;
			pixels[py * HEIGHT + px] = map_color(color);
		}
	}
	stbi_write_png(filename, WIDTH, HEIGHT, 4, &pixels, sizeof(struct RGBA) * WIDTH);
	return 0;
}
