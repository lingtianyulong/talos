//
// Created by xiaoqiang on 2025/3/22.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <string_view>

#ifdef __cplusplus
extern "C" {
#endif
	bool init_logger(const char* path);
	void logger_debug(const char* msg);
	void logger_info(const char* msg);
	void logger_warn(const char* msg);
	void logger_error(const char* msg);
	void logger_trace(const char* msg);
	void logger_flush();
	void shutdown_logger();
#ifdef __cplusplus
}
#endif

namespace talos::Logger {
	bool initLog(std::string_view path);
	void Debug(std::string_view msg);
	void Info(std::string_view msg);
	void Warn(std::string_view msg);
	void Error(std::string_view msg);
	void Trace(std::string_view msg);
	void Flush();
	void Shutdown();
}

#endif // LOGGER_H
