#include "parser.h"

Parser::VersionCommand::print_version()
{
    std::cout << "Version: " << version << std::endl;
}

Parser::HelpCommand::print_help()
{
    std::cout << "Help: " << std::endl;
    std::cout << "    -h, --help: Prints the help of the program" << std::endl;
    std::cout << "    -v, --version: " << Parser::VersionCommand::description << std::endl;
    std::cout << "    -f, --file: " << Parser::FileCommand::description << std::endl;
    std::cout << "    -d, --dir: " << Parser::DirCommand::description << std::endl;
    std::cout << "    -r, --recursive: " << Parser::RecursiveCommand::description << std::endl;
    std::cout << "    -e, --export: " << Parser::ExportCommand::description << std::endl;
    std::cout << "    -s, --resize: " << Parser::ResizeCommand::description << std::endl;
    std::cout << "    --output_dir: " << Parser::OutputDirCommand::description << std::endl;
    std::cout << "    --output_name: " << Parser::OutputNameCommand::description << std::endl;
    std::cout << "    --output_name_ext: " << Parser::OutputNameExtCommand::description << std::endl;
}

Parser::FileCommand::init(const char *input)
{
    input.split(".");
}

Parser::DirCommand::init(const char *input)
{
    dir_path = input;
}


Parser::VersionCommand version_command = Parser::VersionCommand();
Parser::HelpCommand help_command = Parser::HelpCommand();
Parser::FileCommand file_command = Parser::FileCommand();
Parser::DirCommand dir_command = Parser::DirCommand();
Parser::RecursiveCommand recursive_command = Parser::RecursiveCommand();
Parser::ExportCommand export_command = Parser::ExportCommand();
Parser::ResizeCommand resize_command = Parser::ResizeCommand();
Parser::OutputDirCommand output_dir_command = Parser::OutputDirCommand();
Parser::OutputNameExtCommand output_name_ext_command = Parser::OutputNameExtCommand();
Parser::OutputNameCommand output_name_command = Parser::OutputNameCommand();

void Parser::parse(int argc, const char *argv[])
{

    if (argc == 1)
    {
        help_command.is_valid = true;
        std::cout << "No arguments provided, showing help" << std::endl;
        help_command.print_help();
        return;
    }

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            help_command.is_valid = true;
            help_command.print_help();
            break;
        }
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
        {
            version_command.is_valid = true;
        }
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0)
        {
            file_command.is_valid = true;
            file_command.file_path = argv[i + 1];
            i++;
        }
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dir") == 0)
        {
            dir_command.is_valid = true;
            dir_command.dir_path = argv[i + 1];
            i++;
        }
        if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--recursive") == 0)
        {
            recursive_command.is_valid = true;
        }
        if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--export") == 0)
        {
            export_command.is_valid = true;
            export_command.
        }
    }
}
