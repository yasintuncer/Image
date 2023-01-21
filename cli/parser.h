#include <string>
#include <vector>
#include <iostream>

// Define the unsupported characters for the program to encode
static const std::wstring UNDEFINED_SYMBOLS = L"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

/*--------------------------------------------------------
 * Command Arguments
 */
enum CommandID
{
    _VERSION = 0,
    _HELP = 1,
    _INFO = 2,
    _FILE = 3,
    _DIR = 4,
    _RECURSIVE = 5,
    _RESIZE = 6,
    _EXPORT_TYPE = 7,
    _OUTPUT = 8,
    _LOG = 9,
    _UNKNOWN = -1
};

/*--------------------------------------------------------
 * Command
 * @brief: Base class for all commands
 */
typedef struct Command
{
    CommandID id;
    std::string name;
    std::string description;
    bool is_valid;
    void virtual init(const char *input) = 0;
} Command;

/*--------------------------------------------------------
 * Version
 * @brief: Version command
 */
typedef struct Version : public Command
{
    Version();
    void init(const char *input) override;
    void print_version();
} Version;

/*---------------------------------------------------------
 * Help
 * @brief: Help command
 */
typedef struct Help : public Command
{
    Help();
    void init(const char *input) override;
    void print_help();
} Help;

/*---------------------------------------------------------
 * Info
 * @brief: Info command
 */
typedef struct Info : public Command
{
    Info();
    void init(const char *input) override;
} Info;

/*---------------------------------------------------------
 * File
 * @brief: File command
 */
typedef struct File : public Command
{
    File();
    void init(const char *input) override;
    std::string filename;
    std::string extension;
    std::string file_path;

} File;

/*---------------------------------------------------------
 * Dir
 * @brief: Dir command
 */
typedef struct Dir : public Command
{
    Dir();
    void init(const char *input) override;
    std::string dir_path;
} Dir;

/*---------------------------------------------------------
 * Recursive
 * @brief: Recursive command
 */
typedef struct Recursive : public Command
{
    Recursive();
    void init(const char *input) override;
} Recursive;

/*---------------------------------------------------------
 * Resize
 * @brief: Resize command
 */
typedef struct Resize : public Command
{
    Resize();
    void init(const char *input) override;
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int edge_size = 0;
} Resize;

/*---------------------------------------------------------
 * ExportType
 * @brief: ExportType command
 */
typedef struct ExportType : public Command
{
    enum Enum
    {
        PNG = 0,
        JPG = 1,
        BMP = 2,
        UNKNOWN = -1
    };
    ExportType();
    void init(const char *input) override;
    Enum type;
} ExportType;

/*---------------------------------------------------------
 * Output
 * @brief: Output command
 */
typedef struct Output : public Command
{
    Output();
    void init(const char *input) override;
    void set_output_name(const char *input);
    void set_output_dir(const char *input);
    void set_output_adjective(const char *input);
    std::string output_name;
    std::string output_dir;
    std::string output_adjective;

    std::string get_output_path();
} Output;



/*---------------------------------------------------------
* Log
* @brief: Log command
*/
typedef struct Log : public Command
{
    Log();
    void init(const char *input) override;
    std::string log_path;
} Log;


/*---------------------------------------------------------
 * Parser
 * @brief: Parser class
 */
class Parser
{
public:
    //constructor and destructor
    Parser();
    ~Parser();
    
    // ----------- print functions
    // print_help() prints the help message
    void print_help();
    // print_version() prints the version message
    void print_version();
    // ----------- end print functions


    // parses the command line arguments
    bool parse(int argc, char *argv[]);
    
    // ----------- get functions
    // get_input_files() returns the list of input files
    std::vector<std::string> get_input_file_paths();
    // get_output_paths() returns the list of output paths
    std::vector<std::string> get_output_file_paths();

    // return width if return 0 then no width was specified
    unsigned int width();
    // return height if return 0 then no height was specified
    unsigned int height();
    // return edge_size if return 0 then no edge_size was specified
    unsigned int edge_size();
    // return export type
    std::string export_type();
    std::string log_dir();
    bool isInfoValid();
    bool isLogValid();
    bool isResizeValid();

private:
    std::vector<std::string> file_list;
    std::vector<std::string> output_list;
    Help *help;
    Version *version;
    Info *info;
    File *file;
    Dir *dir;
    Recursive *recursive;
    Resize *resize;
    ExportType *_export_type;
    Output *output;
    Log *log;
    bool parsing_successful;
};
