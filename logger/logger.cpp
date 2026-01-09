//
// Created by xiaoqiang on 2025/3/22.
//

#include "logger.h"

namespace talos::Logger {
    bool initLog(std::string_view path)
    {
        return init_logger(path.data());
    }

    void Debug(std::string_view msg)
    {
        logger_debug(msg.data());
    }

    void Info(std::string_view msg)
    {
        logger_info(msg.data());
    }

    void Warn(std::string_view msg)
    {
        logger_warn(msg.data());
    }

    void Error(std::string_view msg)
    {
        logger_error(msg.data());
    }

    void Trace(std::string_view msg)
    {
        logger_trace(msg.data());
    }

    void Flush() {
        logger_flush();
    }

    void Shutdown() {
        shutdown_logger();
    }

} // namespace talos::logger
