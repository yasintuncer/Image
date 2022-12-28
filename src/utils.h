#ifndef UTILS_H
#define UTILS_H
#include <string>

namespace utils
{
    // ------------------------------------------------------------
    // split extension from path
    static inline std::string getExtension(const std::string &path)
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



#endif // UTILS_H