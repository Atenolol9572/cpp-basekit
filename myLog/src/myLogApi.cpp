#include "myLogApi.h"

#include <cstdarg>
#include <cctype>
#include <string>

#if !defined(_MSC_VER)
#include <cstring>
#endif

#if defined(_WIN32) && !defined(__CYGWIN__)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "myLog.h"

using namespace std;
inline string safeFormat(const char* format, va_list args)
{
    try
    {
        va_list argsCopy;
        va_copy(argsCopy, args);
        const int len = vsnprintf(nullptr, 0, format, argsCopy);
        va_end(argsCopy);
        if (len <= 0 || len > 4096)
            return "[FORMAT ERROR: length invalid]";
        char buffer[1024];
#if defined(_WIN32) && !defined(__CYGWIN__)
        void(vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, format, args));
#else
        void(vsnprintf(buffer, sizeof(buffer), format, args));
#endif
        return {buffer, static_cast<size_t>(len)};
    }
    catch (...)
    {
        return "[FORMAT ERROR: invalid format string or argument type]";
    }
}

#ifdef __cplusplus
extern "C" {
#endif

MYLOG_API void log_write(int level, const char* file, const int line, const char* function, const char* format, ...)
{
    if (level < 0 || level > 4)
        return;
    if (!format)
        format = "(null format)";
    std::string sanitized;
    std::string content;
    va_list args;
    va_start(args, format);
#ifdef _DEBUG
    content = safeFormat(format, args);
#else
    {
        char buffer[1024];
#if defined(_WIN32) && !defined(__CYGWIN__)
        void(vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, format, args));
#else
        void(vsnprintf(buffer, sizeof(buffer), format, args));
#endif
        content = buffer;
    }
#endif
    va_end(args);
    char timeStr[64];
#if defined(_WIN32) && !defined(__CYGWIN__)
    SYSTEMTIME st;
    GetLocalTime(&st);
    void(snprintf(timeStr, sizeof(timeStr), "[%04d-%02d-%02d %02d:%02d:%02d.%03d]", st.wYear, st.wMonth, st.wDay,
                  st.wHour, st.wMinute, st.wSecond, st.wMilliseconds));
#else
    timeval tv{};
    gettimeofday(&tv, nullptr);
    const time_t t = tv.tv_sec;
    tm tm_info{};
    localtime_r(&t, &tm_info);
    snprintf(timeStr, sizeof(timeStr), "[%04d-%02d-%02d %02d:%02d:%02d.%03ld]", tm_info.tm_year + 1900,
             tm_info.tm_mon + 1, tm_info.tm_mday, tm_info.tm_hour, tm_info.tm_min, tm_info.tm_sec, tv.tv_usec / 1000);
#endif
    LogRecord record;
    record.level = static_cast<LogLevel>(level);
    record.currentTime = timeStr;
    if (nullptr != file && strlen(file) > 0)
    {
        string fileStr(file);
        const size_t pos = fileStr.find_last_of("/\\");
        record.file = pos != string::npos ? fileStr.substr(pos + 1) : fileStr;
    }
    else
        record.file = "";
    if (line > 0)
        record.line = line;
    if (nullptr != function && strlen(function) > 0)
        record.function = function;
    else
        record.function = "";
    record.content = content;
    myLog::getInstance().pushLogRecord(record);
}

#ifdef __cplusplus
}
#endif
