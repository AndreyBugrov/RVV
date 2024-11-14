#pragma once

#include <cstdlib>  // std::abort

#include <string>  // log strings
#include <vector>  // log format
#include <iostream>  // log output
#include <source_location>  // location of code
#include <unordered_map>  // format converter
#include <ctime>  // get current date and time
#include <chrono>  // get current date and time

#include "exception.hpp"  // exceptions
#include "generators.hpp"  // string generator
 
using std::string;
using std::source_location;

enum class LoggerLevel{
    kDebug,
    kInfo,
    kWarning,
    kError,
    kCritical,
};

enum class FormatItem{
    kMessage,
    kLevel,
    kFile,
    kFunction,
    kTime,
    kLine,
    kColumn,
};

class GlobalLogger{
public:
    static std::unordered_map<FormatItem, string> format_converter;

    static void debug(const string& message, const source_location& location) noexcept;
    static void info(const string& message, const source_location& location) noexcept;
    static void warning(const string& message, const source_location& location) noexcept;
    static void error(const string& message, const source_location& location) noexcept;
    static void critical(const string& message, const source_location& location) noexcept;

    static void set_log_level(LoggerLevel level) noexcept;
    static string get_log_level() noexcept;
    static void set_log_format(const string& format);
    static string get_log_format() noexcept;
    static void set_terminate_level(LoggerLevel level) noexcept;
    static void unset_terminate_level() noexcept;
    static string get_terminate_level() noexcept;
    static void set_output_stream(std::ostream& stream) noexcept;

    GlobalLogger() = default;
    virtual ~GlobalLogger(){}


private:
    enum InnerLoggerLevel{
        kDebug = 0,
        kInfo,
        kWarning,
        kError,
        kCritical,
        kNoLevel,
    };
    static std::ostream* stream_;
    static size_t log_level_;
    static size_t terminate_level_;
    static std::vector<string> log_format_;

    GlobalLogger(const GlobalLogger& logger) = delete;
    GlobalLogger& operator=(const GlobalLogger& logger) = delete;
    GlobalLogger(GlobalLogger&& logger) = delete;
    GlobalLogger& operator=(GlobalLogger&& logger) = delete;

    void print_log(const string& message, const source_location& location) const noexcept;
    void log(InnerLoggerLevel level, const string& message, const source_location& location) const noexcept;
};
