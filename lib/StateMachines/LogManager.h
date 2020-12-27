#ifndef _LOGMANAGER_H_
#define _LOGMANAGER_H_

#include "ConnectionManager.h"
#include "Arduino.h"

enum LogLevel {
    DEBUG=0,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    Logger(const char *tag);
    void debug(const char *fmt, ...);
    void info(const char *fmt, ...);
    void warning(const char *fmt, ...);
    void error(const char *fmt, ...);

private:
    void log(LogLevel level, const char *fmt, ...);

private:
    const char *tag;
};

class LogManager {
public:
    LogManager(ConnectionManager *connection = NULL);

public:
    static void log(const char *tag, LogLevel level, const char *fmt, ...);
    static const char *toString(LogLevel level);

private:
    static ConnectionManager *connection;
};

#endif