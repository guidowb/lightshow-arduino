#include "LogManager.h"

#define MAX_MSG_SIZE 1024

Logger::Logger(const char *tag) {
    this->tag = tag;
}

void Logger::debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(DEBUG, fmt, args);
    va_end(args);
}

void Logger::info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(INFO, fmt, args);
    va_end(args);
}

void Logger::warning(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(WARNING, fmt, args);
    va_end(args);
}

void Logger::error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(ERROR, fmt, args);
    va_end(args);
}

void Logger::log(LogLevel level, const char *fmt, va_list args) {
    LogManager::log(tag, level, fmt, args);
}

ConnectionManager *LogManager::connection = NULL;

LogManager::LogManager(ConnectionManager *connection) {
    this->connection = connection;
}

void LogManager::log(const char *tag, LogLevel level, const char *fmt, va_list args) {

    char buffer[MAX_MSG_SIZE];
    char *dst = buffer;
    size_t remaining = MAX_MSG_SIZE;

    size_t used = snprintf(dst, remaining, "%s %s ", tag, toString(level));
    dst += used;
    remaining -= used;

    vsnprintf(dst, remaining, fmt, args);

    if (Serial) Serial.printf("%s\n", buffer);
    if (connection) connection->send("lightshow/log/%c", "%s", buffer);
}

const char *LogManager::toString(LogLevel level) {
    switch (level) {
    case DEBUG:   return "DEBUG";
    case INFO:    return "INFO";
    case WARNING: return "WARNING";
    case ERROR:   return "ERROR";
    default:      return "-";
    }
}
