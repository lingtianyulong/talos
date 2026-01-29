#ifndef DOCKPANEL_H
#define DOCKPANEL_H

#include "DockWidget.h"
#include <QFont>

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
  void setIconfont(const QString &font);

private:
  void loadFont();
  QFont _iconFont;
};
} // namespace controls::dockpanel

#endif // DOCKPANEL_H
