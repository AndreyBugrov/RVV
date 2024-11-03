#include "single_logger.hpp"

std::unordered_map<FormatItem, string> SingleLogger::format_converter = {
    std::pair<FormatItem, string>(FormatItem::kMessage, "%m"),
    std::pair<FormatItem, string>(FormatItem::kFunction, "%f"),
    std::pair<FormatItem, string>(FormatItem::kFile, "%u"), // unit
    std::pair<FormatItem, string>(FormatItem::kTime, "%t"),
    std::pair<FormatItem, string>(FormatItem::kLine, "%r"),  // row
    std::pair<FormatItem, string>(FormatItem::kColumn, "%c"),
    std::pair<FormatItem, string>(FormatItem::kLevel, "%l")
};

SingleLogger* SingleLogger::self_ = nullptr;

void SingleLogger::log(InnerLoggerLevel level, const string& message, const source_location& location) const noexcept{
    if(log_level_ <= level){
        print_log(message, location);
    }
    if(terminate_level_ <= level){
        std::abort();
    }
}

void SingleLogger::debug(const string& message, const source_location& location) const noexcept{
    log(InnerLoggerLevel::kDebug, message, location);
}
void SingleLogger::info(const string& message, const source_location& location) const noexcept{
    log(InnerLoggerLevel::kInfo, message, location);
}
void SingleLogger::warning(const string& message, const source_location& location) const noexcept{
    log(InnerLoggerLevel::kWarning, message, location);
}
void SingleLogger::error(const string& message, const source_location& location) const noexcept{
    log(InnerLoggerLevel::kError, message, location);
}
void SingleLogger::critical(const string& message, const source_location& location) const noexcept{
    log(InnerLoggerLevel::kCritical, message, location);
}

void SingleLogger::set_log_level(LoggerLevel level) noexcept{
    switch (level)
    {
    case LoggerLevel::kDebug:
        log_level_ = InnerLoggerLevel::kDebug;
        break;
    case LoggerLevel::kInfo:
        log_level_ = InnerLoggerLevel::kInfo;
        break;
    case LoggerLevel::kWarning:
        log_level_ = InnerLoggerLevel::kWarning;
        break;
    case LoggerLevel::kError:
        log_level_ = InnerLoggerLevel::kError;
        break;
    case LoggerLevel::kCritical:
        log_level_ = InnerLoggerLevel::kCritical;
        break;
    }
}
void SingleLogger::set_terminate_level(LoggerLevel level) noexcept{
    switch (level)
    {
    case LoggerLevel::kDebug:
        terminate_level_ = InnerLoggerLevel::kDebug;
        break;
    case LoggerLevel::kInfo:
        terminate_level_ = InnerLoggerLevel::kInfo;
        break;
    case LoggerLevel::kWarning:
        terminate_level_ = InnerLoggerLevel::kWarning;
        break;
    case LoggerLevel::kError:
        terminate_level_ = InnerLoggerLevel::kError;
        break;
    case LoggerLevel::kCritical:
        terminate_level_ = InnerLoggerLevel::kCritical;
        break;
    }
}

void SingleLogger::unset_terminate_level() noexcept{
    terminate_level_ = InnerLoggerLevel::kNoLevel;
}

string SingleLogger::get_terminate_level() const noexcept{
    switch (terminate_level_)
    {
    case InnerLoggerLevel::kDebug:
        return "DEBUG";
    case InnerLoggerLevel::kInfo:
        return "INFO";
    case InnerLoggerLevel::kWarning:
        return "WARNING";
    case InnerLoggerLevel::kError:
        return "ERROR";
    case InnerLoggerLevel::kCritical:
        return "CRITICAL";
    case InnerLoggerLevel::kNoLevel:
        return "NO LEVEL";
    default:
        return "NO LEVEL";
    }
}

void SingleLogger::set_output_stream(std::ostream& stream) noexcept{
    stream_ = &stream;
}

SingleLogger* SingleLogger::get_instance() noexcept{
    if(!self_){
        self_ = new SingleLogger();
    }
    return self_;
}

void SingleLogger::destroy_instance() noexcept{
    if(self_){
        delete self_;
    }
}

void SingleLogger::set_log_format(const string& format){
    // aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
}

string SingleLogger::get_log_format() const noexcept{
    string output = "";
    for(string lexem : log_format_){
        output.append(lexem);
    }
    return output;
}

string SingleLogger::get_log_level() const noexcept{
    switch (log_level_)
    {
    case InnerLoggerLevel::kDebug:
        return "DEBUG";
    case InnerLoggerLevel::kInfo:
        return "INFO";
    case InnerLoggerLevel::kWarning:
        return "WARNING";
    case InnerLoggerLevel::kError:
        return "ERROR";
    case InnerLoggerLevel::kCritical:
        return "CRITICAL";
    default:
        return "NO LEVEL";
    }
}

void SingleLogger::print_log(const string& message, const source_location& location) const noexcept{
    for(std::string lexem: log_format_){
        if(lexem == "%m"){
            const string msg(message);
            for(auto c: msg){
                stream_->operator<<(c);
            }
        }
        if(lexem == "%f"){
            stream_->operator<<(location.function_name());
            continue;
        }
        if(lexem == "%u"){
            stream_->operator<<(location.file_name());
            continue;
        }
        if(lexem == "%t"){
            auto current_date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            stream_->operator<<(::ctime(&current_date));
            continue;
        }
        if(lexem == "%r"){
            stream_->operator<<(location.line());
            continue;
        }
        if(lexem == "%c"){
            stream_->operator<<(location.column());
            continue;
        }
        if(lexem == "%l"){
            switch (log_level_)
            {
            case InnerLoggerLevel::kDebug:
                stream_->operator<<("DEBUG");
                break;
            case InnerLoggerLevel::kInfo:
                stream_->operator<<("INFO");
                break;
            case InnerLoggerLevel::kWarning:
                stream_->operator<<("WARNING");
                break;
            case InnerLoggerLevel::kError:
                stream_->operator<<("ERROR");
                break;
            case InnerLoggerLevel::kCritical:
                stream_->operator<<("CRITICAL");
            default:
                Exception exception(ErrorType::kValueError, generate_string("Unsupported number of InnerLoggerLevel variable value: ", log_level_));
                break;
            }
            continue;
        }
        stream_->operator<<(lexem.data());
    }
    stream_->operator<<("\n");
}