#include "mainwindow.h"

#include <QApplication>
#include "ui/logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto dlg = new LoginDialog();
    dlg->exec();


    MainWindow w;
    w.show();
    return a.exec();
}
