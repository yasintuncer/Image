#ifndef CHANNEL_H
#define CHANNEL_H
#include "Image.h"
#include <stdint.h>

namespace Image
{
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
}

#endif