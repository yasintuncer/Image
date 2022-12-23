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

//-------------------- Helper functions --------------------
namespace
{

    template <typename T>
    uint8_t *remapBuffer(T buffer, unsigned int count)
    {
        double quantization_coef = 255.0 / (double)std::numeric_limits<T>::max();
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
    void CopyRemainingPixels(T *src, uint8_t *dst, unsigned int count)
    {
        uint8_t *buffer = nullptr;
        if (sizeof(T) > sizeof(uint8_t))
        {
            buffer = remapBuffer(src, count);
        }
        else
        {
            buffer = src;
        }
        for (unsigned int i = 0; i < count; ++i)
        {
            dst[i] = src[i];
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
    Image *createImage(psd::NativeFile *file, psd::Allocator *allocator, psd::Document *doc)
    {
        Image *image = nullptr;

        switch (doc->colorMode)
        {
        case psd::colorMode::BITMAP:
            /* code */
            break;
        case psd::colorMode::GRAYSCALE:
            /* code */
            break;
        case psd::colorMode::INDEXED:
            /* code */
            break;
        case psd::colorMode::RGB:

            psd::ImageDataSection *dataSection = psd::ParseImageDataSection(doc, file, allocator);
            psd::ImageResourcesSection *resourcesSection = psd::ParseImageResourcesSection(doc, file, allocator);
            if (dataSection)
            {
                image = new Image();
                image->channels = new Channel[doc->channelCount];
                image->channels[0].type = Channel::R;
                image->channels[1].type = Channel::G;
                image->channels[2].type = Channel::B;
                image->channels[3].type = Channel::A;

                for (unsigned int i = 0; i < doc->channelCount; ++i)
                {

                    if (doc->bitsPerChannel == 8)
                    {
                        uint8_t *buffer = static_cast<uint8_t *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }
                    else if (doc->bitsPerChannel == 16)
                    {
                        uint16_t *buffer = static_cast<uint16_t *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }
                    else if (doc->bitsPerChannel == 32)
                    {
                        float32_t *buffer = static_cast<float *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }

                    GetColorTable(resourcesSection->alphaChannels[i].color, doc->channelCount, image->channels[i].color, resourcesSection->alphaChannels[i].colorSpace);
                }
                image->width = doc->width;
                image->height = doc->height;
                image->channelCount = doc->channelCount;
                image->colorMode = ColorMode::RGB;
                psd::DestroyImageDataSection(dataSection, allocator);
            }

            break;

        case psd::colorMode::CMYK:
            psd::ImageDataSection *dataSection = psd::ParseImageDataSection(doc, file, allocator);
            if (dataSection)
            {
                image = new Image();
                image->channels = new Channel[doc->channelCount];
                image->channels[0].type = Channel::C;
                image->channels[1].type = Channel::M;
                image->channels[2].type = Channel::Y;
                image->channels[3].type = Channel::K;
                for (unsigned int i = 0; i < doc->channelCount; i++)
                {
                    if (doc->bitsPerChannel == 8)
                    {
                        uint8_t *buffer = static_cast<uint8_t *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }
                    else if (doc->bitsPerChannel == 16)
                    {
                        uint16_t *buffer = static_cast<uint16_t *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }
                    else if (doc->bitsPerChannel == 32)
                    {
                        float32_t *buffer = static_cast<float *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }
                }
                image->width = doc->width;
                image->height = doc->height;
                image->channelCount = doc->channelCount;
                image->colorMode = ColorMode::CMYK;
                psd::DestroyImageDataSection(dataSection, allocator);
            }
            break;
        case psd::colorMode::MULTICHANNEL:
            psd::ImageDataSection *dataSection = psd::ParseImageDataSection(doc, file, allocator);
            psd::ImageResourcesSection *resourcesSection = psd::ParseImageResourcesSection(doc, file, allocator);
            if (dataSection)
            {
                image = new Image();
                image->channels = new Channel[doc->channelCount];

                for (unsigned int i = 0; i < doc->channelCount; ++i)
                {

                    if (doc->bitsPerChannel == 8)
                    {
                        uint8_t *buffer = static_cast<uint8_t *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }
                    else if (doc->bitsPerChannel == 16)
                    {
                        uint16_t *buffer = static_cast<uint16_t *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }
                    else if (doc->bitsPerChannel == 32)
                    {
                        float32_t *buffer = static_cast<float *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }

                    GetColorTable(resourcesSection->alphaChannels[i].color, doc->channelCount, image->channels[i].color, resourcesSection->alphaChannels[i].colorSpace);
                    if (resourcesSection->alphaChannels[i].colorSpace == psd::AlphaChannel::Mode::SPOT)
                        image->channels[i].type = Channel::Spot;
                    else if (resourcesSection->alphaChannels[i].colorSpace == psd::AlphaChannel::Mode::ALPHA)
                        image->channels[i].type = Channel::Alpha;
                    else if (resourcesSection->alphaChannels[i].colorSpace == psd::AlphaChannel::Mode::INVERTED_ALPHA)
                        image->channels[i].type = Channel::InvertedAlpha;
                }
                image->width = doc->width;
                image->height = doc->height;
                image->channelCount = doc->channelCount;
                image->colorMode = ColorMode::RGB;

                psd::DestroyImageResourcesSection(resourcesSection, allocator);
                psd::DestroyImageDataSection(dataSection, allocator);
            }
            break;

            break;
        case psd::colorMode::DUOTONE:
            /* code */
            break;
        case psd::colorMode::LAB:
            psd::ImageDataSection *dataSection = psd::ParseImageDataSection(doc, file, allocator);
            if (dataSection)
            {
                image = new Image();
                image->channels = new Channel[doc->channelCount];
                image->channels[0].type = Channel::L;
                image->channels[1].type = Channel::A;
                image->channels[2].type = Channel::B;
                image->channels[3].type = Channel::A;
                for (unsigned int i = 0; i < doc->channelCount; i++)
                {
                    if (doc->bitsPerChannel == 8)
                    {
                        uint8_t *buffer = static_cast<uint8_t *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }
                    else if (doc->bitsPerChannel == 16)
                    {
                        uint16_t *buffer = static_cast<uint16_t *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }
                    else if (doc->bitsPerChannel == 32)
                    {
                        float32_t *buffer = static_cast<float *>(dataSection->images[i].data);
                        CopyRemainingPixels(buffer, image->channels[i].data, doc->width * doc->height);
                    }
                }
                image->width = doc->width;
                image->height = doc->height;
                image->channelCount = doc->channelCount;
                image->colorMode = ColorMode::LAB;

                psd::DestroyImageDataSection(dataSection, allocator);
            }

            break;
        default:
            break;
        }
        return image;
    }
}