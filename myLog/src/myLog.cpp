#include "myLog.h"

#include <chrono>
#include <cstdarg>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <direct.h>
#include <iostream>
#include <Windows.h>

namespace myLogLib
{
    Log::Log()
        : m_bRun(true),
          m_thread(nullptr)
    {
        if (!openFile())
            return;
        m_map[LOG_INFO] = "INFO   ";
        m_map[LOG_DEBUG] = "DEBUG  ";
        m_map[LOG_WARNING] = "WARNING";
        m_map[LOG_ERROR] = "ERROR  ";
        m_map[LOG_FATAL] = "FATAL  ";
        m_thread = new std::thread(runLogThread, this);
    }

    Log::~Log()
    {
        m_bRun = false;
        if (nullptr != m_thread)
        {
            _cv.notify_all();
            m_thread->join();
            delete m_thread;
            m_thread = nullptr;
        }
        if (m_stream.is_open())
            m_stream.close();
    }

    bool Log::getProgramNameAndPath(std::string& programPath, std::string& programName)
    {
        char path[MAX_PATH];
        if (GetModuleFileNameA(nullptr, path, MAX_PATH) == 0)
            return false;
        std::string fullPath(path);
        size_t pos = fullPath.find_last_of('\\');
        if (pos == std::string::npos)
            return false;
        programPath = fullPath.substr(0, pos);
        programName = fullPath.substr(pos + 1);
        size_t lastDot = programName.find_last_of('.');
        programName = lastDot != std::string::npos ? programName.substr(0, lastDot) : programName;
        return true;
    }

    bool Log::buildLogPath(const std::string& programPath, const std::string& programName)
    {
        std::string logPath = programPath + "/Log";
        const DWORD ftp = GetFileAttributesA(logPath.c_str());
        if (ftp == INVALID_FILE_ATTRIBUTES)
        {
            if (_mkdir(logPath.c_str()) == 0 || errno == EEXIST)
                return true;
            return false;
        }
        if (ftp & FILE_ATTRIBUTE_DIRECTORY)
        {
            std::vector<std::string> logFiles;
            WIN32_FIND_DATAA findFileData;
            HANDLE hFind = FindFirstFileA((logPath + "/*.log").c_str(), &findFileData);
            if (hFind != INVALID_HANDLE_VALUE)
            {
                do
                {
                    std::string filename = findFileData.cFileName;
                    if (filename.size() == programName.size() + 20 && filename.find(programName) == 0)
                        logFiles.push_back(filename);
                }
                while (FindNextFileA(hFind, &findFileData) != 0);
                FindClose(hFind);
            }
            std::cout << logFiles.size() << "\n";
            if (logFiles.empty())
                return true;
            std::sort(logFiles.begin(), logFiles.end());
            while (logFiles.size() > 9)
            {
                std::string filePath = logPath + "/" + logFiles.front();
                DeleteFileA(filePath.c_str());
                logFiles.erase(logFiles.begin());
            }
            return true;
        }
        if (_mkdir(logPath.c_str()) == 0 || errno == EEXIST)
            return true;
        return false;
    }

    bool Log::buildLogFile(const std::string& programPath, const std::string& programName)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime{};
        void(localtime_s(&localTime, &currentTime));
        std::ostringstream filepath;
        filepath << programPath << "/Log/" << programName << "_" << std::put_time(&localTime, "%Y%m%d_%H%M%S") <<
            ".log";
        m_stream.open(filepath.str(), std::ios::out);
        return m_stream.is_open();
    }

    bool Log::openFile()
    {
        std::string programPath, programName;
        if (!getProgramNameAndPath(programPath, programName))
            return false;
        if (!buildLogPath(programPath, programName))
            return false;
        return buildLogFile(programPath, programName);
    }

    void Log::pushLogRecord(const LogRecord& record)
    {
        {
            std::lock_guard<std::mutex> lock(_muFifo);
            m_queue.push(record);
        }
        _cv.notify_one();
    }

    bool Log::popLogRecord(LogRecord& record)
    {
        std::lock_guard<std::mutex> lock(_muFifo);
        if (m_queue.empty())
            return false;
        record = m_queue.front();
        m_queue.pop();
        return true;
    }

    std::string Log::LevelToStr(LogLevel level)
    {
        return m_map.at(level);
    }

    void Log::runLogThread(Log* pParam)
    {
        pParam->logThread();
    }

    void Log::logThread()
    {
        LogRecord record;
        int nWriteNum = 0;
        while (m_bRun)
        {
            std::unique_lock<std::mutex> lock(_muThread);
            _cv.wait(lock);
            while (popLogRecord(record))
            {
                nWriteNum++;
                std::ostringstream os;
                os << record.currentTime
                    << "|"
                    << LevelToStr(record.level)
                    << "|"
                    << record.file
                    << ":"
                    << record.line
                    << "|" << "["
                    << record.function
                    << "]:"
                    << record.content << "\n";
                m_stream << os.str();
                if (nWriteNum > 20)
                {
                    nWriteNum = 0;
                    m_stream.flush();
                }
            }
        }
        if (m_stream.is_open())
            m_stream.flush();
    }
} // myLogLib
