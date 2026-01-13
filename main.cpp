#include "logger/logger.h"
#include "ui/logindialog.h"
#include "ui/mainwidget.h"
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QObject>
#include <QtQuickControls2/QQuickStyle>
#include <QtQuickControls2/QtQuickControls2>

using namespace talos;

int main(int argc, char *argv[]) {

  QQuickStyle::setStyle("Fusion");

  QApplication a(argc, argv);
  // 适配高分屏
  QApplication::setHighDpiScaleFactorRoundingPolicy(
      Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

  auto version = QString::fromUtf8(GIT_VERSION);

  QString path = QApplication::applicationDirPath() + "/sys_config";
  // load config of logger
  auto logConfigPath = path + "/log_config.json";
  if (QDir dir(path); !dir.exists() && !dir.mkdir(path)) {
    QMessageBox::warning(nullptr, "Warn", QObject::tr("创建路径失败"));
  }

  if (!Logger::initLog(logConfigPath.toStdString().c_str())) {
    QMessageBox::warning(nullptr, "Warn", QObject::tr("日志初始化失败"));
  }

  Logger::Info("程序开始启动......");

  // 加载 qss
  QStringList qssFiles = {":/theme/buttons.qss", ":/theme/checkbox.qss",
                          ":/theme/menubar.qss"};
  QString qss = "";
  for (auto &file : qssFiles) {
    QFile f(file);
    if (f.open(QFile::ReadOnly)) {
      qss += f.readAll();
      qss += "\n";
    }
  }
  qApp->setStyleSheet(qss);

  // auto dlg = new LoginDialog();
  // if (dlg->exec() != QDialog::Accepted) {
  //   return 0;
  // }

  MainWidget w;
  w.setWindowIcon(QIcon(":/icons/icon.ico"));
  w.setWindowTitle("Talos");
  w.resize(1024, 768);
  w.show();
  return a.exec();
}
