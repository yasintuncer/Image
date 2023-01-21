#include <string>
#include <fstream>

static std::string LOG_DIR = ".";

enum LogLevel
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

inline std::string getLogLevelString(LogLevel level)
{
    switch (level)
    {
    case LOG_DEBUG:
        return "DEBUG";
    case LOG_INFO:
        return "INFO";
    case LOG_WARN:
        return "WARN";
    case LOG_ERROR:
        return "ERROR";
    case LOG_FATAL:
        return "FATAL";
    default:
        return "UNKNOWN";
    }
};

inline std::string getCurrentDateTime(std::string s)
{
    time_t now = time(0);

    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    if (s == "now")
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    else if (s == "date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return std::string(buf);
};
inline void Logger(std::string logMsg, LogLevel level)
{
    std::ofstream logFile;
    logFile.open(LOG_DIR + getCurrentDateTime("date") + ".log", std::ios::app);
    logFile << getCurrentDateTime("now") << " " << getLogLevelString(level) << " " << logMsg << std::endl;
    logFile.close();
}