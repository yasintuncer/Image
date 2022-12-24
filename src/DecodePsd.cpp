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
//-------------------- Helper functions --------------------
namespace
{

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
    template <typename T>
    double remap(T src, double src_max, double src_min, double dst_max, double dst_min)
    {
        return (src - src_min) / (src_max - src_min) * (dst_max - dst_min) + dst_min;
    }

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
    void GetColorTable(uint16_t *src, unsigned int channel_count, double *dst, int colorspace)
    {
        for (unsigned int i = 0; i < channel_count; ++i)
        {
            switch (colorspace)
            {
            case psd::COLOR_SPACE::RGB:
                dst = new double[4];
                dst[0] = remap(src[0], 65535.0, 0.0, 255.0, 0.0);
                dst[1] = remap(src[1], 65535.0, 0.0, 255.0, 0.0);
                dst[2] = remap(src[2], 65535.0, 0.0, 255.0, 0.0);
                dst[3] = remap(src[3], 65535.0, 0.0, 255.0, 0.0);
                break;
            case psd::COLOR_SPACE::CMYK:
                dst = new double[4];
                dst[0] = remap(src[0], 65535.0, 0.0, 100.0, 0.0);
                dst[1] = remap(src[1], 65535.0, 0.0, 100.0, 0.0);
                dst[2] = remap(src[2], 65535.0, 0.0, 100.0, 0.0);
                dst[3] = remap(src[3], 65535.0, 0.0, 100.0, 0.0);
                break;

            case psd::COLOR_SPACE::LAB:
                dst = new double[4];
                dst[0] = remap(src[0], 65535.0, 0.0, 100.0, 0.0);
                dst[1] = remap(src[1], 65535.0, 0.0, 255.0, -128.0);
                dst[2] = remap(src[2], 65535.0, 0.0, 255.0, -128.0);
                dst[3] = remap(src[3], 65535.0, 0.0, 255.0, 0.0);
                break;

            case psd::COLOR_SPACE::HSB:
                dst = new double[4];
                dst[0] = remap(src[0], 65535.0, 0.0, 360.0, 0.0);
                dst[1] = remap(src[1], 65535.0, 0.0, 100.0, 0.0);
                dst[2] = remap(src[2], 65535.0, 0.0, 100.0, 0.0);
                dst[3] = remap(src[3], 65535.0, 0.0, 255.0, 0.0);
                break;
            }
        }
    }
}
namespace Image
{
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
            result = 1;
            break;
        case psd::colorMode::GRAYSCALE:
            /* code */
            result = 1;
            break;
        case psd::colorMode::INDEXED:
            /* code */
            result = 1;
            break;
        case psd::colorMode::RGB:

            image->channels = new Channel[doc->channelCount];
            for (unsigned int i = 0; i < doc->channelCount; ++i)
            {

                if (doc->bitsPerChannel == 8)
                {
                    CopyRemainingPixels<uint8_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 16)
                {
                    CopyRemainingPixels<uint16_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 32)
                {
                    CopyRemainingPixels<float32_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }

                GetColorTable(resourcesSection->alphaChannels[i].color, doc->channelCount, image->channels[i].color, resourcesSection->alphaChannels[i].colorSpace);
            }
            image->width = doc->width;
            image->height = doc->height;
            image->channelCount = doc->channelCount;
            image->colorMode = ColorMode::RGB;
            break;

        case psd::colorMode::CMYK:
            image->channels = new Channel[doc->channelCount];
            for (unsigned int i = 0; i < doc->channelCount; i++)
            {
                if (doc->bitsPerChannel == 8)
                {
                    CopyRemainingPixels<uint8_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 16)
                {
                    CopyRemainingPixels<uint16_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 32)
                {
                    CopyRemainingPixels<float32_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }
            }
            image->width = doc->width;
            image->height = doc->height;
            image->channelCount = doc->channelCount;
            image->colorMode = ColorMode::CMYK;
            break;

        case psd::colorMode::MULTICHANNEL:
            image->channels = new Channel[doc->channelCount];
            for (unsigned int i = 0; i < doc->channelCount; ++i)
            {
                image->channels[i].data = new uint8_t[doc->width * doc->height];
                if (doc->bitsPerChannel == 8)
                {
                    CopyRemainingPixels<uint8_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 16)
                {
                    CopyRemainingPixels<uint16_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 32)
                {
                    CopyRemainingPixels<float32_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }

                GetColorTable(resourcesSection->alphaChannels[i].color, doc->channelCount, image->channels[i].color, resourcesSection->alphaChannels[i].colorSpace);
                if (resourcesSection->alphaChannels[i].colorSpace == psd::AlphaChannel::Mode::SPOT)
                    image->channels[i].mode = Channel::Spot;
                else if (resourcesSection->alphaChannels[i].colorSpace == psd::AlphaChannel::Mode::ALPHA)
                    image->channels[i].mode = Channel::Alpha;
                else if (resourcesSection->alphaChannels[i].colorSpace == psd::AlphaChannel::Mode::INVERTED_ALPHA)
                    image->channels[i].mode = Channel::InvertedAlpha;
                if (resourcesSection->alphaChannels[i].colorSpace == psd::COLOR_SPACE::RGB)
                    image->channels[i].colorspace = ColorMode::RGB;
                else if (resourcesSection->alphaChannels[i].colorSpace == psd::COLOR_SPACE::CMYK)
                    image->channels[i].colorspace = ColorMode::CMYK;
                else if (resourcesSection->alphaChannels[i].colorSpace == psd::COLOR_SPACE::LAB)
                    image->channels[i].colorspace = ColorMode::LAB;
                else if (resourcesSection->alphaChannels[i].colorSpace == psd::COLOR_SPACE::HSB)
                    image->channels[i].colorspace = ColorMode::HSV;
            }
            image->width = doc->width;
            image->height = doc->height;
            image->channelCount = doc->channelCount;
            image->colorMode = ColorMode::MULTI_CHANNEL;
            break;
        case psd::colorMode::DUOTONE:
            /* code */
            break;
        case psd::colorMode::LAB:

            image->channels = new Channel[doc->channelCount];
            for (unsigned int i = 0; i < doc->channelCount; i++)
            {
                if (doc->bitsPerChannel == 8)
                {
                    CopyRemainingPixels<uint8_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 16)
                {
                    CopyRemainingPixels<uint16_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }
                else if (doc->bitsPerChannel == 32)
                {
                    CopyRemainingPixels<float32_t>(dataSection->images[i].data, image->channels[i].data, doc->width * doc->height);
                }
            }
            image->width = doc->width;
            image->height = doc->height;
            image->channelCount = doc->channelCount;
            image->colorMode = ColorMode::LAB;
            break;
        default:
            result = -1;
            break;
        }
        return result;
    }
}