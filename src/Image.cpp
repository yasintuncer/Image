#include "Image.h"
#include "DecodePsd.h"
#include "ImageProc.h"
#include "Channel.h"
#include "Export.h"
#include "utils.h"

#include <cmath>
#include <iostream>
#include <cstring>

//------------------------------------------ Image ------------------------------------------
IMAGE_NAMESPACE_BEGIN

const char *SupportedFormatsToString(unsigned int value)
{
#define IMPLEMENT_CASE(value)     \
    case SupportedFormats::value: \
        return #value;
    switch (value)
    {
        IMPLEMENT_CASE(PSD);
    default:
        return "Unknown";
    }
}

SupportedFormats StringToSupportedFormats(const char *value)
{
    if (strcmp(value, "psd") == 0 || strcmp(value, "PSD"))
    {
        return SupportedFormats::PSD;
    }
    else
    {
        return SupportedFormats::UNKNOWN;
    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Image::Image()
{
    width = 0;
    height = 0;
    channels = NULL;
    channelCount = 0;
    colorMode = RGB;
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
    const char *suffix = utils::getExtension(path).c_str();
    int isOpen = 0;
    SupportedFormats fileformat = StringToSupportedFormats(suffix);
    switch (fileformat)
    {

    case SupportedFormats::PSD:
        isOpen = DecodePsd::open(path, this);
        break;
    case SupportedFormats::UNKNOWN:
        std::cout << "Error: Unknown file format" << std::endl;
        return -1;
    }
    return isOpen;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
uint8_t *Image::InterleaveRGB()
{

    this->interleavedRgb = new uint8_t[this->width * this->height * 3];
    memset(this->interleavedRgb, 255, this->width * this->height * 3);
    switch (this->colorMode)
    {

    case Image::RGB:
        ImageProc::interLeaveRGBFromRGB(channels[0]->data, channels[1]->data, channels[2]->data, this->interleavedRgb, this->width * this->height);
        break;
    case Image::CMYK:
        ImageProc::interLeaveRGBFromCMYK(channels[0]->data, channels[1]->data, channels[2]->data, channels[3]->data, this->interleavedRgb, this->width * this->height);
        break;
    case Image::LAB:
        ImageProc::interleaveRGBFromHSV(channels[0]->data, channels[1]->data, channels[2]->data, this->interleavedRgb, this->width * this->height);
        break;
    case Image::MULTICHANNEL:
        ImageProc::interleaveRGBFromMultichannel(this->channels, this->interleavedRgb, channelCount, this->width * this->height);
        break;
    default:
        std::cout << "Error: Unsupported color mode" << std::endl;
        delete[] this->interleavedRgb;
        this->interleavedRgb = nullptr;
        break;
    }
    return this->interleavedRgb;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Image &Image::operator=(const Image &other)
{
    this->width = other.width;
    this->height = other.height;
    this->channelCount = other.channelCount;
    this->colorMode = other.colorMode;
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
    int result = Export::export_image(path, this, Export::PNG, 100);
    delete[] dest;
    return result;
}
int Image::free()
{
    if (channels)
    {
        delete[] channels;
    }
    return 0;
}
IMAGE_NAMESPACE_END
