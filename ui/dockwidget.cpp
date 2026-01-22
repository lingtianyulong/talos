#include "dockwidget.h"
#include "DockManager.h"
#include "ui_dockwidget.h"
#include <QLabel>
#include <QSizePolicy>
#include <QTextEdit>
#include <QVBoxLayout>

DockWidget::DockWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::DockWidget) {
  ui->setupUi(this);
  init();
}

DockWidget::~DockWidget() { delete ui; }

void DockWidget::init() {

  // **1. 设置自动隐藏配置（可选）
  //    这个配置使得停靠面板支持自动隐藏/显示
  ads::CDockManager::setAutoHideConfigFlags(
      ads::CDockManager::DefaultAutoHideConfig);
  ads::CDockManager::setConfigFlag(ads::CDockManager::FocusHighlighting, true);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  // **2. 创建 DockManager
  ads::CDockManager *dockManager = new ads::CDockManager(this);
  layout->addWidget(dockManager);
  dockManager->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  // **3. 中央区域（主编辑区）
  QLabel *central = new QLabel("Central Editor Area");
  central->setAlignment(Qt::AlignCenter);
  central->setStyleSheet("font-size: 18px; color: #222;");
  //   this->setCentralWidget(central);
  //   mainWindow.setCentralWidget(central);

  // **4. 创建右侧属性面板
  ads::CDockWidget *propDock = new ads::CDockWidget("Properties", this);
  QTextEdit *propText = new QTextEdit("Properties content …");
  propDock->setWidget(propText);

  // 允许自动隐藏与停靠、浮动等功能
  propDock->setFeatures(ads::CDockWidget::DockWidgetClosable |
                        ads::CDockWidget::DockWidgetMovable |
                        ads::CDockWidget::DockWidgetFloatable |
                        ads::CDockWidget::DockWidgetPinnable);

  // 添加到右侧边栏
  dockManager->addDockWidget(ads::RightDockWidgetArea, propDock);

  // **5. 创建底部输出面板
  ads::CDockWidget *outDock = new ads::CDockWidget("Output", this);
  QTextEdit *outText = new QTextEdit("Build log / runtime output …");
  outDock->setWidget(outText);

  outDock->setFeatures(ads::CDockWidget::DockWidgetClosable |
                       ads::CDockWidget::DockWidgetMovable |
                       ads::CDockWidget::DockWidgetFloatable |
                       ads::CDockWidget::DockWidgetPinnable);
  dockManager->addDockWidget(ads::BottomDockWidgetArea, outDock);

  // **6. 左侧资源管理器面板
  ads::CDockWidget *leftDock = new ads::CDockWidget("Project Explorer", this);
  QTextEdit *leftText = new QTextEdit("Project files …");
  leftDock->setWidget(leftText);

  leftDock->setFeatures(ads::CDockWidget::DockWidgetClosable |
                        ads::CDockWidget::DockWidgetMovable |
                        ads::CDockWidget::DockWidgetFloatable |
                        ads::CDockWidget::DockWidgetPinnable);
  dockManager->addDockWidget(ads::LeftDockWidgetArea, leftDock);
}