#include "myLog.h"

#include <cerrno>
#include <ctime>
#include <sys/stat.h>

#include <algorithm>
#include <vector>

#if defined(__MINGW64__) || defined(__MINGW32__)
#include <cstring>
#include <stdexcept>
#endif

#if defined(_WIN32) && !defined(__CYGWIN__)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <direct.h>
#include <windows.h>
#define PATH_SEP "\\"
#define MKDIR(dir) _mkdir(dir)
#else
#include <climits>
#include <dirent.h>
#include <unistd.h>
#define PATH_SEP "/"
#define MKDIR(dir) mkdir((dir), 0755)
#endif

using namespace std;

myLog::myLog()
    : m_pFile(nullptr),
      m_bRun(true),
      m_pThread(nullptr)
{
    if (!openFile())
        return;
    m_map[LOG_INFO] = "INFO   ";
    m_map[LOG_DEBUG] = "DEBUG  ";
    m_map[LOG_WARNING] = "WARNING";
    m_map[LOG_ERROR] = "ERROR  ";
    m_map[LOG_FATAL] = "FATAL  ";
    m_bRun.store(true);
    m_pThread = new thread(runLogThread, this);
}

myLog::~myLog()
{
    m_bRun.store(false);
    if (nullptr != m_pThread)
    {
        m_cvThread.notify_all();
        if (m_pThread->joinable())
            m_pThread->join();
        delete m_pThread;
        m_pThread = nullptr;
    }
    if (nullptr != m_pFile)
    {
        void(fflush(m_pFile));
        void(fclose(m_pFile));
        m_pFile = nullptr;
    }
}

bool myLog::getProgramNameAndPath(string& programPath, string& programName)
{
#if defined(_WIN32) && !defined(__CYGWIN__)
    char path[MAX_PATH];
    if (GetModuleFileNameA(nullptr, path, MAX_PATH) == 0)
        return false;
#else
    char path[PATH_MAX];
    const ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count == -1)
        return false;
    path[count] = '\0';
#endif
    const string fullPath(path);
    const size_t pos = fullPath.find_last_of(PATH_SEP);
    if (pos == string::npos)
        return false;
    programPath = fullPath.substr(0, pos);
    programName = fullPath.substr(pos + 1);
    const size_t lastDot = programName.find_last_of('.');
    programName = lastDot != string::npos ? programName.substr(0, lastDot) : programName;
    return true;
}

bool myLog::buildLogPath(const string& programPath, const string& programName)
{
    const string logPath = programPath + "/Log";
    struct stat info{};
    if (stat(logPath.c_str(), &info) != 0)
    {
        if (MKDIR(logPath.c_str()) == 0 || errno == EEXIST)
            return true;
        return false;
    }
    if (!(info.st_mode & S_IFDIR))
        return false;
    vector<string> logFiles;
#if defined(_WIN32) && !defined(__CYGWIN__)
    WIN32_FIND_DATAA findFileData;
    const HANDLE hFind = FindFirstFileA((logPath + "/*.log").c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            string filename = findFileData.cFileName;
            if (filename.find(programName) == 0 && filename.size() >= 5 &&
                filename.substr(filename.size() - 4) == ".log")
                logFiles.push_back(filename);
        }
        while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
#else
    DIR* dir = opendir(logPath.c_str());
    if (dir)
    {
        dirent* entry;
        while ((entry = readdir(dir)) != nullptr)
        {
            string filename = entry->d_name;
            if (filename.find(programName) == 0 && filename.size() >= 5 &&
                filename.substr(filename.size() - 4) == ".log")
                logFiles.push_back(filename);
        }
        closedir(dir);
    }
#endif
    if (logFiles.empty())
        return true;
    sort(logFiles.begin(), logFiles.end());
    while (logFiles.size() > 9)
    {
        string filePath = logPath + "/" + logFiles.front();
#if defined(_WIN32) && !defined(__CYGWIN__)
        DeleteFileA(filePath.c_str());
#else
        remove(filePath.c_str());
#endif
        logFiles.erase(logFiles.begin());
    }
    return true;
}

bool myLog::buildLogFile(const string& programPath, const string& programName)
{
    char timeStr[64];
    char filepath[512];
    const time_t t = time(nullptr);
    tm tmStruct{};
#if defined(_WIN32) && !defined(__CYGWIN__)
    void(localtime_s(&tmStruct, &t));
    const DWORD pid = GetCurrentProcessId();
#else
    localtime_r(&t, &tmStruct);
    pid_t pid = getpid();
#endif
    void(snprintf(timeStr, sizeof(timeStr), "%04d%02d%02dT%02d%02d%02d_PID_%d", tmStruct.tm_year + 1900,
                  tmStruct.tm_mon + 1, tmStruct.tm_mday, tmStruct.tm_hour, tmStruct.tm_min, tmStruct.tm_sec,
                  static_cast<int>(pid)));
    void(snprintf(filepath, sizeof(filepath), "%s/Log/%s_%s.log", programPath.c_str(), programName.c_str(), timeStr));
#if defined(_WIN32) && !defined(__CYGWIN__)
    const errno_t err = fopen_s(&m_pFile, filepath, "w");
    if (err != 0 || nullptr == m_pFile)
#else
    m_pFile = fopen(filepath, "w");
    if (nullptr == m_pFile)
#endif
    {
        m_pFile = nullptr;
        return false;
    }
    return true;
}

bool myLog::openFile()
{
    string programPath, programName;
    if (!getProgramNameAndPath(programPath, programName))
        return false;
    if (!buildLogPath(programPath, programName))
        return false;
    return buildLogFile(programPath, programName);
}

bool myLog::isQueueEmpty()
{
    lock_guard<mutex> lock(m_muFifo);
    return m_queue.empty();
}

void myLog::pushLogRecord(const LogRecord& record)
{
    {
        lock_guard<mutex> lock(m_muFifo);
        m_queue.push(record);
    }
    m_cvThread.notify_one();
}

myLog& myLog::getInstance()
{
    static myLog instance;
    return instance;
}

bool myLog::popLogRecord(LogRecord& record)
{
    lock_guard<mutex> lock(m_muFifo);
    if (m_queue.empty())
        return false;
    record = m_queue.front();
    m_queue.pop();
    return true;
}

string myLog::levelToStr(const LogLevel level)
{
    return m_map.at(level);
}

void myLog::runLogThread(myLog* pParam)
{
    pParam->logThread();
}

void myLog::logThread()
{
    LogRecord record;
    int nWriteNum = 0;
#if defined(_WIN32) && !defined(__CYGWIN__)
    char logLine[MAX_PATH];
#else
    char logLine[PATH_MAX];
#endif
    while (m_bRun.load() || !isQueueEmpty())
    {
        unique_lock<mutex> lock(m_muThread);
        while (m_bRun.load() && isQueueEmpty())
            m_cvThread.wait(lock);
        if (!m_bRun.load() && isQueueEmpty())
            break;
        while (popLogRecord(record))
        {
            nWriteNum++;
            int offset = 0;
            offset += snprintf(logLine + offset, sizeof(logLine) - offset, "%s|%s|", record.currentTime.c_str(),
                               levelToStr(record.level).c_str());
            if (!record.file.empty())
            {
                offset += snprintf(logLine + offset, sizeof(logLine) - offset, "%s:", record.file.c_str());
                if (record.line > 0)
                    offset += snprintf(logLine + offset, sizeof(logLine) - offset, "%d|", record.line);
            }
            if (!record.function.empty())
                offset += snprintf(logLine + offset, sizeof(logLine) - offset, "[%s]:", record.function.c_str());
            void(snprintf(logLine + offset, sizeof(logLine) - offset, "%s\n", record.content.c_str()));
            void(fprintf(m_pFile, "%s", logLine));
            if (++nWriteNum >= 120)
            {
                nWriteNum = 0;
                void(fflush(m_pFile));
            }
        }
    }
    if (nullptr != m_pFile)
        void(fflush(m_pFile));
}
