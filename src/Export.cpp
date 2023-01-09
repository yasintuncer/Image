#include <png.h>
#include <jpeglib.h>
#include <iostream>
#include <cstring>
#include "Export.h"

//--------------------------------- Helper functions ---------------------------------
namespace
{
    int write_to_png_file(const char *filename, int width, int height, uint8_t *interleaved_buffer)
    {
        png_structp png_ptr;
        png_infop info_ptr;
        png_bytep row;
        png_bytep *row_pointers;
        int i;
        FILE *fp = fopen(filename, "wb");
        if (!fp)
        {
            return -11;
        }
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr)
        {
            return -1;
        }
        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            return -11;
        }
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            return 1;
        }
        png_init_io(png_ptr, fp);
        png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        png_write_info(png_ptr, info_ptr);
        row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
        for (i = 0; i < height; i++)
        {
            row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
            memcpy(row_pointers[i], interleaved_buffer + i * width * 3, width * 3);
        }
        png_write_image(png_ptr, row_pointers);
        png_write_end(png_ptr, NULL);
        for (i = 0; i < height; i++)
        {
            free(row_pointers[i]);
        }
        free(row_pointers);
        fclose(fp);
        return 0;
    }

    int write_to_jpeg_file(const char *filename, int width, int height, uint8_t *interleaved_buffer, int quality)
    {
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;
        FILE *outfile;           /* target file */
        JSAMPROW row_pointer[1]; /* pointer to JSAMPLE row[s] */
        int row_stride;          /* physical row width in image buffer */
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);
        if ((outfile = fopen(filename, "wb")) == NULL)
        {
            std::cout << "Error: Failed to open file" << std::endl;
            return -1;
        }
        jpeg_stdio_dest(&cinfo, outfile);
        cinfo.image_width = width; /* image width and height, in pixels */
        cinfo.image_height = height;
        cinfo.input_components = 3;     /* # of color components per pixel */
        cinfo.in_color_space = JCS_RGB; /* colorspace of input image */
        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
        jpeg_start_compress(&cinfo, TRUE);
        row_stride = width * 3; /* JSAMPLEs per row in image_buffer */
        while (cinfo.next_scanline < cinfo.image_height)
        {
            row_pointer[0] = &interleaved_buffer[cinfo.next_scanline * row_stride];
            (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }
        jpeg_finish_compress(&cinfo);
        fclose(outfile);
        jpeg_destroy_compress(&cinfo);
        return 0;
    }
}
IMAGE_NAMESPACE_BEGIN
namespace Export
{
    int export_image(const char *filename, Image *image, ExportAs format, int quality = 100)
    {
        // Check image has been interlaved
        if (image->interleavedRgb == nullptr)
        {
            // if not, interleave it
            if (image->InterleaveRGB() == nullptr)
            {
                std::cout << "Error: Failed to interleave RGB" << std::endl;
                std::cout << "Error: Failed to export image" << std::endl;
                return -1;
            }
        }

        int result = 0;
        if (format == PNG)
        {
            result = write_to_png_file(filename, image->width, image->height, image->interleavedRgb);
        }
        else if (format == JPEG)
        {
            result = write_to_jpeg_file(filename, image->width, image->height, image->interleavedRgb, quality);
        }
        return result;
    }
}

IMAGE_NAMESPACE_END
