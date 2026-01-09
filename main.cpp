#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include "config/system_config/sys_config.h"
#include "logger/logger.h"
#include "ui/widgets/logindialog.h"
#include "utils/dump_helper.h"
#include "db/db_util.h"
#include "ui/mainwindow.h"

using namespace talos::sys::config;
using namespace talos;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 适配高分屏
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QString dumpDir = QApplication::applicationDirPath() + "/dumps";
    // 显示创建目录
    if (!QDir().exists(dumpDir)) {
        if (!QDir().mkpath(dumpDir)) {
            QMessageBox::critical(nullptr, "Error", QString("Unable to create dump directory: %1").arg(dumpDir));
            return -1;
        }
    }

    QByteArray dumpDirUtf8 = dumpDir.toUtf8();
    if (!utils::init_dump(dumpDirUtf8.constData())) {
        QMessageBox::critical(nullptr, "Error", "Dump dir not found.");
    }

    QString path = QApplication::applicationDirPath() + "/sys_config";
    // load config of logger
    auto logConfigPath = path + "/log_config.json";
    if (QDir dir(path); !dir.exists() && !dir.mkdir(path)) {
        QMessageBox::warning(nullptr, "Warn", QObject::tr("创建路径失败"));
    }

    if (!Logger::initLog(logConfigPath.toStdString().c_str())) {
        QMessageBox::warning(nullptr, "Warn", QObject::tr("日志初始化失败"));
    }

    Logger::Info("程序开始启动!");

    // 加载系统配置文件
    auto filePath = path + "/sys_config.json";
    bool bSuccess = RustSysConfig::getInstance().loadConfig(filePath.toStdString());

    auto width_opt = RustSysConfig::getInstance().getWinWidth();
    int width = 0;
    if (width_opt) {
        width = width_opt.value_or(1920);
    }

    auto height_opt = RustSysConfig::getInstance().getWinHeight();
    int height = 0;
    if (height_opt) {
        height = height_opt.value_or(1080);
    }

    if (!bSuccess) {
        RustSysConfig::getInstance().saveConfig(filePath.toStdString());
    }

    QString dbPath = QCoreApplication::applicationDirPath() + "/talos.db";
    if (!db::DbUtil::instance().init(dbPath.toStdString())) {
        Logger::Error("数据库初始化失败!");
    }

    // 加载 qss
    QStringList qssFiles = {
        ":/theme/buttons.qss",
        ":/theme/checkbox.qss",
        ":/theme/menubar.qss"
    };
    QString qss = "";
    for (auto &file: qssFiles) {
        QFile f(file);
        if (f.open(QFile::ReadOnly)) {
            qss += f.readAll();
            qss += "\n";
        }
    }
    qApp->setStyleSheet(qss);

    auto dlg = new LoginDialog();
    if (dlg->exec() != QDialog::Accepted) {
        return 0;
    }

    MainWindow mainWindow;
    mainWindow.setWindowTitle("talos");
    mainWindow.resize(width, height);
    mainWindow.show();

    return app.exec();
}
