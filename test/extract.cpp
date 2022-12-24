#include "../src/Image.h"
#include <iostream>

int main(int argc, char *argv[])
{
    Image::Image *image = new Image::Image();
    image->open("/home/master/Yasin/deneme/gray-ramp.psd");

    std::cout << "Width: " << image->width << std::endl;
    std::cout << "Height: " << image->height << std::endl;
    std::cout << "Channel Count: " << (int)image->channelCount << std::endl;
    std::cout << "Color Mode: " << (int)image->colorMode << std::endl;
   
   for(int i=0;i< image->channelCount;  i++)
   {
         std::cout << "Channel " << i << " Mode: " << (int)image->channels[i].mode << std::endl;
         std::cout << "Channel " << i << " Bits Per Pixel: " << (int)image->channels[i].bitsPerPixel << std::endl;
         std::cout << "Channel " << i << " Color Space: " << (int)image->channels[i].colorspace << std::endl;
         for(int j=0;j<3;j++)
         {
             std::cout << "Channel " << i << " Color " << j << ": " << image->channels[i].color[j] << std::endl;
         }
   }

    //image->save("/home/master/Yasin/deneme/gray-ramp.png");
    return 0;
}