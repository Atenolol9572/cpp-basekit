#ifndef MYLOG_LIBRARY_H
#define MYLOG_LIBRARY_H

#include <string>

#ifndef LOG_INFO
#define LOG_INFO(Format, ...) \
myLogLib::myLog::GetInstance().writeLog(myLogLib::LogLevel::LOG_INFO, __FILE__, __LINE__, __func__, Format, ##__VA_ARGS__)
#endif

#ifndef LOG_DEBUG
#define LOG_DEBUG(Format, ...) \
myLogLib::myLog::GetInstance().writeLog(myLogLib::LogLevel::LOG_DEBUG, __FILE__, __LINE__, __func__, Format, ##__VA_ARGS__)
#endif

#ifndef LOG_WARNING
#define LOG_WARNING(Format, ...) \
myLogLib::myLog::GetInstance().writeLog(myLogLib::LogLevel::LOG_WARNING, __FILE__, __LINE__, __func__, Format, ##__VA_ARGS__)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(Format, ...) \
myLogLib::myLog::GetInstance().writeLog(myLogLib::LogLevel::LOG_ERROR, __FILE__, __LINE__, __func__, Format, ##__VA_ARGS__)
#endif

#ifndef LOG_FATAL
#define LOG_FATAL(Format, ...) \
myLogLib::myLog::GetInstance().writeLog(myLogLib::LogLevel::LOG_FATAL, __FILE__, __LINE__, __func__, Format, ##__VA_ARGS__)
#endif

namespace myLogLib
{
    class Log;

    enum LogLevel
    {
        LOG_DEBUG = 0,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_FATAL
    };

    class myLog
    {
    public:
        myLog(myLog&& other) = delete;
        myLog(const myLog& other) = delete;
        myLog& operator=(myLog&& other) = delete;
        myLog& operator=(const myLog& other) = delete;

        static myLog& GetInstance();
        void writeLog(LogLevel level, const std::string& file, int line, std::string function, const char* format, ...) const;

    private:
        myLog();
        ~myLog();
        Log* m_log;
    };
} // namespace myLogLib

#endif // MYLOG_LIBRARY_H
