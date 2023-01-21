// Image library includes
#include "Image.h"
#include "DecodePsd.h"
// Psd sdk includes
#include "Psd.h"
#include "PsdDocument.h"
#include "PsdStringUtil.h"
#include "PsdMallocAllocator.h"
#include "PsdNativeFile.h"
#include "PsdColorMode.h"
#include "PsdAlphaChannel.h"
#include "PsdPlanarImage.h"
#include "PsdImageResourcesSection.h"
#include "PsdImageDataSection.h"
#include "PsdParseDocument.h"
#include "PsdParseImageDataSection.h"
#include "PsdParseImageResourcesSection.h"
#include "Psdinttypes.h"

// C++ includes
#include <stdlib.h>
#include <cmath>
#include <cstring>
#include <iostream>
PSD_USING_NAMESPACE;
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
            dst[0] = 100 - remap(src[0], 65535.0, 0.0, 100.0, 0.0);
            dst[1] = 100 - remap(src[1], 65535.0, 0.0, 100.0, 0.0);
            dst[2] = 100 - remap(src[2], 65535.0, 0.0, 100.0, 0.0);
            dst[3] = 100 - remap(src[3], 65535.0, 0.0, 100.0, 0.0);
            break;

        case psd::COLOR_SPACE::LAB:
            dst[0] = remap(src[0], 65535.0, 0.0, 100.0, 0.0);
            dst[1] = remap(src[1], 65535.0, 0.0, 100.0, 0.0);
            dst[2] = remap(src[2], 65535.0, 0.0, 255.0, 0.0);
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
IMAGE_NAMESPACE_BEGIN
namespace DecodePsd
{
    // ---------------------------------------------------------------------------------------------
    // getChannel - get channel data from psd file
    int getChannel(void *data, psd::Document *doc, psd::AlphaChannel *channel_resource, Channel *channel, int channelIndex)
    {
        switch (doc->colorMode)
        {
        // unsupported color modes
        case psd::colorMode::BITMAP:
        {
            return -1;
        }
        break;
        // unsupported color modes
        case psd::colorMode::GRAYSCALE:
        {
            return -1;
        }
        break;
        // unsupported color modes
        case psd::colorMode::INDEXED:
        {
            return -1;
        }
        break;
        case psd::colorMode::RGB:
        {
            channel->size = doc->width * doc->height * sizeof(uint8_t);
            channel->colorspace = Channel::RGB;
            channel->bitsPerPixel = 8;
            channel->opacity = channel_resource->opacity;

            // channel colors
            switch (channelIndex)
            {
            case 0:
            {
                channel->color[0] = 255;
                channel->color[1] = 0;
                channel->color[2] = 0;
                channel->color[3] = 0;
            }
            break;
            case 1:
            {
                channel->color[0] = 0;
                channel->color[1] = 255;
                channel->color[2] = 0;
                channel->color[3] = 0;
            }
            break;
            case 2:
            {
                channel->color[0] = 0;
                channel->color[1] = 0;
                channel->color[2] = 255;
                channel->color[3] = 0;
            }
            break;
            case 3:
            {
                channel->color[0] = 0;
                channel->color[1] = 0;
                channel->color[2] = 0;
                channel->color[3] = 0;
            }
            break;
            default:
                break;
            }

            // Copy data to channel
            channel->data = new uint8_t[channel->size];
            switch (channel->bitsPerPixel)
            {
            case 8:
                CopyRemainingPixels<uint8_t>(data, channel->data, channel->size);
                break;

            case 16:
                CopyRemainingPixels<uint16_t>(data, channel->data, channel->size);
                break;
            case 32:
                CopyRemainingPixels<float32_t>(data, channel->data, channel->size);
                break;
            default:
                delete[] channel->data;
                channel->data = nullptr;
                return -1;
            }
        }
        break;
        case psd::colorMode::CMYK:
        {
            channel->size = doc->width * doc->height * sizeof(uint8_t);
            channel->colorspace = Channel::CMYK;
            channel->bitsPerPixel = 8;
            channel->opacity = channel_resource->opacity;
            // channel colors
            switch (channelIndex)
            {
            case 0:
            {
                channel->color[0] = 100;
                channel->color[1] = 0;
                channel->color[2] = 0;
                channel->color[3] = 0;
            }
            break;
            case 1:
            {
                channel->color[0] = 0;
                channel->color[1] = 100;
                channel->color[2] = 0;
                channel->color[3] = 0;
            }
            break;
            case 2:
            {
                channel->color[0] = 0;
                channel->color[1] = 0;
                channel->color[2] = 100;
                channel->color[3] = 0;
            }
            break;
            case 3:
            {
                channel->color[0] = 0;
                channel->color[1] = 0;
                channel->color[2] = 0;
                channel->color[3] = 100;
            }
            break;
            default:
            {
                channel->color[0] = 0;
                channel->color[1] = 0;
                channel->color[2] = 0;
                channel->color[3] = 0;
            }

            break;
            }

            // Copy data to channel
            channel->data = new uint8_t[channel->size];
            switch (channel->bitsPerPixel)
            {
            case 8:
                CopyRemainingPixels<uint8_t>(data, channel->data, channel->size);
                break;
            case 16:
                CopyRemainingPixels<uint16_t>(data, channel->data, channel->size);
                break;
            case 32:
                CopyRemainingPixels<float32_t>(data, channel->data, channel->size);
                break;
            default:
                delete[] channel->data;
                channel->data = nullptr;
                return -1;
            }
        }
        break;
        case psd::colorMode::MULTICHANNEL:
        {
            channel->size = doc->width * doc->height * sizeof(uint8_t);
            channel->mode = channel_resource->mode;
            channel->bitsPerPixel = 8;
            channel->colorspace = channel_resource->colorSpace;
            channel->opacity = channel_resource->opacity;
            uint16_t color[4];
            GetColorTable(channel_resource->color, channel->color, channel_resource->colorSpace);
            channel->data = new uint8_t[channel->size];
            switch (channel->bitsPerPixel)
            {
            case 8:
                CopyRemainingPixels<uint8_t>(data, channel->data, channel->size);
                break;
            case 16:
                CopyRemainingPixels<uint16_t>(data, channel->data, channel->size);
                break;
            case 32:
                CopyRemainingPixels<float32_t>(data, channel->data, channel->size);
                break;
            default:
                delete[] channel->data;
                channel->data = nullptr;
                return -1;
            }
        }
        break;
        case psd::colorMode::DUOTONE:
        {
            return -1;
        }
        break;
        case psd::colorMode::LAB:
        {
            channel->size = doc->width * doc->height * sizeof(uint8_t);
            channel->colorspace = channel_resource->colorSpace;
            channel->bitsPerPixel = 8;
            channel->opacity = channel_resource->opacity;
            // channel colors
            switch (channelIndex)
            {
            case 0:
            {
                channel->color[0] = 100;
                channel->color[1] = 0;
                channel->color[2] = 0;
                channel->color[3] = 0;
            }
            break;
            case 1:
            {
                channel->color[0] = 0;
                channel->color[1] = 100;
                channel->color[2] = 0;
                channel->color[3] = 0;
            }
            break;
            case 2:
            {
                channel->color[0] = 0;
                channel->color[1] = 0;
                channel->color[2] = 100;
                channel->color[3] = 0;
            }
            break;
            default:
            {
                channel->color[0] = 0;
                channel->color[1] = 0;
                channel->color[2] = 0;
                channel->color[3] = 0;
            }
            break;
            }

            // Copy data to channel
            channel->data = new uint8_t[channel->size];
            switch (channel->bitsPerPixel)
            {
            case 8:
                CopyRemainingPixels<uint8_t>(data, channel->data, channel->size);
                break;
            case 16:
                CopyRemainingPixels<uint16_t>(data, channel->data, channel->size);
                break;
            case 32:
                CopyRemainingPixels<float32_t>(data, channel->data, channel->size);
                break;
            default:
                delete[] channel->data;
                channel->data = nullptr;
                return -1;
            }
        }
        break;
        default:
            return -1;
        }
        return 0;
    }

    // ---------------------------------------------------------------------------------------------
    // DecodePSD - decode psd file
    int open(const char *filename, Image *image)
    {
        // Create allocator and file object
        psd::MallocAllocator allocator;
        psd::NativeFile file(&allocator);

        // Open file
        const wchar_t *wpath = stringUtil::ConvertString(filename);
        
        if (!file.OpenRead(wpath))
        {
            std::cout << "Error: Failed to open file " << filename << std::endl;
            return -1;
        }

        // Create document
        psd::Document *document;
        document = psd::CreateDocument(&file, &allocator);
        if (!document)
        {
            std::cout << "Error: Failed to create document" << std::endl;
            file.Close();
            return -1;
        }


        image->depth = document->bitsPerChannel;
        // indexed, duotone, bitmap color modes are not supported yet
        if (document->colorMode == psd::colorMode::INDEXED || document->colorMode == psd::colorMode::DUOTONE || document->colorMode == psd::colorMode::BITMAP)
        {
            std::cout << "Error: Unsupported color mode \n ColorMode : " << psd::colorMode::ToString(document->colorMode) << std::endl;
            psd::DestroyDocument(document, &allocator);
            file.Close();
            return -1;
        }

        // check if image data section available. the image data section stores the final, merged image data,
        //      as well as alpha channel data this only available when saving the document with "Maximize Compatibility" turned on.
        if (document->imageDataSection.length == 0)
        {
            std::cout << "Error: Image data section is empty" << std::endl;
            psd::DestroyDocument(document, &allocator);
            file.Close();
            return -1;
        }

        // Parse image resources section and image data section
        psd::ImageResourcesSection *resourcesSection = psd::ParseImageResourcesSection(document, &file, &allocator);
        psd::ImageDataSection *dataSection = psd::ParseImageDataSection(document, &file, &allocator);
        if (!resourcesSection || !dataSection)
        {
            std::cout << "Error: Failed to parse image data section" << std::endl;
            psd::DestroyDocument(document, &allocator);
            psd::DestroyImageResourcesSection(resourcesSection, &allocator);
            psd::DestroyImageDataSection(dataSection, &allocator);
            file.Close();
            return -1;
        }
        unsigned int channel_count = document->channelCount;
        image->channels = new Channel *[channel_count];
        psd::AlphaChannel *channel_resource = &resourcesSection->alphaChannels[0];

        for (unsigned int i = 0; i < channel_count; i++)
        {
            image->channels[i] = new Channel();

            int result = getChannel(dataSection->images[i].data, document, channel_resource, image->channels[i], i);
            // if failed to get channel, destroy document  resources and data section and return
            if (result < 0)
            {
                std::cout << "Error: Failed to get channel" << std::endl;
                psd::DestroyDocument(document, &allocator);
                psd::DestroyImageResourcesSection(resourcesSection, &allocator);
                psd::DestroyImageDataSection(dataSection, &allocator);
                file.Close();
                return -1;
            }
            channel_resource++;
        }
        // set image properties
        image->width = document->width;
        image->height = document->height;
        image->channelCount = document->channelCount;
        image->colorMode = document->colorMode;

        // destroy document  resources and data section
        psd::DestroyDocument(document, &allocator);
        psd::DestroyImageResourcesSection(resourcesSection, &allocator);
        psd::DestroyImageDataSection(dataSection, &allocator);
        file.Close();

        return 0;
    }

}

IMAGE_NAMESPACE_END
