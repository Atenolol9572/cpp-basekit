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

#include <myLogLib.h>

namespace myLogLib
{
    struct LogRecord
    {
        LogLevel level;
        std::string currentTime;
        std::string file;
        int line;
        std::string function;
        std::string content;
    };

    class Log
    {
    public:
        Log();
        ~Log();

        Log(Log&& other) = delete;
        Log(const Log& other) = delete;
        Log& operator=(Log&& other) = delete;
        Log& operator=(const Log& other) = delete;

        void pushLogRecord(const LogRecord& record);

    private:
        FILE* m_file;
        std::atomic_bool m_bRun;
        std::thread* m_thread;
        std::mutex _muThread, _muFifo;
        std::condition_variable _cv;
        std::queue<LogRecord> m_queue;
        std::map<LogLevel, std::string> m_map;

        static bool getProgramNameAndPath(std::string& programPath, std::string& programName);
        static bool buildLogPath(const std::string& programPath, const std::string& programName);
        bool buildLogFile(const std::string& programPath, const std::string& programName);
        bool openFile();
        bool popLogRecord(LogRecord& record);
        std::string LevelToStr(LogLevel level);
        static void runLogThread(Log* pParam);
        void logThread();
    };
} // namespace myLogLib

#endif // MY_LOG_H
