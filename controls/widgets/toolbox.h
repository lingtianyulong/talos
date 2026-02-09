#ifndef TOOLBOX_H
#define TOOLBOX_H
#include <QListWidget>

namespace controls::widgets {
class ToolBox : public QListWidget {
    Q_OBJECT
public:
    explicit ToolBox(QWidget* parent = nullptr);
    ~ToolBox() override;

    /// @brief 添加工具
    /// @param toolName 工具名称(此处主要为算法名称)
    void addTool(const QString& toolName);
    void loadTools();

private:
    void init();
};
}  // namespace controls::widgets

#endif  // TOOLBOX_H