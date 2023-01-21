#include "parser.h"
#include <vector>
#include "../src/Image.h"
#include "../src/ImageProc.h"
#include <algorithm>
#include "logger.inl"

#define CONSOLE_OUTPUT_ENABLED 1
static int LOG_ENABLED = 0;

inline void DEBUG(std::string msg, LogLevel level, int console_output = CONSOLE_OUTPUT_ENABLED, int log_output = LOG_ENABLED)
{
    if (console_output == CONSOLE_OUTPUT_ENABLED)
        std::cout << msg << std::endl;
    if (log_output == LOG_ENABLED)
        Logger(msg, level);
}

using namespace Image;

int main(int argc, char *argv[])
{
    Parser parser;
    if (parser.parse(argc, argv) < 0)
    {
        DEBUG("Error: Failed to parse arguments", LOG_ERROR, 1, 0);
        return -1;
    }

    if (parser.isLogValid())
    {
        LOG_DIR = parser.log_dir();
        LOG_ENABLED = 1;
    }

    std::vector<std::string> input_files = parser.get_input_file_paths();
    std::vector<std::string> output_file = parser.get_output_file_paths();

    if (input_files.empty())
    {
        DEBUG("Error: No input files specified", LOG_ERROR);
        parser.print_help();
        return -1;
    }

    // start processing the images
    for (int i = 0; i < input_files.size(); i++)
    {
        DEBUG("Opening file: " + input_files[i], LOG_INFO);
        Image::Image *image = new Image::Image();
        // open the image
        if (image->open(input_files[i].c_str()) < 0)
        {
            DEBUG("file couldn't opened \t" + input_files[i], LOG_ERROR);
            continue;
        }
        else
            DEBUG("file opened successfully \t" + input_files[i], LOG_INFO);

        if (parser.isInfoValid())
        {
            std::cout << "Image Information" << std::endl;
            std::cout << "-----------------" << std::endl;
            std::cout << "Width: " << image->width << std::endl;
            std::cout << "Height: " << image->height << std::endl;
            std::cout << "Channels: " << image->channelCount << std::endl;
            std::cout << "Color Mode: " << image->colorMode << std::endl;
            std::cout << "Depth: " << image->depth << std::endl;
        }
        // if resize is valid then resize the image
        if (parser.isResizeValid())
        {
            // if both width and height are specified
            if (parser.width() != 0 && parser.height() != 0)
            {
                // if image is smaller than the desired output size
                if (image->width < parser.width() || image->height < parser.height())
                {
                    DEBUG("Image is smaller than the desired output size", LOG_ERROR);
                    DEBUG("Closing image " + input_files[i], LOG_INFO);
                    image->Free();
                    continue;
                }

                // create a new image for resized image
                Image::Image *resizedImage = new Image::Image();
                if (Image::ImageProc::resizeImage(image, resizedImage, parser.width(), parser.height(), true) < 0)
                {
                    DEBUG("Error: Couldn't resize the image", LOG_ERROR);
                    DEBUG("Closing image " + input_files[i], LOG_INFO);
                    image->Free();
                    resizedImage->Free();
                    continue;
                }
                else
                    DEBUG("Image resized ", LOG_INFO);

                // save the resized image
                if (resizedImage->save(output_file[i].c_str(), parser.export_type().c_str()) < 0)
                {
                    DEBUG("Error: Couldn't save the file" + output_file[i], LOG_ERROR);
                    DEBUG("Closing image" + input_files[i], LOG_INFO);
                    image->Free();
                    resizedImage->Free();
                }
                else
                {
                    DEBUG("File saved successfully" + output_file[i], LOG_INFO);
                    DEBUG("Closing image" + input_files[i], LOG_INFO);
                    image->Free();
                    resizedImage->Free();
                }
            }
            // if only given edge size
            else if (parser.edge_size() != 0)
            {
                // if image is smaller than the desired output size
                if (image->width < parser.edge_size() && image->height < parser.edge_size())
                {
                    DEBUG("Image is smaller than the desired output size", LOG_ERROR);
                    DEBUG("Image Closed", LOG_INFO);
                    image->Free();
                    continue;
                }
                // set the resize width and height
                unsigned int resize_width, resize_height;
                if (image->width > image->height)
                {
                    resize_width = parser.edge_size();
                    resize_height = (image->height * parser.edge_size()) / image->width;
                }
                else
                {
                    resize_height = parser.edge_size();
                    resize_width = (image->width * parser.edge_size()) / image->height;
                }
                // create a new image for resized image
                Image::Image *resizedImage = new Image::Image();
                if (Image::ImageProc::resizeImage(image, resizedImage, resize_width, resize_height, true) < 0)
                {
                    DEBUG("Error: Couldn't resize the image", LOG_ERROR);
                    DEBUG("Closing image" + input_files[i], LOG_INFO);
                    image->Free();
                    resizedImage->Free();
                    continue;
                }
                else
                    DEBUG("Image resized ", LOG_INFO);

                // save the resized image
                if (resizedImage->save(output_file[i].c_str(), parser.export_type().c_str()) < 0)
                {
                    DEBUG("Error: Couldn't save the file" + output_file[i], LOG_ERROR);
                    DEBUG("Closing image" + input_files[i], LOG_INFO);
                    image->Free();
                    resizedImage->Free();
                }
                else
                {
                    DEBUG("File saved successfully" + output_file[i], LOG_INFO);
                    DEBUG("Closing image" + input_files[i], LOG_INFO);
                    image->Free();
                    resizedImage->Free();
                }
            }
        }
        else
        {
            // save the image
            if (image->save(output_file[i].c_str(), parser.export_type().c_str()) < 0)
            {
                DEBUG("Error: Couldn't save the file" + output_file[i], LOG_ERROR);
                DEBUG("Closing image" + input_files[i], LOG_INFO);
                image->Free();
            }
            else
            {
                DEBUG("File saved successfully" + output_file[i], LOG_INFO);
                DEBUG("Closing image" + input_files[i], LOG_INFO);
                image->Free();
            }
        }
    }
    return 0;
}
