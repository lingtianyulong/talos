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
#include <QLabel>
#include <QLinearGradient>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPushButton>
#include <QScreen>
#include <QTimer>
#include <QWindow>

#include "../logger/logger.h"
#include "../utils/font_helper.h"
#include "capture/rust/capture_rust.h"
#include "controls/buttons/default_button.h"
#include "controls/messagebox/messagebox.h"

using namespace controls::buttons;
using namespace controls::image_viewer;
using namespace controls::messagebox;
using namespace talos::screen_capture;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWidget) {
  ui->setupUi(this);

  setWindowIcon(QIcon(":/icons/icon.ico"));
  setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
  setAttribute(Qt::WA_TranslucentBackground);
  setFixedSize(1024, 768);

  initUI();
  initTitle();
}

MainWidget::~MainWidget() { delete ui; }

bool MainWidget::isInTitleArea(const QPoint &pos) const {
  QRect rect;
  rect.setTopLeft(this->geometry().topLeft());
  rect.setBottomRight(
      QPoint(this->geometry().right(), this->geometry().top() + 30));
  return rect.contains(pos);
}

void MainWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {

    QPoint pos = event->globalPosition().toPoint();
    // 将窗口可拖动区域设置在标题栏区域
    if (isInTitleArea(pos)) {
      this->windowHandle()->startSystemMove();
    }
  }

  QWidget::mousePressEvent(event);
}

void MainWidget::mouseMoveEvent(QMouseEvent *event) {

  QWidget::mouseMoveEvent(event);
}

void MainWidget::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QRect rect = this->rect();
  rect.adjust(0, 0, 0, 0);

  QPainterPath path;
  path.addRoundedRect(rect, 12, 12);
  painter.fillPath(path, QBrush(QColor(106, 122, 239, 255)));
}

/**
 * @brief 创建 title 栏的按钮
 * @param title 按钮文本, utf8 字符串, 在 iconfont 中的字体符号
 * @return 返回 QPushbutton 若创建失败,则返回 nullptr
 */
QPushButton *MainWidget::createTitleButton(const QString &title) {
  QPointer btn = new QPushButton(this);
  btn->setFont(_iconfont);
  btn->setText(title);
  btn->setFixedSize(40, 30);
  btn->setStyleSheet("QPushButton {"
                     "background-color: transparent;"
                     "color: snow;"
                     "font-size: 12px;"
                     "border: none;"
                     "}"
                     "QPushButton:hover {"
                     "background-color: rgb(126, 142, 255);"
                     "}"
                     "QPushButton:pressed {"
                     "background-color: rgb(96, 112, 239);"
                     "}");
  return btn;
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

  if (_titleLayout == nullptr) {
    _titleLayout = new QHBoxLayout(this); // 移除 this 以避免顶层布局冲突
  }

  _titleLayout->setSpacing(0);
  _titleLayout->setContentsMargins(10, 0, 0, 0); // 左边缩进 10px
  _titleLayout->setAlignment(Qt::AlignTop | Qt::AlignVCenter);

  // icon 图像
  auto iconLabel = new QLabel(this);
  iconLabel->setFixedSize(16, 16);
  iconLabel->setScaledContents(true);
  iconLabel->setPixmap(windowIcon().pixmap(16, 16));
  iconLabel->setAlignment(Qt::AlignVCenter);
  _titleLayout->addWidget(iconLabel); // 正确添加到布局左侧

  _titleLayout->addSpacing(5);

  auto titleLabel = new QLabel(this);
  titleLabel->setText("Talos");
  titleLabel->setFixedSize(100, 30);
  titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  titleLabel->setStyleSheet("QLabel {"
                            "color: snow;"
                            "font-size: 14px;"
                            "}");
  _titleLayout->addWidget(titleLabel);

  _titleLayout->addStretch();

  auto min_btn = createTitleButton(QString::fromUtf8("\ue67a"));
  auto max_btn = createTitleButton(QString::fromUtf8("\ue653"));
  _titleLayout->addWidget(min_btn);
  _titleLayout->addWidget(max_btn);

  auto close_btn = createTitleButton(QString::fromUtf8("\ue624"));
  close_btn->setStyleSheet("QPushButton {"
                           "background-color: transparent;"
                           "color: snow;"
                           "font-size: 12px;"
                           "border: none;"
                           "border-top-right-radius: 12px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: red;"
                           "border-top-right-radius: 12px;"
                           "}");
  _titleLayout->addWidget(close_btn);

  connect(close_btn, &QPushButton::clicked, this, &MainWidget::closeClicked);
  connect(min_btn, &QPushButton::clicked, this, &MainWidget::minClicked);
  connect(max_btn, &QPushButton::clicked, this, &MainWidget::maxClicked);

  _mainLayout->addLayout(_titleLayout);
}

void MainWidget::initUI() {
  QString fontFamily =
      utils::FontHelper::loadApplicationFont(":/font/iconfont.ttf");
  if (fontFamily.isEmpty()) {
    talos::Logger::Warn("Failed to load icon font!");
    fontFamily = utils::FontHelper::getDefaultFallbackFont();
  }

  _iconfont = QFont(fontFamily, 14);

  if (_mainLayout == nullptr) {
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->setSpacing(0);
  }

  this->setLayout(_mainLayout);
}

void MainWidget::closeClicked() {
  auto ret = MessageBox::warning("确定要退出程序吗?", this);
  if (ret == MessageBox::StandardButton::Ok) {
    this->close();
  }
}

/**
 * @brief 最大化/正常化窗口, 并保存正常状态下的几何矩形
 * @note 如果窗口已经最大化, 则恢复到正常状态, 并保存正常状态下的几何矩形
 *       如果窗口不是最大化, 则最大化窗口, 并保存当前窗口的几何矩形
 *       由自己维护状态, 避免窗口状态丢失
 */
void MainWidget::maxClicked() {
  if (_isMaximized) {
    this->showNormal();
    _isMaximized = false;
    this->setGeometry(_normalGeometry);
  } else {
    this->showMaximized();
    _isMaximized = true;
    _normalGeometry = this->geometry();
  }
}

void MainWidget::minClicked() { this->showMinimized(); }