#include "jpeglib.h"

int write_jpeg_file(const char *filename, int quality, int image_width, int image_height, JSAMPLE *image_buffer);
