#!/usr/bin/env sh
set +xe
cc mandelbrot2png.c -Wall -Wextra -Werror -pedantic -o mandelbrot2png -lm
