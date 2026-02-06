#ifndef MY_LOG_LIB_API_H
#define MY_LOG_LIB_API_H

#include <myLogApi_global.h>

#ifdef __cplusplus
extern "C" {
#endif

MYLOG_API void log_write(int level, const char* file, int line, const char* function, const char* format, ...);

#ifdef __cplusplus
}
#endif

#ifndef LOG_DEBUG
#define LOG_DEBUG(Format, ...)   log_write(0, __FILE__, __LINE__, __func__, Format, ##__VA_ARGS__)
#endif

#ifndef LOG_INFO
#define LOG_INFO(Format, ...)    log_write(1, __FILE__, __LINE__, __func__, Format, ##__VA_ARGS__)
#endif


#ifndef LOG_WARNING
#define LOG_WARNING(Format, ...) log_write(2, __FILE__, __LINE__, __func__, Format, ##__VA_ARGS__)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(Format, ...)   log_write(3, __FILE__, __LINE__, __func__, Format, ##__VA_ARGS__)
#endif

#ifndef LOG_FATAL
#define LOG_FATAL(Format, ...)   log_write(4, __FILE__, __LINE__, __func__, Format, ##__VA_ARGS__)
#endif

#endif
