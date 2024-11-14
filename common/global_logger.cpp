#include "single_logger.hpp"

std::unordered_map<FormatItem, string> GlobalLogger::format_converter = {
    std::pair<FormatItem, string>(FormatItem::kMessage, "%m"),
    std::pair<FormatItem, string>(FormatItem::kFunction, "%f"),
    std::pair<FormatItem, string>(FormatItem::kFile, "%u"), // unit
    std::pair<FormatItem, string>(FormatItem::kTime, "%t"),
    std::pair<FormatItem, string>(FormatItem::kLine, "%r"),  // row
    std::pair<FormatItem, string>(FormatItem::kColumn, "%c"),
    std::pair<FormatItem, string>(FormatItem::kLevel, "%l")
};

GlobalLogger* GlobalLogger::self_ = nullptr;

void GlobalLogger::log(InnerLoggerLevel level, const string& message, const source_location& location) const noexcept{
    if(log_level_ <= level){
        print_log(message, location);
    }
    if(terminate_level_ <= level){
        std::abort();
    }
}

void GlobalLogger::debug(const string& message, const source_location& location) const noexcept{
    log(InnerLoggerLevel::kDebug, message, location);
}
void GlobalLogger::info(const string& message, const source_location& location) const noexcept{
    log(InnerLoggerLevel::kInfo, message, location);
}
void GlobalLogger::warning(const string& message, const source_location& location) const noexcept{
    log(InnerLoggerLevel::kWarning, message, location);
}
void GlobalLogger::error(const string& message, const source_location& location) const noexcept{
    log(InnerLoggerLevel::kError, message, location);
}
void GlobalLogger::critical(const string& message, const source_location& location) const noexcept{
    log(InnerLoggerLevel::kCritical, message, location);
}

void GlobalLogger::set_log_level(LoggerLevel level) noexcept{
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
void GlobalLogger::set_terminate_level(LoggerLevel level) noexcept{
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

void GlobalLogger::unset_terminate_level() noexcept{
    terminate_level_ = InnerLoggerLevel::kNoLevel;
}

string GlobalLogger::get_terminate_level() const noexcept{
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

void GlobalLogger::set_output_stream(std::ostream& stream) noexcept{
    stream_ = &stream;
}

GlobalLogger* GlobalLogger::get_instance() noexcept{
    if(!self_){
        self_ = new GlobalLogger();
    }
    return self_;
}

void GlobalLogger::destroy_instance() noexcept{
    if(self_){
        delete self_;
    }
}

void GlobalLogger::set_log_format(const string& format){
    // aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
}

string GlobalLogger::get_log_format() const noexcept{
    string output = "";
    for(string lexem : log_format_){
        output.append(lexem);
    }
    return output;
}

string GlobalLogger::get_log_level() const noexcept{
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

void GlobalLogger::print_log(const string& message, const source_location& location) const noexcept{
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