#ifndef DOCKPANEL_H
#define DOCKPANEL_H

#include "DockWidget.h"

namespace ads {
class CDockManager;
}

namespace controls::dockpanel {
class DockPanel : public ads::CDockWidget {
  Q_OBJECT
public:
  explicit DockPanel(const QString &title, QWidget *parent = nullptr);
  DockPanel(ads::CDockManager *manager, const QString &title,
            QWidget *parent = nullptr);
  ~DockPanel() override;

  static void initGlobalAdsConfig();
  void initFeatures();
};
} // namespace controls::dockpanel

#endif // DOCKPANEL_H
