#include "Psd.h"
#include "PsdNativeFile.h"
#include "PsdDocument.h"
#include "PsdMallocAllocator.h"
#include "PsdStringUtil.h"
#include "PsdParseDocument.h"
#include "ColorSpace.h"
#include "Conversion.h"
#include "Image.h"
#include "DecodePsd.h"
#include <iostream>
#include <cstring>
#include "JpegExtractor.h"

//------------------------------------------ Helper functions ------------------------------------------
namespace
{
    // split extension from path
    std::string getExtension(const std::string &path)
    {
        std::string extension;
        std::string::size_type index = path.find_last_of('.');
        if (index != std::string::npos)
        {
            extension = path.substr(index + 1);
        }
        return extension;
    }
    void interLeaveRGBFromRGB(uint8_t *R, uint8_t *G, uint8_t *B, uint8_t *dest, unsigned int count)
    {
        dest = new uint8_t[count * 3];
        for (uint32_t i = 0; i < count; i++)
        {
            dest[i * 3] = R[i];
            dest[i * 3 + 1] = G[i];
            dest[i * 3 + 2] = B[i];
        }
    }
    void interLeaveRGBFromCMYK(uint8_t *C, uint8_t *M, uint8_t *Y, uint8_t *K, uint8_t *dest, unsigned int count)
    {
        dest = new uint8_t[count * 3];
        for (unsigned int i = 0; i < count; i++)
        {
            ColorSpace::Cmyk cmyk;
            cmyk.c = (1.0 - (double)C[i] / 255.0) * 100.0;
            cmyk.m = (1.0 - (double)M[i] / 255.0) * 100.0;
            cmyk.y = (1.0 - (double)Y[i] / 255.0) * 100.0;
            ColorSpace::Rgb rgb;
            cmyk.To<ColorSpace::Rgb>(&rgb);

            dest[i * 3] = (uint8_t)(rgb.r);
            dest[i * 3 + 1] = (uint8_t)(rgb.g);
            dest[i * 3 + 2] = (uint8_t)(rgb.b);
        }
    }
    void interleaveRGBFromLAB(uint8_t *L, uint8_t *A, uint8_t *B, uint8_t *dest, unsigned int count)
    {
        dest = new uint8_t[count * 3];
        for (unsigned int i = 0; i < count; i++)
        {
            ColorSpace::Lab lab;
            lab.l = (double)L[i] / 255.0 * 100.0;
            lab.a = (double)A[i] / 255.0 * 255.0 - 128.0;
            lab.b = (double)B[i] / 255.0 * 255.0 - 128.0;
            ColorSpace::Rgb rgb;
            lab.To<ColorSpace::Rgb>(&rgb);

            dest[i * 3] = (uint8_t)(rgb.r);
            dest[i * 3 + 1] = (uint8_t)(rgb.g);
            dest[i * 3 + 2] = (uint8_t)(rgb.b);
        }
    }
    void interleaveRGBFromHSV(uint8_t *H, uint8_t *S, uint8_t *V, uint8_t *dest, unsigned int count)
    {
        dest = new uint8_t[count * 3];
        for (unsigned int i = 0; i < count; i++)
        {
            ColorSpace::Hsv hsv;
            hsv.h = (double)H[i] / 255.0 * 360.0;
            hsv.s = (double)S[i] / 255.0 * 100.0;
            hsv.v = (double)V[i] / 255.0 * 100.0;
            ColorSpace::Rgb rgb;
            hsv.To<ColorSpace::Rgb>(&rgb);

            dest[i * 3] = (uint8_t)(rgb.r);
            dest[i * 3 + 1] = (uint8_t)(rgb.g);
            dest[i * 3 + 2] = (uint8_t)(rgb.b);
        }
    }
    void interleaveRGBFromMultichannel(Image::Channel *channels, uint8_t *dest, unsigned int channel_count, unsigned int count)
    {
        dest = new uint8_t[count * 3];
        memset(dest, 0, count * 3);
        for (unsigned int i = 0; i < channel_count; i++)
        {
            ColorSpace::Rgb rgb_coef;
            ColorSpace::Cmyk cmyk_coef;
            ColorSpace::Hsv hsv_coef;
            ColorSpace::Lab lab_coef;

            switch (channels[i].colorspace)
            {
            case Image::ColorMode::RGB:
                rgb_coef = ColorSpace::Rgb(channels[i].color[0], channels[i].color[1], channels[i].color[2]);
                break;
            case Image::ColorMode::CMYK:
                cmyk_coef = ColorSpace::Cmyk(100.0 - channels[i].color[0], 100.0 - channels[i].color[1], 100.0 - channels[i].color[2], 100.0 - channels[i].color[3]);
                cmyk_coef.To<ColorSpace::Rgb>(&rgb_coef);
                break;
            case Image::ColorMode::HSV:
                hsv_coef = ColorSpace::Hsv(channels[i].color[0], channels[i].color[1], channels[i].color[2]);
                hsv_coef.To<ColorSpace::Rgb>(&rgb_coef);
                break;
            case Image::ColorMode::LAB:
                lab_coef = ColorSpace::Lab(channels[i].color[0], channels[i].color[1], channels[i].color[2]);
                lab_coef.To<ColorSpace::Rgb>(&rgb_coef);
                break;
            default:
                break;
            }

            for (unsigned int j = 0; j < count; j++)
            {
                double pixel = 1.0 - (double)channels[i].data[j] / 256.0;
                dest[j * 3u + 0u] += (uint8_t)((pixel * rgb_coef.r) + (1.0 - pixel) * (double)dest[j * 3u + 0u]);
                dest[j * 3u + 1u] += (uint8_t)((pixel * rgb_coef.g) + (1.0 - pixel) * (double)dest[j * 3u + 1u]);
                dest[j * 3u + 2u] += (uint8_t)((pixel * rgb_coef.b) + (1.0 - pixel) * (double)dest[j * 3u + 2u]);
            }
        }
    }
};

//------------------------------------------ Image ------------------------------------------
namespace Image
{
    Image::Image()
    {
        width = 0;
        height = 0;
        channels = NULL;
        channelCount = 0;
        colorMode = RGB;
    }

    Image::~Image()
    {
        if (channels)
        {
            delete[] channels;
        }
    }

    int Image::open(const char *path)
    {
        std::string suffix = getExtension(path).c_str();
        if (suffix == "psd" || suffix == "PSD")
        {
            psd::MallocAllocator allocator;
            psd::NativeFile file(&allocator);
            const wchar_t *wpath = psd::stringUtil::ConvertString(path);
            if (!file.OpenRead(wpath))
            {
                std::cout << "Error: Failed to open file " << wpath << std::endl;
                return -1;
            }
            psd::Document *document;
            document = psd::CreateDocument(&file, &allocator);

            if (!document)
            {
                std::cout << "Error: Failed to create document" << std::endl;
                file.Close();
                return -1;
            }

            if (createImage(&file, &allocator, document, this) < 0)
            {
                std::cout << "Error: Failed to create image" << std::endl;
                file.Close();
                return -1;
            }
        }
        else
        {
            return -1;
        }
        return 0;
    }

    uint8_t *Image::InterleaveRGB()
    {
        uint8_t *dest = nullptr;
        switch (this->colorMode)
        {
        case ColorMode::RGB:
            interLeaveRGBFromRGB(this->channels[0].data, this->channels[1].data, this->channels[2].data, dest, this->width * this->height);
            break;
        case ColorMode::CMYK:
            interLeaveRGBFromCMYK(this->channels[0].data, this->channels[1].data, this->channels[2].data, this->channels[3].data, dest, this->width * this->height);
            break;
        case ColorMode::HSV:
            interleaveRGBFromHSV(this->channels[0].data, this->channels[1].data, this->channels[2].data, dest, this->width * this->height);
            break;
        case ColorMode::LAB:
            interleaveRGBFromHSV(this->channels[0].data, this->channels[1].data, this->channels[2].data, dest, this->width * this->height);
            break;
        case ColorMode::MULTI_CHANNEL:
            interleaveRGBFromMultichannel(this->channels, dest, this->channelCount, this->width * this->height);
            break;
        default:
            break;
        }
        return dest;
    }

    Image &Image::operator=(const Image &other)
    {
        this->width = other.width;
        this->height = other.height;
        this->channelCount = other.channelCount;
        this->colorMode = other.colorMode;
        this->channels = new Channel[this->channelCount];
        for (unsigned int i = 0; i < this->channelCount; i++)
        {
            this->channels[i] = other.channels[i];
        }
        return *this;
    }

    void Image::save(char *path)
    {
        uint8_t *dest = InterleaveRGB();
        if(dest == nullptr)
        {
            std::cout << "Error: Failed to interleave RGB" << std::endl;
            return;
        }
        write_jpeg_file(path, 100, this->width, this->height, dest);
    }
};
