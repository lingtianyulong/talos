#ifndef ALGOPLUGIN_H
#define ALGOPLUGIN_H
#include "rust_core/algo_base_tool.h"
#include <QLibrary>
#include <QPointer>
#include <string>

#ifdef _WIN32
#ifdef ALGO_PLUGIN_LIBRARY
#define ALGO_PLUGIN_EXPORT __declspec(dllexport)
#else
#define ALGO_PLUGIN_EXPORT __declspec(dllimport)
#endif
#else
#define ALGO_PLUGIN_EXPORT
#endif

/// @brief 插件管理类
/// @details 负责加载和卸载算法插件, 用于对接 rust 算法插件
/// @note 插件必须实现ToolCreateFunc函数，返回Tool结构体
/// @note 插件必须实现ToolVTable结构体，包含name, init, run, destroy函数指针
/// @note 插件必须实现ToolObject结构体，作为插件的上下文

namespace talos::plugin {
class ALGO_PLUGIN_EXPORT PluginManager {
public:
  ~PluginManager();
  static PluginManager &instance();

  PluginManager(const PluginManager &) = delete;
  PluginManager &operator=(const PluginManager &) = delete;
  PluginManager(PluginManager &&) = delete;
  PluginManager &operator=(PluginManager &&) = delete;

  bool loadAlgoPlugins(const std::string &pluginDir);
  void unloadAlgoPlugins();

private:
  PluginManager();
  QPointer<QLibrary> _lib = nullptr;
};

} // namespace talos::plugin

#endif // ALGOPLUGIN_H