#include "../src/Image.h"
#include "../src/ImageProc.h"
#include <iostream>
#include <cstring>
#include <dirent.h>
#include <vector>


// get all files in a directory who have a specific extension psd
void getFiles(std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(dir.c_str())) == NULL)
    {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        std::string filename = dirp->d_name;
        if (filename.find(".psd") != std::string::npos)
        { // if the file has the extension psd split the string and get the filename
            std::string file = filename.substr(0, filename.find(".psd"));
            std::cout << "Found file: " << file << std::endl;
            files.push_back(file);
        }
    }
    closedir(dp);
}

int extractTopng(const char *dir)
{

    std::vector<std::string> files = std::vector<std::string>();
    getFiles(dir, files);
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        try
        {

            std::string file = *it;
            std::string path = std::string(dir) + "/" + file + ".psd";
            std::cout << "Opening file: " << file << std::endl;
            Image::Image *image = new Image::Image();
            if (image->open(path.c_str()) < 0)
            {
                std::cout << "Error: Failed to open file " << path << std::endl;
                continue;
            }
            // make directory for result
            std::string command = "mkdir -p " + std::string(dir) + "/result";
            system(command.c_str());
            // save the image
            std::string savepath = std::string(dir) + "/result/" + file + ".png";
            image->save(savepath.c_str());
            image->free();
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}



int extractResizedImageTopng(const char *dir)
{

    std::vector<std::string> files = std::vector<std::string>();
    getFiles(dir, files);
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        try
        {

            std::string file = *it;
            std::string path = std::string(dir) + "/" + file + ".psd";
            std::cout << "Opening file: " << file << std::endl;
            Image::Image *image = new Image::Image();
            if (image->open(path.c_str()) < 0)
            {
                std::cout << "Error: Failed to open file " << path << std::endl;
                continue;
            }


            // resize the image
            Image::Image *resizedImage = new Image::Image();
            Image::ImageProc::resizeImage(image, resizedImage, 512,512, true);
            image->free();

            // make directory for result
            std::string command = "mkdir -p " + std::string(dir) + "/result";
            system(command.c_str());
            // save the image
            std::string savepath = std::string(dir) + "/result/resized_" + file + ".png";
            resizedImage->save(savepath.c_str());
            resizedImage->free();
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <directory>" << std::endl;

        return -1;
    }
    extractResizedImageTopng(argv[1]);

    return 0;
}