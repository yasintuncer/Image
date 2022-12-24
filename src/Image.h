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
    } ColorMode;
    typedef struct Channel
    {
        enum Mode
        {
            Alpha,
            Spot,
            InvertedAlpha
        };
        uint8_t *data;
        Mode mode;
        uint8_t bitsPerPixel;
        double *color;
        ColorMode colorspace;
    } Channel;
    class Image
    {

    public:
        uint32_t width;
        uint32_t height;
        Channel *channels;
        uint8_t channelCount;
        ColorMode colorMode;

    public:
        Image();
        ~Image();
        int open(const char *path);
        Image *resize(uint32_t width, uint32_t height);
        Image *resize(uint32_t size, bool keepAspectRatio);
        void save(char *path);
        uint8_t *InterleaveRGB();
        //assign operator
        Image &operator=(const Image &other);

    };
}

#endif // IMAGE_H
