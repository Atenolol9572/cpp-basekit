#include <ctime>
#include <myLogLib.h>

#include <chrono>
#include <cstdarg>
#include <iomanip>
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
        SYSTEMTIME st;
        GetLocalTime(&st);
        char timeStr[64];
        snprintf(timeStr, sizeof(timeStr), "[%04d-%02d-%02d %02d:%02d:%02d.%03d]",
                 st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        LogRecord record;
        record.level = level;
        record.currentTime = timeStr;
        const size_t lastSlashPos = file.find_last_of("/\\");
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
