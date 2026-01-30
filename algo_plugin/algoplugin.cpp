#include "algoplugin.h"
#include "logger/logger.h"
#include <QDir>
#include <QFileInfoList>
// #include <QStringList>

using namespace talos;

namespace talos::plugin {
PluginManager::PluginManager() {}

PluginManager::~PluginManager() {}

PluginManager &PluginManager::instance() {
  static PluginManager instance;
  return instance;
}

bool PluginManager::loadAlgoPlugins(const QString &pluginPath) {
  // 检查插件目录是否存在
  QDir dir(pluginPath);
  if (!dir.exists()) {
    Logger::Error("Plugin directory not found: " + pluginPath.toStdString());
    return false;
  }

  Logger::Info("Searching for plugins in: " + dir.absolutePath().toStdString());

  QStringList filters;
  filters << "*.dll";

  QFileInfoList files =
      dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

  if (files.isEmpty()) {
    Logger::Error("No plugin found in directory: " + pluginPath.toStdString());
    return false;
  }
  Logger::Info("Found " + std::to_string(files.size()) +
               " potential plugin files");
  for (const QFileInfo &file : files) {
    QString pluginFilePath = file.absoluteFilePath();
    Logger::Info("Processing file: " + pluginFilePath.toStdString());

    if (pluginFilePath.contains("base_tool")) {
      Logger::Info("Skipping base_tool: " + pluginFilePath.toStdString());
      continue;
    }

    auto lib = new QLibrary(pluginFilePath);
    if (!lib->load()) {
      Logger::Error("Load plugin failed: " + pluginFilePath.toStdString() +
                    " Error: " + lib->errorString().toStdString());
      delete lib;
      continue;
    }

    ToolCreateFunc createTool = (ToolCreateFunc)lib->resolve("create_tool");
    if (!createTool) {
      Logger::Error("Create tool function not found in plugin: " +
                    pluginFilePath.toStdString() +
                    " Error: " + lib->errorString().toStdString());
      lib->unload();
      delete lib;
      continue;
    }

    Tool tool = createTool();
    if (!tool.vtable || !tool.vtable->init(tool.obj)) {
      Logger::Error("Init tool failed or vtable is null for: " +
                    pluginFilePath.toStdString());
      lib->unload();
      delete lib;
      continue;
    }

    auto toolName = tool.vtable->name(tool.obj);
    if (!toolName) {
      Logger::Error("Tool name is null for: " + pluginFilePath.toStdString());
      lib->unload();
      delete lib;
      continue;
    }

    // 不在 map 中的工具才添加
    if (!_tools.contains(toolName)) {
      _tools.emplace(toolName, tool);
    }

    _libs.push_back(lib);
    Logger::Info("Load tool success: " + std::string(toolName));
  }

  Logger::Info("Total tools loaded: " + std::to_string(_tools.size()));
  return true;
}

void PluginManager::unloadAlgoPlugins() {
  // 先销毁所有工具
  for (auto &item : _tools) {
    Tool &tool = item.second;
    if (tool.vtable) {
      tool.vtable->destroy(tool.obj);
    }
  }
  _tools.clear();

  // 再卸载所有库
  for (auto &lib : _libs) {
    if (lib) {
      lib->unload();
      lib->deleteLater();
    }
  }
  _libs.clear();
}

} // namespace talos::plugin
