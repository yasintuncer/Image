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
#include "Channel.h"
#include <iostream>
#include <cstring>
#include "JpegExtractor.h"
#include "utils.h"

//------------------------------------------ Helper functions ------------------------------------------
namespace
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
            ColorSpace::Cmyk cmyk;
            cmyk.c = (1.0 - (double)C[i] / 255.0);
            cmyk.m = (1.0 - (double)M[i] / 255.0);
            cmyk.y = (1.0 - (double)Y[i] / 255.0);
            cmyk.k = (1.0 - (double)K[i] / 255.0);
            ColorSpace::Rgb rgb;
            cmyk.To<ColorSpace::Rgb>(&rgb);

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

    //--------------------------------------------------------------------------------------------------
    // Interleave RGB data from H,S,V channels
    void interleaveRGBFromHSV(uint8_t *H, uint8_t *S, uint8_t *V, uint8_t *dest, unsigned int count)
    {
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

    //--------------------------------------------------------------------------------------------------
    // Interleave RGB data from multichannel
    void interleaveRGBFromMultichannel(Image::Channel **channels, uint8_t *dest, unsigned int channel_count, unsigned int count)
    {
        for (unsigned int i = 0; i < channel_count; i++)
        {
            ColorSpace::Rgb rgb_coef;

            if (channels[i]->colorspace == Image::ColorSpace::RGB)
            {
                 ColorSpace::Rgb rgb_coef(channels[i]->color[0] , channels[i]->color[1], channels[i]->color[2]);
                 std::cout << "rgb: " << rgb_coef.r << " " << rgb_coef.g << " " << rgb_coef.b << std::endl;
            }

            else if (channels[i]->colorspace == Image::ColorSpace::CMYK)
            {
                 ColorSpace::Cmyk cmyk(channels[i]->color[0]/ 100.0, channels[i]->color[1], channels[i]->color[2]/ 100.0, channels[i]->color[3]/ 100.0);
                 std::cout << "cmyk: " << cmyk.c << " " << cmyk.m << " " << cmyk.y << " " << cmyk.k << std::endl;
                 cmyk.ToRgb(&rgb_coef);
            }

            else if(channels[i]->colorspace == Image::ColorSpace::HSV)
            {
                 ColorSpace::Hsv hsv(channels[i]->color[0], channels[i]->color[1] / 100.0, channels[i]->color[2] / 100.0);
                 std::cout << "hsv: " << hsv.h << " " << hsv.s << " " << hsv.v << std::endl;
                 hsv.ToRgb(&rgb_coef);
            }
            else if(channels[i]->colorspace == Image::ColorSpace::LAB)
            {
                 ColorSpace::Lab lab(channels[i]->color[0], channels[i]->color[1], channels[i]->color[2]);
                 lab.ToRgb(&rgb_coef);
            }
            std::cout << "rgb_coef: " << rgb_coef.r << " " << rgb_coef.g << " " << rgb_coef.b << std::endl;
            double opacity_coef = channels[i]->opacity / 100.0;
            // if (channels[i]->mode == Image::Mode::Spot)
            //{
            for (unsigned int j = 0; j < count; j++)
            {
                 double pixel = 1.0 - (double)channels[i]->data[j] / 256.0;
                 dest[j * 3u + 0u] = (uint8_t)((pixel * rgb_coef.r) + (1.0 - pixel) * (double)dest[j * 3u + 0u]);
                 dest[j * 3u + 1u] = (uint8_t)((pixel * rgb_coef.g) + (1.0 - pixel) * (double)dest[j * 3u + 1u]);
                 dest[j * 3u + 2u] = (uint8_t)((pixel * rgb_coef.b) + (1.0 - pixel) * (double)dest[j * 3u + 2u]);
            }
            /*
            else if (channels[i]->mode == Image::Mode::Masked)
            {
                for (unsigned int j = 0; j < count; j++)
                {
                    double pixel =(double)channels[i]->data[j] / 256.0;
                    if (pixel > 0.0)
                    {
                        dest[j * 3u + 0u] = (uint8_t)((pixel * rgb_coef.r) * (opacity_coef) + (1.0 - opacity_coef) * (double)dest[j * 3u + 0u]);
                        dest[j * 3u + 1u] = (uint8_t)((pixel * rgb_coef.g) * (opacity_coef) + (1.0 - opacity_coef) * (double)dest[j * 3u + 1u]);
                        dest[j * 3u + 2u] = (uint8_t)((pixel * rgb_coef.b) * (opacity_coef) + (1.0 - opacity_coef) * (double)dest[j * 3u + 2u]);
                    }
                }
            }
            else
            {
                for (unsigned int j = 0; j < count; j++)
                {
                    double pixel = 1.0 -(double)channels[i]->data[j] / 256.0;
                    if (pixel > 0.0)
                    {
                        dest[j * 3u + 0u] = (uint8_t)((pixel * rgb_coef.r) * (opacity_coef) + (1.0 - opacity_coef) * (double)dest[j * 3u + 0u]);
                        dest[j * 3u + 1u] = (uint8_t)((pixel * rgb_coef.g) * (opacity_coef) + (1.0 - opacity_coef) * (double)dest[j * 3u + 1u]);
                        dest[j * 3u + 2u] = (uint8_t)((pixel * rgb_coef.b) * (opacity_coef) + (1.0 - opacity_coef) * (double)dest[j * 3u + 2u]);
                    }
                }
            }
            */
        }
    }
};

//------------------------------------------ Image ------------------------------------------
namespace Image
{
    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------
    Image::Image()
    {
        width = 0;
        height = 0;
        channels = NULL;
        channelCount = 0;
        colorSpace = RGB;
    }

    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------
    Image::~Image()
    {
        if (channels)
        {
            delete[] channels;
        }
    }

    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------
    int Image::open(const char *path)
    {
        std::string suffix = utils::getExtension(path).c_str();
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
            unsigned int channel_count = document->channelCount;
            channels = new Channel *[channel_count];
            for (unsigned int i = 0; i < channel_count; i++)
            {
                 channels[i] = new Channel();
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

    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------
    uint8_t *Image::InterleaveRGB()
    {
        uint8_t *dest;
        dest = new uint8_t[this->width * this->height * 3];
        memset(dest, 0, this->width * this->height * 3);
        switch (this->colorSpace)
        {
        case ColorSpace::RGB:
            interLeaveRGBFromRGB(channels[0]->data, channels[1]->data, channels[2]->data, dest, this->width * this->height);
            break;
        case ColorSpace::CMYK:
            interLeaveRGBFromCMYK(channels[0]->data, channels[1]->data, channels[2]->data, channels[3]->data, dest, this->width * this->height);
            break;
        case ColorSpace::HSV:
            interleaveRGBFromHSV(channels[0]->data, channels[1]->data, channels[2]->data, dest, this->width * this->height);
            break;
        case ColorSpace::LAB:
            interleaveRGBFromHSV(channels[0]->data, channels[1]->data, channels[2]->data, dest, this->width * this->height);
            break;
        case ColorSpace::MULTI_CHANNEL:
            interleaveRGBFromMultichannel(this->channels, dest, channelCount, this->width * this->height);
            break;
        default:
            break;
        }
        return dest;
    }

    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------
    Image &Image::operator=(const Image &other)
    {
        this->width = other.width;
        this->height = other.height;
        this->channelCount = other.channelCount;
        this->colorSpace = other.colorSpace;
        this->channels = new Channel *[this->channelCount];
        for (unsigned int i = 0; i < this->channelCount; i++)
        {
            this->channels[i] = other.channels[i];
        }
        return *this;
    }

    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------
    int Image::save(const char *path)
    {
        uint8_t *dest = InterleaveRGB();
        if (dest == nullptr)
        {
            std::cout << "Error: Failed to interleave RGB" << std::endl;
            return -1;
        }
        int result = write_jpeg_file(path, 100, this->width, this->height, dest);
        delete[] dest;
        return result;
    }
};
