#include "../src/DecodePsd.h"
#include "../src/Image.h"
#include <iostream>
#include <cstring>
#include <dirent.h>
#include <vector>



int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <psd file>" << std::endl;
        return 0;
    }
   
    Image::Image *image = new Image::Image();
    if (image->open(argv[1]) < 0)
    {
        std::cout << "Error: Failed to open file " << argv[1] << std::endl;
        return -1;
    }
    std::cout << "Image Name: " << argv[1] << std::endl;
    std::cout << "Image Width: " << image->width << std::endl;
    std::cout << "Image Height: " << image->height << std::endl;
    std::cout << "Image Color Mode: " << image->colorMode << std::endl;
    std::cout << "Image Channel Count: " << image->channelCount << std::endl;
    for(unsigned int i = 0; i< image->channelCount; i++){
        std::cout << "Channel Mode: " << (int)image->channels[i]->mode << std::endl;
        std::cout << "Channel bitsPerPixel: " << (int)image->channels[i]->bitsPerPixel << std::endl;
        std::cout << "Channel Opacity: " << image->channels[i]->opacity << std::endl;
        std::cout << "Channel ColorSpace: " << image->channels[i]->colorspace << std::endl;
        std::cout << "Channel Color: " << image->channels[i]->color[0] << " " << image->channels[i]->color[1] << " " << image->channels[i]->color[2] << " " << image->channels[i]->color[3] << std::endl;
    }
    image->~Image();   


 
    return 0;
}
