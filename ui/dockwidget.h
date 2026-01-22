#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QWidget>

namespace Ui {
class DockWidget;
}

class DockWidget : public QWidget {
  Q_OBJECT

public:
  explicit DockWidget(QWidget *parent = nullptr);
  ~DockWidget();

  void init();

private:
  Ui::DockWidget *ui;
};

#endif // DOCKWIDGET_H
