#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cstddef>
#include <cstddef>

extern "C"
{
#include <jpeglib.h>
}
#include "jerror.h"
#include "setjmp.h"

#include "Image.h"

IMAGE_NAMESPACE_BEGIN

namespace Export
{
    typedef enum ExportAs
    {
        PNG = 0,
        JPEG = 1
    } ExportAs;

    int export_image(const char *filename, Image *image, ExportAs format, int quality);
}
IMAGE_NAMESPACE_END