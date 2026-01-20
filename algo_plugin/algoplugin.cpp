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
    Logger::Error("Load plugin failed: " + _lib->errorString().toStdString());
    return false;
  }
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
