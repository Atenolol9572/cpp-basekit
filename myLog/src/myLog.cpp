#include "myLog.h"

#include <chrono>
#include <cstdarg>
#include <ctime>
#include <iomanip>

#include <direct.h>
#include <iostream>

#include <Windows.h>

using namespace std;

namespace myLogLib
{
    Log::Log()
        : m_file(nullptr),
          m_bRun(true),
          m_thread(nullptr)
    {
        if (!openFile())
            return;
        m_map[LOG_INFO] = "INFO   ";
        m_map[LOG_DEBUG] = "DEBUG  ";
        m_map[LOG_WARNING] = "WARNING";
        m_map[LOG_ERROR] = "ERROR  ";
        m_map[LOG_FATAL] = "FATAL  ";
        m_thread = new thread(runLogThread, this);
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
        if (m_file)
        {
            fflush(m_file);
            fclose(m_file);
            m_file = nullptr;
        }
    }

    bool Log::getProgramNameAndPath(string& programPath, string& programName)
    {
        char path[MAX_PATH];
        if (GetModuleFileNameA(nullptr, path, MAX_PATH) == 0)
            return false;
        const string fullPath(path);
        const size_t pos = fullPath.find_last_of('\\');
        if (pos == string::npos)
            return false;
        programPath = fullPath.substr(0, pos);
        programName = fullPath.substr(pos + 1);
        const size_t lastDot = programName.find_last_of('.');
        programName = lastDot != string::npos ? programName.substr(0, lastDot) : programName;
        return true;
    }

    bool Log::buildLogPath(const string& programPath, const string& programName)
    {
        const string logPath = programPath + "/Log";
        const DWORD ftp = GetFileAttributesA(logPath.c_str());
        if (ftp == INVALID_FILE_ATTRIBUTES)
        {
            if (_mkdir(logPath.c_str()) == 0 || errno == EEXIST)
                return true;
            return false;
        }
        if (ftp & FILE_ATTRIBUTE_DIRECTORY)
        {
            vector<string> logFiles;
            WIN32_FIND_DATAA findFileData;
            const HANDLE hFind = FindFirstFileA((logPath + "/*.log").c_str(), &findFileData);
            if (hFind != INVALID_HANDLE_VALUE)
            {
                do
                {
                    string filename = findFileData.cFileName;
                    if (filename.size() == programName.size() + 20 && filename.find(programName) == 0)
                        logFiles.push_back(filename);
                }
                while (FindNextFileA(hFind, &findFileData) != 0);
                FindClose(hFind);
            }
            cout << logFiles.size() << "\n";
            if (logFiles.empty())
                return true;
            sort(logFiles.begin(), logFiles.end());
            while (logFiles.size() > 9)
            {
                string filePath = logPath + "/" + logFiles.front();
                DeleteFileA(filePath.c_str());
                logFiles.erase(logFiles.begin());
            }
            return true;
        }
        if (_mkdir(logPath.c_str()) == 0 || errno == EEXIST)
            return true;
        return false;
    }

    bool Log::buildLogFile(const string& programPath, const string& programName)
    {
        const auto now = chrono::system_clock::now();
        const time_t currentTime = chrono::system_clock::to_time_t(now);
        tm localTime{};
        void(localtime_s(&localTime, &currentTime));
        char timeBuf[32];
        strftime(timeBuf, sizeof(timeBuf), "%Y%m%d_%H%M%S", &localTime);
        char filepath[512];
        snprintf(filepath, sizeof(filepath), "%s/Log/%s_%s.log", programPath.c_str(), programName.c_str(), timeBuf);
        const errno_t err = fopen_s(&m_file, filepath, "w");
        if (err != 0 || m_file == nullptr)
        {
            m_file = nullptr;
            return false;
        }
        return true;
    }

    bool Log::openFile()
    {
        string programPath, programName;
        if (!getProgramNameAndPath(programPath, programName))
            return false;
        if (!buildLogPath(programPath, programName))
            return false;
        return buildLogFile(programPath, programName);
    }

    void Log::pushLogRecord(const LogRecord& record)
    {
        {
            lock_guard<mutex> lock(_muFifo);
            m_queue.push(record);
        }
        _cv.notify_one();
    }

    bool Log::popLogRecord(LogRecord& record)
    {
        lock_guard<mutex> lock(_muFifo);
        if (m_queue.empty())
            return false;
        record = m_queue.front();
        m_queue.pop();
        return true;
    }

    string Log::LevelToStr(const LogLevel level)
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
        char logLine[1024];
        while (m_bRun)
        {
            unique_lock<mutex> lock(_muThread);
            _cv.wait(lock);
            while (popLogRecord(record))
            {
                nWriteNum++;
                snprintf(logLine, sizeof(logLine), "%s|%s|%s:%d|[%s]:%s\n",
                         record.currentTime.c_str(),
                         LevelToStr(record.level).c_str(),
                         record.file.c_str(),
                         record.line,
                         record.function.c_str(),
                         record.content.c_str());
                fprintf(m_file, "%s", logLine);
                if (nWriteNum > 120)
                {
                    nWriteNum = 0;
                    fflush(m_file);
                }
            }
        }
        if (m_file)
        {
            fflush(m_file);
            fclose(m_file);
            m_file = nullptr;
        }
    }
} // myLogLib
