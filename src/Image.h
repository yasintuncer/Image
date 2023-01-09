#pragma once
#include "NameSpace.h"
#include "Channel.h"
#include <stdint.h>

IMAGE_NAMESPACE_BEGIN
class Channel;

// ------------------------------------------------------------
// supported file formats
typedef enum SupportedFormats
{
    UNKNOWN = -1,
    PSD = 0

} SupportedFormats;

// ------------------------------------------------------------
// supported file formats to string
const char *SupportedFormatsToString(unsigned int value);

// ------------------------------------------------------------
// string to supported file formats
SupportedFormats StringToSupportedFormats(const char *value);

// ------------------------------------------------------------
// image class
class Image
{
public:
    enum Enum
    {
        BITMAP = 0,
        GRAYSCALE = 1,
        INDEXED = 2,
        RGB = 3,
        CMYK = 4,
        MULTICHANNEL = 7,
        DUOTONE = 8,
        LAB = 9
    };
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
    uint8_t colorMode;           // color space of image
    SupportedFormats fileformat; // file format of image
    uint8_t *interleavedRgb;     // interleaved RGB data

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

    int free();

    // ---------------
    // interleave RGB data from channels
    uint8_t *InterleaveRGB();
};
IMAGE_NAMESPACE_END


