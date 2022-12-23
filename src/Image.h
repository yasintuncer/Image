#ifndef IMAGE_H
#define IMAGE_H
#include <stdint.h>
namespace Image
{
    enum SupportedFormats
    {
        PSD = 0
    };
    typedef enum ColorMode
    {
        RGB = 0,
        CMYK = 1,
        HSV = 2,
        LAB = 3,
        MULTI_CHANNEL = 4
    }ColorMode;
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
            InvertedAlpha,
        }; 
        uint8_t *data;
        Type type;
        uint8_t bitsPerPixel;
        double *color;
    } Channel;
    typedef struct Image
    {
        uint32_t width;
        uint32_t height;
        Channel *channels;
        uint8_t channelCount;
        ColorMode colorMode;
        void open(const char *path);
        uint8_t* InterleaveRGB();
        Image* resize(uint32_t width, uint32_t height);      
        void save(const char *path);
    } Image;
}

#endif
