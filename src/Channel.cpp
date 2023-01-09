#include "Channel.h"
#include <cstring>

IMAGE_NAMESPACE_BEGIN


// ------------------------------------------------------------
// ------------------------------------------------------------
Channel::Channel()
{

    data = NULL;
    colorspace = RGB;
    opacity = 100;
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    color[3] = 0;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
Channel::~Channel()
{
    if (data)
    {
        delete[] data;
    }
}

// ------------------------------------------------------------
// ------------------------------------------------------------
Channel &Channel::operator=(const Channel &other)
{
    if (this != &other)
    {
        if (data)
        {
            delete[] data;
        }
        data = new uint8_t[other.size];
        memcpy(data, other.data, other.size);
        colorspace = other.colorspace;
        opacity = other.opacity;
        color[0] = other.color[0];
        color[1] = other.color[1];
        color[2] = other.color[2];
        color[3] = other.color[3];
    }
    return *this;
}

IMAGE_NAMESPACE_END
