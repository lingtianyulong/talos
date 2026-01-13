//
// Created by lingt on 2025/5/14.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"

#include "ui/mainwidget.h"
#include <QPainter>
#include <QPainterPath>
#include <qevent.h>

#include "controls/messagebox/messagebox.h"

using namespace controls::messagebox;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);
  this->setWindowIcon(QIcon(":/icons/icon.ico"));
  this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
  this->setAttribute(Qt::WA_TranslucentBackground);
  // this->setAttribute(Qt::WA_NoSystemBackground, false);
  this->statusBar()->hide();

  auto mainWidget = new MainWidget(this);
  // central 自己画背景
  mainWidget->setAttribute(Qt::WA_StyledBackground);
  mainWidget->setStyleSheet(R"(
      QWidget {
           background: qlineargradient(
           x1:0, y1:0,
           x2:0, y2:1,
           stop:0 rgba(255, 255, 255, 220),
           stop:1 rgba(123, 92, 184, 220)
          );
          border-radius: 5px;
      }
  )");

  this->setCentralWidget(mainWidget);

  // auto layout = new QVBoxLayout(this);
  // auto glWidget = new OpenGLWidget();
  // this->setCentralWidget(glWidget);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::paintEvent(QPaintEvent *event) {
  if (isMaximized()) {
    return;
  }

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QRect rect = this->rect();
  rect.adjust(1, 1, -1, -1);

  // 绘制圆角矩形
  QPainterPath path;
  path.addRoundedRect(rect, 12, 12);

  painter.fillPath(path, QBrush(QColor(40, 40, 40, 220)));
}

void MainWindow::changeEvent(QEvent *event) {
  if (event->type() == QEvent::WindowStateChange) {
    // 状态改变时强制刷新布局和重绘，解决无边框窗口状态同步滞后
    if (this->centralWidget()) {
      this->centralWidget()->update();
    }
    this->update();
  }
  QMainWindow::changeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  auto ret = MessageBox::warning("确定要退出程序吗?", this);
  if (ret == MessageBox::StandardButton::Ok) {
    event->accept();
  } else {
    event->ignore();
  }
}
