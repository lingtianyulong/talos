//
// Created by lingt on 2025/5/14.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"

#include "ui/mainwidget.h"
#include <qevent.h>

#include "controls/messagebox/messagebox.h"

using namespace controls::messagebox;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setWindowIcon(QIcon(":/icons/icon.ico"));

  auto mainWidget = new MainWidget(this);
  this->setCentralWidget(mainWidget);

  // auto layout = new QVBoxLayout(this);
  // auto glWidget = new OpenGLWidget();
  // this->setCentralWidget(glWidget);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::closeEvent(QCloseEvent *event) {
  auto ret = MessageBox::warning("确定要退出程序吗?", this);
  if (ret == MessageBox::StandardButton::Ok) {
    event->accept();
  } else {
    event->ignore();
  }
}
