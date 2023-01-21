#include "parser.h"
#include <dirent.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string.h>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <sstream>

// char pointer split with char delimiter
std::string fix_blankspace(const char *input)
{
    std::stringstream input_str;
    input_str << input;

    return input_str.str();
}
std::string encode_turkish_character(const char *input)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::wstring wide = converter.from_bytes(input);
    std::string encoded = converter.to_bytes(wide);
    return encoded;
}
std::string encode(const char *input)
{

    std::string fixed_blanskspace = fix_blankspace(input);
    std::string encoded_turkish_character = encode_turkish_character(fixed_blanskspace.c_str());
    return encoded_turkish_character;
}
std::vector<std::string> split(const char *str, char c)
{
    std::vector<std::string> result;

    do
    {
        const char *begin = str;

        while (*str != c && *str)
            str++;

        result.push_back(std::string(begin, str));
    } while (0 != *str++);

    return result;
}
std::string getfilename(std::string filepath)
{
    std::vector<std::string> split_path = split(filepath.c_str(), '/');

    std::string file = split_path[split_path.size() - 1];
    std::vector<std::string> split_file = split(file.c_str(), '.');
    std::string filename;
    for (int i = 0; i < split_file.size() - 1; i++)
    {
        filename += split_file[i];
    }
    return filename;
}
std::string getfileDir(std::string filepath)
{
    std::vector<std::string> split_path = split(filepath.c_str(), '/');
    std::string dir;
    for (int i = 0; i < split_path.size() - 1; i++)
    {
        dir += split_path[i] + "/";
    }
    return dir;
}
std::string getfileExtension(std::string filepath)
{
    std::vector<std::string> split_path = split(filepath.c_str(), '/');
    std::string file = split_path[split_path.size() - 1];
    std::vector<std::string> split_file = split(file.c_str(), '.');
    return split_file[split_file.size() - 1];
}

std::vector<std::string> get_files(const char *path, bool isRecursive)
{
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_type == DT_REG)
            {
                if (path[strlen(path) - 1] == '/')
                {
                    std::string newPath = encode(path) + encode(ent->d_name);
                    files.push_back(newPath);
                }
                else
                {
                    std::string newPath = encode(path) + "/" + encode(ent->d_name);
                    files.push_back(newPath);
                }
            }
            else if (ent->d_type == DT_DIR && isRecursive)
            {
                if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
                {
                    std::string newPath = path;
                    newPath += "/";
                    newPath += ent->d_name;
                    std::vector<std::string> subFiles = get_files(newPath.c_str(), isRecursive);
                    files.insert(files.end(), subFiles.begin(), subFiles.end());
                }
            }
        }
        closedir(dir);
    }
    else
    {
        std::cout << "Could not open the directory" << std::endl;
    }
    return files;
}

inline bool isUpperCase(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), ::isupper);
}

std::vector<std::string> filter_supported_files(std::vector<std::string> files, std::vector<std::string> supportedExtensions)
{
    std::vector<std::string> filteredFiles;
    for (std::string file : files)
    {
        std::vector<std::string> fileParts = split(file.c_str(), '.');
        std::string extension = fileParts[fileParts.size() - 1];
        if (std::find(supportedExtensions.begin(), supportedExtensions.end(), extension.c_str()) != supportedExtensions.end())
        {

            filteredFiles.push_back(file);
        }
    }
    return filteredFiles;
}

Help::Help()
{
    this->id = CommandID::_HELP;
    this->name = "help";
    this->description = "Prints the help message";
    this->is_valid = false;
}

void Help::init(const char *input)
{
    this->is_valid = true;
}

void Help::print_help()
{
    std::cout << "Usage: " << std::endl;
    std::cout << " -h or --help:\t\t Prints the help message\n"
              << std::endl;
    std::cout << " -i or --info:\t\t Prints the info of the image\n"
              << std::endl;
    std::cout << " -f or --file:\t\t Reads the image from the file path\n"
              << std::endl;
    std::cout << " -d or --dir:\t\t Reads the images from the directory\n"
              << std::endl;
    std::cout << " --recursive:\t\t Reads the images from the directory recursively\n"
              << std::endl;
    std::cout << " -r or --resize:\t Resizes the image to the given size <width>x<height> or <size of one edge>\n"
              << std::endl;
    std::cout << " -e or _export_type:\t Sets the export type of the image <png|jpg>\n"
              << std::endl;
    std::cout << " --output_dir:\t\t Sets the output directory of the image.\n \t\t\t if not defined output directory created 'result' in the input directory\n"
              << std::endl;
    std::cout << " --output_adjective:\t Adds the addjective before the file name\n"
              << std::endl;
    std::cout << " --output_name:\t\t Sets the output name of the image.\n \t\t\t if not defined output name get same as input name\n"
              << std::endl;
    std::cout << " --log_dir <path>:\t Sets the log directory\n"
              << std::endl;
}

Info::Info()
{
    this->id = CommandID::_INFO;
    this->name = "info";
    this->description = "Prints the info of the image";
    this->is_valid = false;
}

void Info::init(const char *input)
{
    this->is_valid = true;
}

File::File()
{
    this->id = CommandID::_FILE;
    this->name = "file";
    this->description = "Reads the image from the file path";
    this->is_valid = false;
}

void File::init(const char *input)
{
    std::string input_str = input;
    filename = getfilename(input_str);
    extension = getfileExtension(input_str);
    file_path = input_str;
    this->is_valid = true;
}

Dir::Dir()
{
    this->id = CommandID::_DIR;
    this->name = "dir";
    this->description = "Reads the images from the directory";
    this->is_valid = false;
}

void Dir::init(const char *input)
{
    dir_path = input;
    is_valid = true;
}

Recursive::Recursive()
{
    this->id = CommandID::_RECURSIVE;
    this->name = "recursive";
    this->description = "Reads the images from the directory recursively";
    this->is_valid = false;
}

void Recursive::init(const char *input)
{
    this->is_valid = true;
}

Resize::Resize()
{
    this->id = CommandID::_RESIZE;
    this->name = "resize";
    this->description = "Resizes the image to the given size <width>x<height> or <size of one edge> ";
    this->is_valid = false;
}

void Resize::init(const char *input)
{
    std::string input_str = input;
    std::vector<std::string> args = split(input_str.c_str(), 'x');
    if (args.size() == 2)
    {
        width = std::stoi(args[0]);
        height = std::stoi(args[1]);
        edge_size = 0;
    }
    else if (args.size() == 1)
    {
        std::vector<std::string> args = split(input_str.c_str(), 'X');
        if (args.size() == 2)
        {
            width = std::stoi(args[0]);
            height = std::stoi(args[1]);
            edge_size = 0;
        }
        else if (args.size() == 1)
        {
            edge_size = std::stoi(args[0]);
            width = 0;
            height = 0;
        }
    }
    else
    {
        std::cout << "Invalid arguments for resize command" << std::endl;
        return;
    }
    this->is_valid = true;
}

ExportType::ExportType()
{
    this->id = CommandID::_EXPORT_TYPE;
    this->name = "export_type";
    this->description = "Sets the export type of the image <png|jpg>";
    this->is_valid = false;
}

void ExportType::init(const char *input)
{
    std::string input_str = input;

    if (input_str == "png")
    {
        this->type = ExportType::PNG;
    }
    else if (input_str == "jpg" || input_str == "jpeg" || input_str == "JPEG" || input_str == "JPG")
    {
        this->type = ExportType::JPG;
    }
    else
    {
        std::cout << "Invalid arguments for _export_type command" << std::endl;
        return;
    }
    this->is_valid = true;
}

Output::Output()
{
    this->id = CommandID::_OUTPUT;
    this->name = "output";
    this->description = "Sets the output directory";
    this->is_valid = false;
    this->output_adjective = "";
    this->output_dir = "";
    this->output_name = "";
}
void Output::init(const char *input)
{
    this->is_valid = true;
}
void Output::set_output_adjective(const char *input)
{
    output_adjective = input;
}
void Output::set_output_dir(const char *input)
{
    output_dir = input;
}
void Output::set_output_name(const char *input)
{
    output_name = input;
}
Log::Log()
{
    this->id = CommandID::_LOG;
    this->name = "log";
    this->description = "Sets the log directory";
    this->is_valid = false;
}
void Log::init(const char *input)
{
    log_path = input;
    is_valid = true;
}
Parser::Parser()
{
    help = new Help();
    info = new Info();
    file = new File();
    dir = new Dir();
    recursive = new Recursive();
    resize = new Resize();
    _export_type = new ExportType();
    output = new Output();
    log = new Log();
    parsing_successful = false;
}
Parser::~Parser()
{
    delete help;
    delete info;
    delete file;
    delete dir;
    delete resize;
    delete recursive;
    delete _export_type;
    delete output;
    delete log;
    if (!output_list.empty())
        output_list.clear();
    if (!file_list.empty())
        file_list.clear();
}
void Parser::print_help()
{
    help->print_help();
}
void Parser::print_version()
{
    std::cout << "Version: 1.0.0" << std::endl;
}
bool Parser::parse(int argc, char *argv[])
{
    if (argc == 1)
    {
        help->print_help();
        return true;
    }

    std::string arg = argv[1];
    if (arg == "-h" || arg == "--help")
    {
        help->print_help();
        return true;
    }
    // parse the arguments
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-f" || arg == "--file")
        {
            file->init(encode(argv[i + 1]).c_str());
            file_list.push_back(file->file_path);
            i++;
        }
        else if (arg == "-d" || arg == "--dir")
        {
            dir->init(argv[i + 1]);
            i++;
        }

        else if (arg == "--recursive")
        {
            recursive->init("init");
        }
        else if (arg == "-r" || arg == "--resize")
        {
            resize->init(argv[i + 1]);
            i++;
        }
        else if (arg == "-e" || arg == "--_export_type")
        {
            _export_type->init(argv[i + 1]);
            i++;
        }
        else if (arg == "-i" || arg == "--info")
        {
            info->init("init");
        }
        else if (arg == "--output_dir")
        {
            output->init("init");
            output->set_output_dir(encode(argv[i + 1]).c_str());
            i++;
        }
        else if (arg == "--output_name")
        {
            output->init("init");
            output->set_output_name(encode(argv[i + 1]).c_str());
            i++;
        }
        else if (arg == "--output_adjective")
        {
            output->init("init");
            output->set_output_adjective(encode(argv[i + 1]).c_str());
            i++;
        }
        else if (arg == "--log_dir")
        {
            log->init(encode(argv[i + 1]).c_str());
            i++;
        }
        else
        {
            std::cout << "Invalid argument: " << arg << std::endl;
            parsing_successful = false;
            return false;
        }
    }
    // end of parsing arguments

    // check if the arguments are valid
    if (file->is_valid)
    {
        file_list.push_back(file->file_path);

        if (output->is_valid)
        {
            std::string output_path;
            output->output_dir == "" ? output_path = getfileDir(file->file_path) : output_path = output->output_dir;
            output->output_adjective == "" ? output_path += "/output" : output_path += "/" + output->output_adjective;
            output->output_name == "" ? output_path += "_" + getfilename(file_list.back()) : output_path += "_" + output->output_name;
            if (_export_type->is_valid)
            {
                if (_export_type->type == ExportType::PNG)
                {
                    output_path += ".png";
                }
                else if (_export_type->type == ExportType::JPG)
                {
                    output_path += ".jpg";
                }
            }
            else
            {
                output_path += ".png";
            }
            output_list.push_back(output_path);
        }
    }

    if (dir->is_valid)
    {
        std::vector<std::string> files = get_files(dir->dir_path.c_str(), recursive->is_valid);
        std::vector<std::string> SupportedFormats = {"PSD", "psd"};
        std::vector<std::string> filtered_files = filter_supported_files(files, SupportedFormats);

        for (int i = 0; i < filtered_files.size(); i++)
        {
            file_list.push_back(filtered_files[i]);
            std::string output_path;

            output->output_dir == "" ? output_path = dir->dir_path + "/result" : output_path = output->output_dir;
            // make directory for result
            std::string command = "mkdir -p " + output_path;
            system(command.c_str());

            output->output_adjective == "" ? output_path += "/output" : output_path += "/" + output->output_adjective;
            output->output_name == "" ? output_path += "_" + getfilename(filtered_files[i]) : output_path += "_" + output->output_name;
            if (_export_type->is_valid)
            {
                if (_export_type->type == ExportType::PNG)
                {
                    output_path += ".png";
                }
                else if (_export_type->type == ExportType::JPG)

                {
                    output_path += ".jpg";
                }
            }
            else
            {
                output_path += ".png";
            }
            output_list.push_back(output_path);
        }
        files.clear();
        filtered_files.clear();
        return true;
    }
}
std::vector<std::string> Parser::get_input_file_paths()
{
    return file_list;
}
std::vector<std::string> Parser::get_output_file_paths()
{
    return output_list;
}
std::string Parser::log_dir()
{
    return log->log_path;
}
unsigned int Parser::width()
{
    return resize->width;
}
unsigned int Parser::height()
{
    return resize->height;
}
unsigned int Parser::edge_size()
{
    return resize->edge_size;
}
bool Parser::isLogValid()
{
    return log->is_valid;
}
bool Parser::isInfoValid()
{
    return info->is_valid;
}
bool Parser::isResizeValid()
{
    return resize->is_valid;
}
std::string Parser::export_type()
{
    if (this->_export_type->is_valid)
    {
        if (this->_export_type->type == ExportType::PNG)
        {
            return "png";
        }
        else if (this->_export_type->type == ExportType::JPG)
        {
            return "jpg";
        }
    }
    return "png";
}