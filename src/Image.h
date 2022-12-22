#ifndef IMAGE_H
#define IMAGE_H
#include <stdint.h>
namespace Image
{
    typedef enum ColorSpace
    {
        RGB = 0,
        CMYK = 1,
        HSV = 2,
        LAB = 3,
        MULTI_CHANNEL = 4
    }ColorSpace;
    typedef struct Channel
    {
        typedef enum Type
        {
            L,
            A,
            B,
            R,
            G,
            B,
            C,
            M,
            Y,
            K,
            H,
            S,
            V,
            Alpha,
            Spot,
        }; 
        uint8_t *data;
        Type type;
        uint8_t bitsPerPixel;
    } Channel;

    typedef struct Image
    {
        uint32_t width;
        uint32_t height;
        Channel *channels;
        uint8_t channelCount;
        ColorSpace colorSpace;
    } Image;

}

#endif
