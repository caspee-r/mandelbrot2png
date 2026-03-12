# Mandelbrot2PNG

A small C program that generates a **PNG image of the Mandelbrot set**.

## Build

Use the provided build script:

```sh
./build.sh
```

## Usage

```
./mandelbrot2png [OPTIONS] [OUTPUT_PATH]
```

### Options

| Option                   | Description                        |
| ------------------------ | ---------------------------------- |
| `-w`, `--width`          | Output image width                 |
| `-h`, `--height`         | Output image height                |
| `-m`, `--max-iterations` | Maximum Mandelbrot iteration count |
| `-v`, `--verbose`        | Enable logging                     |
| `-h`, `--help`           | Print help message and exit        |

### Example

```sh
./mandelbrot2png -w 1920 -h 1080 -m 1000 mandelbrot.png
```

This generates a `1920×1080` Mandelbrot image with a maximum of `1000` iterations.
