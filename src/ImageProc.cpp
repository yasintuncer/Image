
#include "ImageProc.h"
#include "Channel.h"
#include "Color.h"
#include "Channel.h"
#include <stdint.h>
#include <cmath>
#include <execution>
#include <algorithm>

// ------------------------------------------------------------ HELPER FUNCTIONS ------------------------------------------------------------
namespace
{
    void downsample(uint8_t *src, uint8_t *dst, unsigned int src_width, unsigned int src_height, unsigned int dest_width, unsigned int dest_height)
    {
        // calculate the kernel size
        unsigned int kernel_width = std::floor(src_width / dest_width);
        unsigned int kernel_height = std::floor(src_height / dest_height);
        unsigned int kernel_size = kernel_width * kernel_height;
        // calculate the left over pixels
        unsigned int width_diff = src_width - (dest_width * kernel_width);
        unsigned int height_diff = src_height - (dest_height * kernel_height);
        unsigned int left_width = width_diff > 0 ? 1 : 0;
        unsigned int left_height = height_diff > 0 ? 1 : 0;

        // calculate the average of the kernel without the left over pixels
        for (unsigned int row = 0; row < dest_height - left_height; row++)
        {
            for (unsigned int col = 0; col < dest_width - left_width; col++)
            {
                double sum = 0;
                for (unsigned int y = 0; y < kernel_height; y++)
                {
                    for (unsigned int x = 0; x < kernel_width; x++)
                    {
                        sum += src[(row * kernel_height + y) * src_width + (col * kernel_width + x)];
                    }
                }
                dst[row * dest_width + col] = sum / kernel_size;
            }
        }

        // calculate the average of the kernel with the left over pixels
        if (width_diff > 0)
        {

            unsigned int kernel_width_with_left = kernel_width + width_diff;
            for (unsigned int row = 0; row < dest_height - left_height; row++)
            {
                double sum = 0;
                unsigned int start_x = src_width - kernel_width_with_left;
                for (unsigned int y = 0; y < kernel_height; y++)
                {
                    for (unsigned int x = 0; x < kernel_width_with_left; x++)
                    {
                        sum += src[(row * kernel_height + y) * src_width + start_x + x];
                    }
                }
                dst[row * dest_width + dest_width - 1] = sum / (kernel_width_with_left * kernel_height);
            }
        }
        if (height_diff > 0)
        {
            unsigned int kernel_height_with_left = kernel_height + height_diff;
            for (unsigned int col = 0; col < dest_width - left_width; col++)
            {
                double sum = 0;
                unsigned int start_y = src_height - kernel_height_with_left;
                for (unsigned int y = 0; y < kernel_height_with_left; y++)
                {
                    for (unsigned int x = 0; x < kernel_width; x++)
                    {
                        sum += src[(start_y + y) * src_width + (col * kernel_width + x)];
                    }
                }
                dst[(dest_height - 1) * dest_width + col] = sum / (kernel_width * kernel_height_with_left);
            }
        }
        if (width_diff > 0 && height_diff > 0)
        {
            unsigned int kernel_width_with_left = kernel_width + width_diff;
            unsigned int kernel_height_with_left = kernel_height + height_diff;
            double sum = 0;
            unsigned int start_x = src_width - kernel_width_with_left;
            unsigned int start_y = src_height - kernel_height_with_left;
            for (unsigned int y = 0; y < kernel_height_with_left; y++)
            {
                for (unsigned int x = 0; x < kernel_width_with_left; x++)
                {
                    sum += src[(start_y + y) * src_width + start_x + x];
                }
            }
            dst[(dest_height - 1) * dest_width + dest_width - 1] = sum / (kernel_width_with_left * kernel_height_with_left);
        }
    }

}
IMAGE_NAMESPACE_BEGIN
namespace ImageProc
{

    //--------------------------------------------------------------------------------------------------
    // Interleave RGB data from R,G,B channels
    void interLeaveRGBFromRGB(uint8_t *R, uint8_t *G, uint8_t *B, uint8_t *dest, unsigned int count)
    {

        for (uint32_t i = 0; i < count; i++)
        {
            dest[i * 3] = R[i];
            dest[i * 3 + 1] = G[i];
            dest[i * 3 + 2] = B[i];
        }
    }

    //--------------------------------------------------------------------------------------------------
    // Interleave RGB data from C,M,Y,K channels
    void interLeaveRGBFromCMYK(uint8_t *C, uint8_t *M, uint8_t *Y, uint8_t *K, uint8_t *dest, unsigned int count)
    {

        for (unsigned int i = 0; i < count; i++)
        {
            Color::Cmyk cmyk;
            Color::Rgb rgb;
            cmyk.c = (1.0 - (double)C[i] / 255.0) * 100.0;
            cmyk.m = (1.0 - (double)M[i] / 255.0) * 100.0;
            cmyk.y = (1.0 - (double)Y[i] / 255.0) * 100.0;
            cmyk.k = (1.0 - (double)K[i] / 255.0) * 100.0;

            cmyk.To(rgb);
            dest[i * 3] = (uint8_t)(rgb.r);
            dest[i * 3 + 1] = (uint8_t)(rgb.g);
            dest[i * 3 + 2] = (uint8_t)(rgb.b);
        }
    }

    //--------------------------------------------------------------------------------------------------
    // Interleave RGB data from L,A,B channels
    void interleaveRGBFromLAB(uint8_t *L, uint8_t *A, uint8_t *B, uint8_t *dest, unsigned int count)
    {
        for (unsigned int i = 0; i < count; i++)
        {
            Color::Lab lab;
            lab.l = (double)L[i] / 255.0 * 100.0;
            lab.a = (double)A[i] / 255.0 * 255.0 - 128.0;
            lab.b = (double)B[i] / 255.0 * 255.0 - 128.0;

            // lab.To<ColorSpace::Rgb>(&rgb);

            // dest[i * 3] = (uint8_t)(rgb.r);
            // dest[i * 3 + 1] = (uint8_t)(rgb.g);
            // dest[i * 3 + 2] = (uint8_t)(rgb.b);
        }
    }

    //--------------------------------------------------------------------------------------------------
    // Interleave RGB data from H,S,V channels
    void interleaveRGBFromHSV(uint8_t *H, uint8_t *S, uint8_t *V, uint8_t *dest, unsigned int count)
    {
        for (unsigned int i = 0; i < count; i++)
        {
            Color::Hsv hsv;
            hsv.h = (double)H[i] / 255.0 * 360.0;
            hsv.s = (double)S[i] / 255.0 * 100.0;
            hsv.v = (double)V[i] / 255.0 * 100.0;
            Color::Rgb rgb;
            hsv.To(rgb);

            dest[i * 3] = (uint8_t)(rgb.r);
            dest[i * 3 + 1] = (uint8_t)(rgb.g);
            dest[i * 3 + 2] = (uint8_t)(rgb.b);
        }
    }

    //--------------------------------------------------------------------------------------------------
    // Interleave RGB data from multichannel
    void interleaveRGBFromMultichannel(Channel **channels, uint8_t *dest, unsigned int channel_count, unsigned int count)
    {
        Color::Rgb color_table;

        for (unsigned int i = 0; i < channel_count; i++)
        {
            // Get color table

            if (channels[i]->colorspace == Channel::RGB)
            {
                color_table.r = channels[i]->color[0];
                color_table.g = channels[i]->color[1];
                color_table.b = channels[i]->color[2];
            }
            else if (channels[i]->colorspace == Channel::CMYK)
            {
                // Convert CMYK to RGB
                Color::Cmyk cmyk(channels[i]->color[0], channels[i]->color[1], channels[i]->color[2], channels[i]->color[3]);
                cmyk.To(color_table);
            }
            else if (channels[i]->colorspace == Channel::HSB)
            { // HSB is the same as HSV
                // Convert HSB to RGB
                Color::Hsv hsv(channels[i]->color[0], channels[i]->color[1], channels[i]->color[2]);
                hsv.To(color_table);
            }
            else if (channels[i]->colorspace == Channel::LAB)
            { // Convert LAB to RGB
                Color::Lab lab(channels[i]->color[0], channels[i]->color[1], channels[i]->color[2]);
                lab.To(color_table);
            }

            // round to nearest integer
            color_table.r = std::round(color_table.r);
            color_table.g = std::round(color_table.g);
            color_table.b = std::round(color_table.b);

            double solidity = (double)channels[i]->opacity / 100.0;
            for (unsigned int j = 0; j < count; j++)
            {

                double pixel = (double)(1.0 - channels[i]->data[j] / 255.0) * solidity;

                dest[j * 3u + 0u] = (uint8_t)((pixel * color_table.r) + (1.0 - pixel) * (double)dest[j * 3u + 0u]);
                dest[j * 3u + 1u] = (uint8_t)((pixel * color_table.g) + (1.0 - pixel) * (double)dest[j * 3u + 1u]);
                dest[j * 3u + 2u] = (uint8_t)((pixel * color_table.b) + (1.0 - pixel) * (double)dest[j * 3u + 2u]);
            }
        }
    }

    //--------------------------------------------------------------------------------------------------
    // resize image
    int resizeImage(Image *src, Image *dest, unsigned int dest_width, unsigned int dest_height, bool keep_aspect_ratio)
    {
        if (src == nullptr || dest == nullptr)
            return -1;

        if (src->width == dest_width && src->height == dest_height)
        {
            // No resize needed
            dest = src;
            return 0;
        }
        if (src->width < dest_width || src->height < dest_height)
        {
            // Upscaling not supported
            return -1;
        }

        if (keep_aspect_ratio)
        {
            // Calculate new width and height
            double aspect_ratio = (double)src->width / (double)src->height;
            if (aspect_ratio > 1.0)
            {
                dest_width = dest_height * aspect_ratio;
            }
            else
            {
                dest_height = dest_width / aspect_ratio;
            }
        }
        dest->height = dest_height;
        dest->width = dest_width;
        dest->channelCount = src->channelCount;
        dest->colorMode = src->colorMode;
        dest->channels = new Channel *[src->channelCount];

        for (unsigned int i = 0; i < src->channelCount; i++)
        {
            dest->channels[i] = new Channel();
            dest->channels[i]->data = new uint8_t[dest_width * dest_height];
            dest->channels[i]->colorspace = src->channels[i]->colorspace;
            dest->channels[i]->color[0] = src->channels[i]->color[0];
            dest->channels[i]->color[1] = src->channels[i]->color[1];
            dest->channels[i]->color[2] = src->channels[i]->color[2];
            dest->channels[i]->color[3] = src->channels[i]->color[3];
            dest->channels[i]->opacity = src->channels[i]->opacity;
            downsample(src->channels[i]->data, dest->channels[i]->data, src->width, src->height, dest_width, dest_height);
        }
        return 0;
    }

}
IMAGE_NAMESPACE_END
