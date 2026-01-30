#include "toolbox.h"
#include "../../algo_plugin/algoplugin.h"

using namespace talos;

namespace controls::widgets {
ToolBox::ToolBox(QWidget *parent) : QListWidget(parent) {
  init();
  loadTools();
}

ToolBox::~ToolBox() {}

void ToolBox::init() {
  setDragEnabled(true);
  setIconSize(QSize(16, 16));
  setStyleSheet("QListWidget { border: none; background-color: transparent; }");
}

/// @brief 添加工具
/// @param toolName 工具名称(此处主要为算法名称)
void ToolBox::addTool(const QString &toolName) {
  auto item = new QListWidgetItem(toolName, this);
  item->setData(Qt::UserRole, toolName);
  addItem(item);
}

/// @brief 加载所有工具
void ToolBox::loadTools() {
  auto tools = plugin::PluginManager::instance().tools();
  for (const auto &[name, tool] : tools) {
    addTool(QString::fromStdString(name));
  }
}

} // namespace controls::widgets
