#include "Image.h"
#include <string>
#include "Psd.h"
#include "PsdNativeFile.h"
#include "PsdDocument.h"
#include "PsdAllocator.h"
#include "PsdStringUtil.h"
#include 

//------------------------------------------ Helper functions ------------------------------------------
namespace
{
    // split extension from path
    std::string getExtension(const std::string &path)
    {
        std::string extension;
        std::string::size_type index = path.find_last_of('.');
        if (index != std::string::npos)
        {
            extension = path.substr(index + 1);
        }
        return extension;
    }

   
}
namespace Image
{

    void Image::open(const char *path)
    {
        std::string extension = getExtension(path);
        if (extension == "psd" || extension == "PSD")
        {
            psd::NativeFile *file;
            psd::Document *document;
            psd::Allocator *allocator;
            

        }

    } // namespace Image
