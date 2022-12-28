#include "Psd.h"
#include "PsdMallocAllocator.h"
#include "PsdColorMode.h"
#include "PsdAlphaChannel.h"
#include "PsdPlanarImage.h"
#include "PsdImageResourcesSection.h"
#include "PsdImageDataSection.h"
#include "PsdParseImageDataSection.h"
#include "PsdParseImageResourcesSection.h"
#include "Psdinttypes.h"
#include "DecodePsd.h"
#include <stdlib.h>
#include <cmath>
#include <cstring>
#include <iostream>

//-------------------- Helper functions --------------------
namespace
{
    // ---------------------------------------------------------------------------------------------
    // remapBuffer - remap buffer to 8-bit
    template <typename T>
    uint8_t *remapBuffer(T *buffer, unsigned int count)
    {
        double quantization_coef = 255.0 / std::pow(2, sizeof(T) * 8);
        uint8_t *new_buffer = (uint8_t *)malloc(count * sizeof(uint8_t));
        for (unsigned int i = 0; i < count; ++i)
        {
            new_buffer[i] = (uint8_t)(buffer[i] * quantization_coef);
        }
        return new_buffer;
    }

    // ---------------------------------------------------------------------------------------------
    // remap - remap value from one range to another
    template <typename T>
    double remap(T src, double src_max, double src_min, double dst_max, double dst_min)
    {
        return (src - src_min) / (src_max - src_min) * (dst_max - dst_min) + dst_min;
    }

    // ---------------------------------------------------------------------------------------------
    // CopyRemainingPixels - copy remaining pixels to destination buffer
    template <typename T>
    void CopyRemainingPixels(void *src, uint8_t *dst, unsigned int count)
    {
        if (sizeof(T) > sizeof(uint8_t))
        {
            const T *buffer = static_cast<const T *>(src);
            memcpy(dst, remapBuffer(buffer, count), count);
        }
        else
        {
            memcpy(dst, src, count);
        }
    }

    // ---------------------------------------------------------------------------------------------
    // CopyRemainingPixels - copy remaining pixels to destination buffer
    void GetColorTable(uint16_t *src, double *dst, uint16_t colorspace)
    {
        switch (colorspace)
        {
        case psd::COLOR_SPACE::RGB:
            dst[0] = remap(src[0], 65535.0, 0.0, 255.0, 0.0);
            dst[1] = remap(src[1], 65535.0, 0.0, 255.0, 0.0);
            dst[2] = remap(src[2], 65535.0, 0.0, 255.0, 0.0);
            dst[3] = remap(src[3], 65535.0, 0.0, 255.0, 0.0);
            break;
        case psd::COLOR_SPACE::CMYK:
            dst[0] = 100.0 - remap(src[0], 65535.0, 0.0, 100.0, 0.0);
            dst[1] = 100.0 - remap(src[1], 65535.0, 0.0, 100.0, 0.0);
            dst[2] = 100.0 - remap(src[2], 65535.0, 0.0, 100.0, 0.0);
            dst[3] = 100.0 - remap(src[3], 65535.0, 0.0, 100.0, 0.0);
            break;

        case psd::COLOR_SPACE::LAB:
            dst[0] = remap(src[0], 65535.0, 0.0, 100.0, 0.0);
            dst[1] = remap(src[1], 65535.0, 0.0, 255.0, -128.0);
            dst[2] = remap(src[2], 65535.0, 0.0, 255.0, -128.0);
            dst[3] = remap(src[3], 65535.0, 0.0, 255.0, 0.0);
            break;

        case psd::COLOR_SPACE::HSB:
            dst[0] = remap(src[0], 65535.0, 0.0, 360.0, 0.0);
            dst[1] = remap(src[1], 65535.0, 0.0, 100.0, 0.0);
            dst[2] = remap(src[2], 65535.0, 0.0, 100.0, 0.0);
            dst[3] = remap(src[3], 65535.0, 0.0, 255.0, 0.0);
            break;
        }
    }
}
namespace Image
{
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    int createImage(psd::NativeFile *file, psd::Allocator *allocator, psd::Document *doc, Image *image)
    {
        psd::ImageDataSection *dataSection = psd::ParseImageDataSection(doc, file, allocator);
        psd::ImageResourcesSection *resourcesSection = psd::ParseImageResourcesSection(doc, file, allocator);
        if (!dataSection || !resourcesSection)
        {
            return -1;
        }

        int result = 0;
        switch (doc->colorMode)
        {
        case psd::colorMode::BITMAP:
            /* code */
            result = -1;
            break;
        case psd::colorMode::GRAYSCALE:
            /* code */
            result = -1;
            break;
        case psd::colorMode::INDEXED:
            /* code */
            result = -1;
            break;
        case psd::colorMode::RGB:

            for (unsigned int i = 0; i < doc->channelCount; ++i)
            {
                image->channels[i] = new Channel();
                image->channels[i]->data = (uint8_t *)malloc(doc->width * doc->height * sizeof(uint8_t));
                image->channels[i]->size = doc->width * doc->height * sizeof(uint8_t);
                if (doc->bitsPerChannel == 8)
                {
                    CopyRemainingPixels<uint8_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 16)
                {
                    CopyRemainingPixels<uint16_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 32)
                {
                    CopyRemainingPixels<float32_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }

                GetColorTable(resourcesSection->alphaChannels[i].color, image->channels[i]->color, resourcesSection->alphaChannels[i].colorSpace);
            }
            image->width = doc->width;
            image->height = doc->height;
            image->channelCount = doc->channelCount;
            image->colorSpace = ColorSpace::RGB;
            break;

        case psd::colorMode::CMYK:
            for (unsigned int i = 0; i < doc->channelCount; i++)
            {
                image->channels[i] = new Channel();
                image->channels[i]->data = (uint8_t *)malloc(doc->width * doc->height * sizeof(uint8_t));
                image->channels[i]->size = doc->width * doc->height * sizeof(uint8_t);
                if (doc->bitsPerChannel == 8)
                {
                    CopyRemainingPixels<uint8_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 16)
                {
                    CopyRemainingPixels<uint16_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 32)
                {
                    CopyRemainingPixels<float32_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }
            }
            image->width = doc->width;
            image->height = doc->height;
            image->channelCount = doc->channelCount;
            image->colorSpace = ColorSpace::CMYK;
            break;

        case psd::colorMode::MULTICHANNEL:
            for (unsigned int i = 0; i < doc->channelCount; ++i)
            {

                image->channels[i]->data = (uint8_t *)malloc(doc->width * doc->height * sizeof(uint8_t));
                image->channels[i]->size = doc->width * doc->height * sizeof(uint8_t);
                if (doc->bitsPerChannel == 8)
                {
                    CopyRemainingPixels<uint8_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 16)
                {
                    CopyRemainingPixels<uint16_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 32)
                {
                    CopyRemainingPixels<float32_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }
                uint16_t color[4];
                color[0] = resourcesSection->alphaChannels[i].color[0];
                color[1] = resourcesSection->alphaChannels[i].color[1];
                color[2] = resourcesSection->alphaChannels[i].color[2];
                color[3] = resourcesSection->alphaChannels[i].color[3];
                GetColorTable(color, image->channels[i]->color, resourcesSection->alphaChannels[i].colorSpace);
                std::cout << image->channels[i]->color[0] << " " << image->channels[i]->color[1] << " " << image->channels[i]->color[2] << " " << image->channels[i]->color[3] << std::endl;
                image->channels[i]->mode = resourcesSection->alphaChannels[i].mode;
                image->channels[i]->colorspace = resourcesSection->alphaChannels[i].colorSpace;
                image->channels[i]->opacity = resourcesSection->alphaChannels[i].opacity;
            }
            image->width = doc->width;
            image->height = doc->height;
            image->channelCount = doc->channelCount;
            image->colorSpace = ColorSpace::MULTI_CHANNEL;
            break;
        case psd::colorMode::DUOTONE:
            /* code */
            break;
        case psd::colorMode::LAB:

            for (unsigned int i = 0; i < doc->channelCount; i++)
            {
                image->channels[i] = new Channel();
                image->channels[i]->data = (uint8_t *)malloc(doc->width * doc->height * sizeof(uint8_t));
                image->channels[i]->size = doc->width * doc->height * sizeof(uint8_t);
                if (doc->bitsPerChannel == 8)
                {
                    CopyRemainingPixels<uint8_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 16)
                {
                    CopyRemainingPixels<uint16_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 32)
                {
                    CopyRemainingPixels<float32_t>(dataSection->images[i].data, image->channels[i]->data, doc->width * doc->height);
                }
            }
            image->width = doc->width;
            image->height = doc->height;
            image->channelCount = doc->channelCount;
            image->colorSpace = ColorSpace::LAB;
            break;
        default:
            result = -1;
            break;
            psd::DestroyImageDataSection(dataSection, allocator);
            psd::DestroyImageResourcesSection(resourcesSection, allocator);
        }
        return result;
    }
}