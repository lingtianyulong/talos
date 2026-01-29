#include "dock_panel.h"
#include "DockManager.h"
#include "IconProvider.h"
#include <QFile>

namespace controls::dockpanel {
DockPanel::DockPanel(const QString &title, QWidget *parent)
    : ads::CDockWidget(title, parent) {
  // init();
}

DockPanel::DockPanel(ads::CDockManager *manager, const QString &title,
                     QWidget *parent)
    : ads::CDockWidget(manager, title, parent) {
  // init();
}

DockPanel::~DockPanel() {}

void DockPanel::initFeatures() {
  // 每个实例特定的初始化逻辑（目前为空，全局逻辑已移动到 initGlobalAdsConfig）
  setFeature(ads::CDockWidget::DockWidgetClosable, true);
  setFeature(ads::CDockWidget::DockWidgetFloatable, true);
  setFeature(ads::CDockWidget::DockWidgetMovable, true);
  setFeature(ads::CDockWidget::DockWidgetPinnable, true);
}

void DockPanel::initGlobalAdsConfig() {
  // 设置 ADS 全局配置
  ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasUndockButton,
                                   true);
  ads::CDockManager::setConfigFlag(ads::CDockManager::FocusHighlighting, true);
  ads::CDockManager::setConfigFlag(ads::CDockManager::OpaqueSplitterResize,
                                   true);
  ads::CDockManager::setAutoHideConfigFlag(
      ads::CDockManager::AutoHideFeatureEnabled, true);
  ads::CDockManager::setAutoHideConfigFlag(
      ads::CDockManager::DockAreaHasAutoHideButton, true);
  ads::CDockManager::setAutoHideConfigFlag(
      ads::CDockManager::AutoHideButtonCheckable, true);

  // 使用 IconProvider 全局设置图标
  ads::CIconProvider &provider = ads::CDockManager::iconProvider();

  // 辅助函数：将 SVG 图标颜色修改为白色
  auto createWhiteIcon = [](const QString &path) -> QIcon {
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
      QByteArray data = file.readAll();
      // 替换颜色值 #000000 为 #FFFAFA
      data.replace("#000000", "#FFFAFA");
      data.replace("fill:#000000", "fill:#FFFAFA");
      data.replace("stroke:#000000", "stroke:#FFFAFA");
      data.replace("style=\"-inkscape-stroke:none\"",
                   "style=\"fill:#FFFAFA;-inkscape-stroke:none\"");

      QPixmap pixmap;
      pixmap.loadFromData(data);
      return QIcon(pixmap);
    }
    return QIcon(path);
  };

  provider.registerCustomIcon(ads::TabCloseIcon,
                              createWhiteIcon(":/ads/images/close-button.svg"));
  provider.registerCustomIcon(ads::DockAreaCloseIcon,
                              createWhiteIcon(":/ads/images/close-button.svg"));

  QIcon pinIcon;
  // 手动处理 Pin 图标的白色版本
  auto createWhitePinIcon = [&](const QString &path) -> QPixmap {
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
      QByteArray data = file.readAll();
      data.replace("#000000", "#FFFAFA");
      data.replace("fill:#000000", "fill:#FFFAFA");
      data.replace("stroke:#000000", "stroke:#FFFAFA");
      // 针对 vs-pin-button.svg 缺少 fill 属性的情况进行特殊处理
      data.replace("style=\"-inkscape-stroke:none\"",
                   "style=\"fill:#FFFAFA;-inkscape-stroke:none\"");
      QPixmap pixmap;
      pixmap.loadFromData(data);
      return pixmap;
    }
    return QPixmap(path);
  };

  QPixmap unpinned = createWhitePinIcon(":/ads/images/vs-pin-button.svg");
  QPixmap pinned = createWhitePinIcon(":/ads/images/vs-pin-button-pinned.svg");
  pinIcon.addPixmap(unpinned, QIcon::Normal, QIcon::Off);
  pinIcon.addPixmap(unpinned, QIcon::Active, QIcon::Off);
  pinIcon.addPixmap(pinned, QIcon::Normal, QIcon::On);
  pinIcon.addPixmap(pinned, QIcon::Active, QIcon::On);
  provider.registerCustomIcon(ads::AutoHideIcon, pinIcon);

  provider.registerCustomIcon(
      ads::DockAreaMenuIcon,
      createWhiteIcon(":/ads/images/tabs-menu-button.svg"));
  provider.registerCustomIcon(
      ads::DockAreaUndockIcon,
      createWhiteIcon(":/ads/images/detach-button.svg"));
  provider.registerCustomIcon(
      ads::DockAreaMinimizeIcon,
      createWhiteIcon(":/ads/images/minimize-button.svg"));
}

} // namespace controls::dockpanel