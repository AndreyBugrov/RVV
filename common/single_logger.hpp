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

class SingleLogger{
public:
    static SingleLogger* get_instance();
    static void destroy_instance();
    static std::unordered_map<FormatItem, string> format_converter;

    void debug(const string& message, const source_location& location) const noexcept;
    void info(const string& message, const source_location& location) const noexcept;
    void warning(const string& message, const source_location& location) const noexcept;
    void error(const string& message, const source_location& location) const noexcept;
    void critical(const string& message, const source_location& location) const noexcept;

    void set_log_level(LoggerLevel level);
    string get_log_level() const noexcept;
    void set_log_format(const string& format);
    string get_log_format() const;
    void set_terminate_level(LoggerLevel level);
    void unset_terminate_level();
    string get_terminate_level() const noexcept;
    void set_output_stream(std::ostream& stream);


private:
    enum InnerLoggerLevel{
        kDebug = 0,
        kInfo,
        kWarning,
        kError,
        kCritical,
        kNoLevel,
    };
    static SingleLogger* self_;
    std::ostream* stream_;
    size_t log_level_;
    size_t terminate_level_;
    std::vector<string> log_format_;

    SingleLogger(): stream_(&std::cout), log_level_(kNoLevel), terminate_level_(kNoLevel), log_format_{"%l", ": ", "%m"}{}
    virtual ~SingleLogger(){}
    SingleLogger(const SingleLogger& logger) = delete;
    SingleLogger& operator=(const SingleLogger& logger) = delete;

    void print_log(const string& message, const source_location& location) const noexcept;
    void log(InnerLoggerLevel level, const string& message, const source_location& location) const noexcept;
};
