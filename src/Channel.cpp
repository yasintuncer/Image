#include "Channel.h"
#include <cstring>

#if defined(__APPLE__)
#include <stdlib.h>
#include <errno.h>
#else
#include <malloc.h>
#endif
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
        free(data);
        data == nullptr;
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
