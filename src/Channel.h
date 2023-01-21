#pragma once

#include "Image.h"
#include <stdint.h>



IMAGE_NAMESPACE_BEGIN
// ------------------------------------------------------------
// mode : mode of channel
typedef enum Mode
{
    Selected = 0,
    Masked = 1,
    Spot = 2,
} Mode;

// ------------------------------------------------------------
// channel class
class Channel
{
public:
    enum Enum
    {
        DUMMY = -1,
        RGB = 0,
        HSB = 1,
        CMYK = 2,
        PANTONE = 3,
        FOCOLTONE = 4,
        TRUMATCH = 5,
        TOYO = 6,
        LAB = 7,
        GRAY = 8,
        WIDE_CMYK = 9,
        HKS = 10,
        DIC = 11,
        TOTALINK = 12,
        MNITOR_RGB = 13,
        DUOTONE = 14,
        OPACITY = 15,
        WEB = 16,
        GRAY_FLOAT = 17,
        RGB_FLOAT = 18,
        OPACITY_FLOAT = 19
    };

    // ---------------
    // constructor
    Channel();

    // ---------------
    // destructor
    ~Channel();

    // ---------------
    // assign operator
    Channel &operator=(const Channel &other);

public:
    uint8_t *data;        // data of channel
    unsigned int size;    // size of channel
    uint8_t mode;         // mode of channel
    uint8_t bitsPerPixel; // bits per pixel of channel
    double color[4];      // color table of channel
    int8_t colorspace;    // color space of channel
    unsigned int opacity; // opacity of channel
};

IMAGE_NAMESPACE_END