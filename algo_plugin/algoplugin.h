#ifndef ALGOPLUGIN_H
#define ALGOPLUGIN_H
#include "rust_core/algo_base_tool.h"
#include <string>

namespace talos::plugin {
class PluginManager {
public:
  PluginManager();
  ~PluginManager();

  bool LoadAlgoPlugins(const std::string &pluginDir);

private:
};

} // namespace talos::plugin

#endif // ALGOPLUGIN_H