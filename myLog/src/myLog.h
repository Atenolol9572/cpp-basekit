#ifndef MY_LOG_H
#define MY_LOG_H

/*!
 * @file            myLog.h
 * @brief			异步日志的类           
 * @note            
 * @version         V1.00
 * @date            2024-07-24 22:40:08
 * @author          Atenolol
 * @copyright
 */

#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include <myLogApi.h>

enum LogLevel
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
};

struct LogRecord
{
    LogLevel level = LOG_DEBUG;
    std::string currentTime;
    std::string file;
    int line = 0;
    std::string function;
    std::string content;
};

class myLog
{
public:
    myLog();
    ~myLog();

    myLog(myLog&& other) = delete;
    myLog(const myLog& other) = delete;
    myLog& operator=(myLog&& other) = delete;
    myLog& operator=(const myLog& other) = delete;

    void pushLogRecord(const LogRecord& record);
    static myLog& getInstance();

private:
    FILE* m_pFile;
    std::mutex m_muFifo;
    std::mutex m_muThread;
    std::atomic_bool m_bRun;
    std::thread* m_pThread;
    std::condition_variable m_cvThread;
    std::queue<LogRecord> m_queue;
    std::map<LogLevel, std::string> m_map;

    static bool getProgramNameAndPath(std::string& programPath, std::string& programName);
    static bool buildLogPath(const std::string& programPath, const std::string& programName);
    bool buildLogFile(const std::string& programPath, const std::string& programName);
    bool openFile();
    bool isQueueEmpty();
    bool popLogRecord(LogRecord& record);
    std::string levelToStr(LogLevel level);
    static void runLogThread(myLog* pParam);
    void logThread();
};

#endif // MY_LOG_H
