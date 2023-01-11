#include <string>

namespace Parser
{

    // Supported input formats
    static const char help[2] = {"-h", "--help"};
    static const char version[2] = {"-v", "--version"};
    static const char file[2] = {"-f", "--file"};
    static const char dir[2] = {"-d", "--dir"};
    static const char recursive[2] = {"-r", "--recursive"};
    static const char info[2] = {"-i", "--info"};
    static const char _export[2] = {"-e", "--export"};
    static const char resize[2] = {"-s", "--resize"};
    static const char output_dir = "--output_dir ";
    static const char output_name = "--output_name ";
    static const char output_name_ext = "--output_name_ext ";

    // ----------------------------- COMMANDS -----------------------------
    /*
     * CommandID
     * @brief: Enumerates the commands
     */
    typedef enum CommandID
    {
        HELP = 0,
        VERSION = 1,
        FILE = 2,
        DIR = 3,
        RECURSIVE = 4,
        INFO = 5,
        EXPORT = 6,
        RESIZE = 7,
        OUTPUT_DIR = 8,
        OUTPUT_NAME = 9,
        OUTPUT_NAME_EXT = 10
    } CommandID;

    /*
     * Command
     * @brief: Base class for all commands
     */
    struct Command
    {
        CommandID id;
        const char *name;
        const char *description;
        bool is_valid;
        void virtual init(const char *input) = 0;
    } Command;

    /*
     * VersionCommand
     * @brief: Prints the version of the program
     */
    struct VersionCommand : Command
    {
        Command()
        {
            id = VERSION;
            name = "version";
            description = "Prints the version of the program";
            is_valid = false;
        }

        char *version;
        void print_version();
    };

    /*
     * HelpCommand
     * @brief: Prints the help of the program
     */
    struct HelpCommand : Command
    {
        Command()
        {
            id = HELP;
            name = "help";
            description = "Prints the help of the program";
            is_valid = false;
        }
        void print_help();
    };

    /*
     * FileCommand
     * @brief: Reads a file
     */
    struct FileCommand : Command
    {
        Command()
        {
            id = FILE;
            name = "file";
            description = "Reads a file";
            is_valid = false;
        }
        void init(const char *input) override;
        char *file_name;
        char *file_path;
        char *file_extension;
    };

    /*
     * DirCommand
     * @brief: Reads a directory
     */
    struct DirCommand : Command
    {
        Command()
        {
            id = DIR;
            name = "dir";
            description = "Reads a directory";
            is_valid = false;
        }
        void init(const char *input) override;

        char *dir_path;
    };

    /*
     * RecursiveCommand
     * @brief: Reads a directory recursively
     */
    struct RecursiveCommand : Command
    {
        Command()
        {
            id = RECURSIVE;
            name = "recursive";
            description = "Reads a directory recursively";
            is_valid = false;
        };
    };

    /*
     * InfoCommand
     * @brief: Prints the info of the file
     */
    struct InfoCommand : Command
    {
        Command()
        {
            id = INFO;
            name = "info";
            description = "Prints the info of the file";
            is_valid = false;
        }
        void print_info();
    };

    /*
     * ExportCommand
     * @brief: Exports the file
     */
    struct ExportCommand : Command
    {
        // Supported output formats
        enum
        {
            PNG = 0,
            JPG = 1
        } Command()
        {
            id = EXPORT;
            name = "export";
            description = "Exports the file";
            is_valid = false;
        }
        void init(const char *input) override;
        int format;
    };

    /*
     * ResizeCommand
     * @brief: Resizes the file
     */
    struct ResizeCommand : Command
    {
        Command()
        {
            id = RESIZE;
            name = "resize";
            description = "Resizes the file";
            is_valid = false;
        }
        void init(const char *input) override;
        int dest_width;
        int dest_height;
    };

    /*
     * OutputCommand
     * @brief: Sets the output name
     */
    struct OutputNameCommand : Command
    {
        Command()
        {
            id = OUTPUT;
            name = "output_name";
            description = "Sets the output name, if not defined the output name will be the same as the input file.";
            is_valid = false;
        }
        void init(const char *input) override;

        char *output_name;
    };

    /*
     * OutputCommand
     * @brief: adds a adjective to the name of the output file
     */
    struct OutputNameExtCommand : Command
    {
        Command()
        {
            id = OUTPUT;
            name = "output_name_ext";
            description = "Adds a adjective to the name of the output file. ";
            is_valid = false;
        }
        void init(const char *input) override;
        char *output_name_extension;
    };

    /*
     * OutputDirCommand
     * @brief: Sets the output directory.
     * If not defined the output directory will be set in the same directory as the input file in a folder called 'output'
     */
    struct OutputDirCommand : Command
    {
        Command()
        {
            id = OUTPUT;
            name = "output_dir";
            description = "Sets the output directory. If not define the output directory will be created in the same directory as the input file in a folder called 'output'";
            is_valid = false;
        }
        void init(const char *input) override;
        char *output_dir;
    };

    /*
     * CommandParser
     * @brief: Parses the command line arguments
     * @param argc: Number of arguments
     * @param argv: Array of arguments
     */
    void parse(int argc, char *argv[]);

}
