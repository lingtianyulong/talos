//
// Created by lingt on 2025/5/19.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_MainWidget.h" resolved

#include "mainwidget.h"
#include "ui_MainWidget.h"

#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QImageReader>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QScreen>
#include <QTimer>

#include "../logger/logger.h"
#include "../utils/font_helper.h"
#include "capture/rust/capture_rust.h"
#include "controls/buttons/default_button.h"

using namespace controls::buttons;
using namespace controls::image_viewer;
using namespace talos::screen_capture;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWidget) {
  ui->setupUi(this);

  initUI();
  initTitle();

  //   this->setWindowIcon(QIcon(":/icons/icon.ico"));
  //   this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

  // initMenu();

  // _view = new ImageViewer(this);

  // _mainLayout->addWidget(_view);
  // setLayout(_mainLayout);
}

MainWidget::~MainWidget() { delete ui; }

/**
 * @brief 创建 title 栏的按钮
 * @param title 按钮文本, utf8 字符串, 在 iconfont 中的字体符号
 * @return 返回 QPushbutton 若创建失败,则返回 nullptr
 */
QPushButton *MainWidget::createTitleButton(const QString &title) {
  auto button = new QPushButton(this);
  button->setFont(_iconfont);
  button->setText(title);
  return button;
}

void MainWidget::initMenu() {
  auto menuBar = new QMenuBar(this);
  if (_mainLayout == nullptr) {
    _mainLayout = new QVBoxLayout();
  }
  menuBar->setProperty("theme", "light");
  _mainLayout->setMenuBar(menuBar);

  auto fileMenu = menuBar->addMenu("文件(&F)");
  fileMenu->setProperty("theme", "light");

  auto openMenu = fileMenu->addAction("打开(&O)");
  fileMenu->addSeparator();
  auto startMenu = fileMenu->addAction("开始抓取");
  auto stopMenu = fileMenu->addAction("停止录屏");
  fileMenu->addSeparator();

  auto closeMenu = fileMenu->addAction("关闭(&X)");

  auto editMenu = menuBar->addMenu("编辑(&E)");

  auto helpMenu = menuBar->addMenu("帮助(&H)");
  helpMenu->setProperty("theme", "light");
  auto aboutMenu = helpMenu->addAction("关于");

  connect(openMenu, &QAction::triggered, this, &MainWidget::onOpenFile);
  connect(startMenu, &QAction::triggered, this, &MainWidget::onStartCapture);
  connect(stopMenu, &QAction::triggered, this, &MainWidget::onStopCapture);
}

void MainWidget::onOpenFile() {
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("打开文件"), "",
      tr("所有文件 (*.*);;图像文件 (*.png *.jpg);;文本文件 (*.txt)"));

  if (!filePath.isEmpty()) {
    qDebug() << "Selected file:" << filePath;
  }

  QPixmap pixmap(filePath);
  _view->setImage(pixmap);
}

void MainWidget::onStartCapture() {
  auto screen = QGuiApplication::primaryScreen();
  QSize logicalSize = screen->size();
  qreal dpr = screen->devicePixelRatio();
  QSize sz = logicalSize * dpr;

  int width = sz.width() - 1;
  int height = sz.height() - 1;

  auto size = QSize(width, height);

  // _timer = new QTimer(this);
  if (!_captureInited) {
    _captureInited = init_capture(width, height);
    if (!_captureInited) {
      return;
    }
  }

  bool bstart = start_capture();
  if (!bstart) {
    return;
  }

  if (_timer == nullptr) {
    _timer = new QTimer(this);
  }
  connect(_timer, &QTimer::timeout, this, [this, size]() {
    if (!is_frame_ready()) {
      return;
    }
    auto buffer = get_frame_buffer();
    QImage img(buffer, size.width(), size.height(), QImage::Format_BGR888);
    _view->setImage(img);
  });
  _timer->start(33);
}

void MainWidget::onStopCapture() {
  stop_capture();
  // 先停止计时器, 再停止 rust 后台线程
  if (_timer) {
    _timer->stop();
    _timer->deleteLater();
  }
}

void MainWidget::initTitle() {
  if (_mainLayout == nullptr) {
    _mainLayout = new QVBoxLayout(this);
  }
  if (_titleLayout == nullptr) {
    _titleLayout = new QHBoxLayout(); // 移除 this 以避免顶层布局冲突
  }

  _titleLayout->setSpacing(5);
  _titleLayout->setContentsMargins(0, 0, 0, 0);
  _titleLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);

  // auto width = _titleLayout->width();
  // auto height = _titleLayout->height();

  auto min_btn = createTitleButton(QString::fromUtf8("\ue67a"));
  min_btn->setStyleSheet("QPushButton {"
                         "background-color: transparent;"
                         "color: snow;"
                         "max-width: 30px;"
                         "max-height: 30px;"
                         "}"
                         "QPushButton:hover {"
                         "background-color: rgb(211, 211, 211);"
                         "}");

  connect(min_btn, &QPushButton::clicked, this, [&]() {
    if (auto win = window()) {
      win->showMinimized();
    }
    // this->showMinimized();
    // if (auto parent = qobject_cast<QMainWindow *>(this->parent())) {
    //   parent->showMinimized();
    // }
  });

  _titleLayout->addWidget(min_btn);

  auto max_btn = createTitleButton(QString::fromUtf8("\ue653"));
  max_btn->setStyleSheet("QPushButton {"
                         "background-color: transparent;"
                         "color: snow;"
                         "max-width: 30px;"
                         "max-height: 30px;"
                         "}"
                         "QPushButton:hover {"
                         "background-color: rgb(211, 211, 211);"
                         "}");

  connect(max_btn, &QPushButton::clicked, [=]() {
    QWidget *win = window();
    if (!win)
      return;

    // 鲁棒性判定逻辑：同时检查标志位和几何尺寸
    bool isMax =
        win->isMaximized() || (win->windowState() & Qt::WindowMaximized);
    if (!isMax && win->screen()) {
      if (win->geometry() == win->screen()->availableGeometry()) {
        isMax = true;
      }
    }

    if (isMax) {
      // 核心修复：手动清除标志位，强制刷新 OS 层面的还原指令
      win->setWindowState(win->windowState() & ~Qt::WindowMaximized);
      win->showNormal();
    } else {
      win->showMaximized();
    }
  });

  _titleLayout->addWidget(max_btn);

  auto close_btn = createTitleButton(QString::fromUtf8("\ue624"));
  close_btn->setStyleSheet("QPushButton {"
                           "background-color: transparent;"
                           "color: snow;"
                           "max-width: 30px;"
                           "max-height: 30px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: red;"
                           "}");

  _titleLayout->addWidget(close_btn);

  _titleLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);

  connect(close_btn, &QPushButton::clicked, [&]() {
    this->close();
    if (auto parent = qobject_cast<QMainWindow *>(this->parent())) {
      parent->close();
    }
  });

  _mainLayout->addLayout(_titleLayout);

  // _mainLayout->addLayout(layout);
}

void MainWidget::initUI() {
  QString fontFamily =
      utils::FontHelper::loadApplicationFont(":/font/iconfont.ttf");
  if (fontFamily.isEmpty()) {
    talos::Logger::Warn("Failed to load icon font!");
    fontFamily = utils::FontHelper::getDefaultFallbackFont();
  }

  _iconfont = QFont(fontFamily, 16);

  // initMenu();

  this->setLayout(_mainLayout);

  // initTitle();
  // _mainLayout->setAlignment(Qt::AlignTop);
  // this->setLayout(_mainLayout);
}
