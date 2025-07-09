#include <ctime>
#include <myLogLib.h>

#include <chrono>
#include <cstdarg>
#include <iomanip>
#include <sstream>
#include <utility>

#include <Windows.h>

#include "myLog.h"

namespace myLogLib
{
    myLog& myLog::GetInstance()
    {
        static myLog instance;
        return instance;
    }

    void myLog::writeLog(const LogLevel level, const std::string& file, const int line, std::string function,
                         const char* format, ...) const
    {
        if (line <= 0 || function.empty())
            return;
        char buffer[MAX_PATH];
        va_list args;
        va_start(args, format);
        const int length = vsnprintf(buffer, MAX_PATH, format, args);
        if (length < 0)
        {
            va_end(args);
            return;
        }
        va_end(args);
        const auto now = std::chrono::system_clock::now();
        const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime{};
        void(localtime_s(&localTime, &currentTime));
        auto duration = now.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;
        std::ostringstream str;
        str << std::put_time(&localTime, "[%Y-%m-%d %H:%M:%S") << '.' << std::setw(3) << std::setfill('0') <<
            milliseconds.
            count() << "]";
        LogRecord record;
        record.level = level;
        record.currentTime = str.str();
        size_t lastSlashPos = file.find_last_of("/\\");
        if (lastSlashPos != std::string::npos)
            record.file = file.substr(lastSlashPos + 1);
        else
            record.file = file;
        record.line = line;
        record.function = std::move(function);
        record.content = std::string(buffer, length);
        m_log->pushLogRecord(record);
    }

    myLog::myLog()
        : m_log(new Log())
    {
    }

    myLog::~myLog()
    {
        delete m_log;
        m_log = nullptr;
    }
} // myLogLib
