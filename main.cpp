#include "mainwindow.h"

#include "ui/logindialog.h"
#include <QApplication>
#include <QtQuickControls2/QQuickStyle>
#include <QtQuickControls2/QtQuickControls2>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QQuickStyle::setStyle("Material");

  auto dlg = new LoginDialog();
  if (dlg->exec() != QDialog::Accepted) {
    return 0;
  }

  MainWindow w;
  w.show();
  return a.exec();
}
