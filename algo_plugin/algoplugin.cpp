#include "algoplugin.h"
#include "logger/logger.h"

using namespace talos;

namespace talos::plugin {
PluginManager::PluginManager() {}

PluginManager::~PluginManager() {}

PluginManager &PluginManager::instance() {
  static PluginManager instance; // C++11 保证线程安全
  return instance;
}

bool PluginManager::loadAlgoPlugins(const std::string &pluginPath) {
  _lib = new QLibrary(QString::fromStdString(pluginPath));
  if (!_lib->load()) {
    std::string err = std::format("Load plugin failed: {}",
                                  _lib->errorString().toStdString());
    Logger::Error(err);
    return false;
  }

  ToolCreateFunc createTool = (ToolCreateFunc)_lib->resolve("create_tool");
  if (!createTool) {
    Logger::Error("Create tool function not found in plugin: " + pluginPath);
    return false;
  }

  Tool tool = createTool();
  if (!tool.vtable->init(tool.obj)) {
    Logger::Error("Init tool failed: " +
                  std::string(tool.vtable->name(tool.obj)));
    return false;
  }
  std::string toolName = tool.vtable->name(tool.obj);
  Logger::Info("Load tool: " + toolName);
  tool.vtable->run(tool.obj);
  tool.vtable->destroy(tool.obj);

  return true;
}

void PluginManager::unloadAlgoPlugins() {
  if (_lib) {
    _lib->unload();
    _lib->deleteLater();
    _lib = nullptr;
  }
}

} // namespace talos::plugin
