#pragma once 

#include "Psd.h"
#include "PsdDocument.h"
#include "PsdNativeFile.h"
#include "PsdAllocator.h"
#include "Image.h"

IMAGE_NAMESPACE_BEGIN
namespace DecodePsd
{
    int getChannel(void *data, psd::Document *doc, Channel *channel, int channelIndex);

    int open(const char *filename, Image *image);
    // int createImage(psd::NativeFile *file, psd::Allocator *allocator, psd::Document *doc, Image *image);
}

IMAGE_NAMESPACE_END