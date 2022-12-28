#ifndef IMAGE_H
#define IMAGE_H
#include "Channel.h"
#include <stdint.h>

namespace Image
{
    class Channel;
    
    // ------------------------------------------------------------
    // supported file formats
    typedef enum SupportedFormats
    {
        PSD = 0

    } SupportedFormats;

    // ------------------------------------------------------------
    // color space : color space of image
    typedef enum ColorSpace
    {
        RGB = 0,
        CMYK = 1,
        HSV = 2,
        LAB = 3,
        MULTI_CHANNEL = 4
    } ColorSpace;

    // ------------------------------------------------------------
    // image class
    class Image
    {
    public:
        // ---------------
        // constructor
        Image();

        // ---------------
        // destructor
        ~Image();
        // ---------------
        // assign operator
        Image &operator=(const Image &other);

    public:
        uint32_t width;              // width of image
        uint32_t height;             // height of image
        Channel **channels;          // channels of image
        uint8_t channelCount;        // number of channels
        uint8_t colorSpace;       // color space of image
        SupportedFormats fileformat; // file format of image

    public:
        // ---------------
        // load image from file
        int open(const char *path);

        // ---------------
        // save image to file jpeg format
        int save(const char *path);

        // ---------------
        // close image
        int close();

        // ---------------
        // interleave RGB data from channels
        uint8_t *InterleaveRGB();

        // ---------------
        // resize image
        Image *resize(uint32_t width, uint32_t height);
        Image *resize(uint32_t size, bool keepAspectRatio);
    };
}

#endif // IMAGE_H
