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
#include "DockManager.h"
#include "DockWidget.h"
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

  // 初始化默认几何尺寸
  _normalGeometry = QRect(0, 0, 1024, 768);

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

  initTitle();
  initUI();

  // _view = new ImageViewer(this);
  // _mainLayout->addWidget(_view);
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
    if (isInTitleArea(pos)) {
      _isDragging = true;
      _pressPos = event->pos(); // 记录本地点击位置
    }
  }
  QWidget::mousePressEvent(event);
}

void MainWidget::mouseMoveEvent(QMouseEvent *event) {
  if (_isDragging && (event->buttons() & Qt::LeftButton)) {
    if (_isMaximized) {
      // 1. 计算点击位置在当前宽度中的比例，以便还原后窗口能准确“粘”在鼠标上
      qreal ratio = (qreal)_pressPos.x() / width();

      // 2. 执行还原
      showNormal();
      _isMaximized = false;

      // 3. 根据比例计算还原后的新位置
      int newX = event->globalPosition().toPoint().x() -
                 (_normalGeometry.width() * ratio);
      int newY = event->globalPosition().toPoint().y() - _pressPos.y();

      // 4. 瞬间移动到新位置，实现无缝衔接
      this->move(newX, newY);
    }

    // 5. 调用系统底层拖动，支持靠边缩放等原生特性
    if (this->windowHandle()) {
      this->windowHandle()->startSystemMove();
    }
    _isDragging = false; // 交给系统后，清除自己的标记
  }
  QWidget::mouseMoveEvent(event);
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (_isDragging && event->button() == Qt::LeftButton) {
    _isDragging = false;
  }
  QWidget::mouseReleaseEvent(event);
}

void MainWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  QPoint pos = event->globalPosition().toPoint();
  if (event->button() == Qt::LeftButton && isInTitleArea(pos)) {
    if (_isMaximized) {
      this->showNormal();
      _isMaximized = false;
      this->setGeometry(_normalGeometry);

      // 恢复后的窗口移动到鼠标附近
      QPoint newPos = QPoint(pos.x() - width() / 2, 30 / 2);
      this->move(newPos);

    } else {
      _normalGeometry = this->geometry();
      this->showMaximized();
      _isMaximized = true;
    }
  }
  QWidget::mouseDoubleClickEvent(event);
}

void MainWidget::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QRect rect = this->rect();
  rect.adjust(0, 0, 0, 0);

  QPainterPath path;
  path.addRoundedRect(rect, 5, 5);
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
                           "border-top-right-radius: 5px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: red;"
                           "border-top-right-radius: 5px;"
                           "}");
  _titleLayout->addWidget(close_btn);

  connect(close_btn, &QPushButton::clicked, this, &MainWidget::closeClicked);
  connect(min_btn, &QPushButton::clicked, this, &MainWidget::minClicked);
  connect(max_btn, &QPushButton::clicked, this, &MainWidget::maxClicked);

  _mainLayout->addLayout(_titleLayout);
}

void MainWidget::initUI() {
  ads::CDockManager *manager = new ads::CDockManager(this);

  // 更加彻底的透明样式设置，并隐藏关闭按钮和其他控制按钮
  manager->setStyleSheet(
      "ads--CDockManager, ads--CDockContainerWidget, ads--CDockAreaWidget, "
      "ads--CDockWidget {"
      "  background: transparent;"
      "  background-color: transparent;"
      "  border: none;"
      "}"
      "ads--CDockAreaTabBar {"
      "  background: transparent;"
      "  background-color: transparent;"
      "}"
      "ads--CDockWidgetTab {"
      "  background: rgba(255, 255, 255, 30);"
      "  color: snow;"
      "  border-radius: 3px;"
      "  margin: 2px;"
      "  padding: 4px 10px;"
      "}"
      "ads--CDockWidgetTab[active=\"true\"] {"
      "  background: rgba(255, 255, 255, 80);"
      "  font-weight: bold;"
      "}"
      "ads--CDockAreaTitleBar {"
      "  background: rgba(0, 0, 0, 30);"
      "  border-bottom: 1px solid rgba(255, 255, 255, 10);"
      "}"
      "ads--CDockAreaTitleBar QLabel {"
      "  color: snow;"
      "}"
      "QSplitter::handle {"
      "  background: rgba(255, 255, 255, 15);"
      "}"
      "/* 隐藏所有关闭、菜单、浮动按钮 */"
      "ads--CDockAreaTitleBar QPushButton,"
      "ads--CDockWidgetTab QPushButton {"
      "  background: transparent;"
      "  border: none;"
      "  width: 0px;"
      "  height: 0px;"
      "  qproperty-icon: url();"
      "  qproperty-text: \"\";"
      "}"
      "/* 针对停靠窗内部的滚动区域等可能存在的白底部件进行穿透 */"
      "QScrollArea, QScrollArea > QWidget > QWidget {"
      "  background: transparent;"
      "  background-color: transparent;"
      "  border: none;"
      "}");

  // 创建中心窗口
  ads::CDockWidget *centralDock = new ads::CDockWidget(" ", manager);
  centralDock->setFeatures(ads::CDockWidget::NoDockWidgetFeatures);
  QWidget *centralWidget = new QWidget();
  centralWidget->setObjectName("centralContentWidget");
  centralWidget->setStyleSheet(
      "#centralContentWidget { background: transparent; }");
  centralDock->setWidget(centralWidget);
  manager->setCentralWidget(centralDock);

  // 添加其他停靠窗口
  ads::CDockWidget *propDock = new ads::CDockWidget("属性", manager);
  // 禁用关闭和浮动功能
  propDock->setFeature(ads::CDockWidget::DockWidgetClosable, false);
  propDock->setFeature(ads::CDockWidget::DockWidgetFloatable, false);
  manager->addDockWidget(ads::RightDockWidgetArea, propDock);

  ads::CDockWidget *toolDock = new ads::CDockWidget("工具", manager);
  // 禁用关闭和浮动功能
  // toolDock->setFeature(ads::CDockWidget::DockWidgetClosable, false);
  // toolDock->setFeature(ads::CDockWidget::DockWidgetFloatable, false);
  manager->addDockWidget(ads::LeftDockWidgetArea, toolDock);
  toolDock->setFeatures(ads::CDockWidget::DockWidgetClosable |
                        ads::CDockWidget::DockWidgetMovable |
                        ads::CDockWidget::DockWidgetFloatable);

  _mainLayout->addWidget(manager);
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
    // 先恢复正常窗口, 再将 geometry 设置为正常状态下的几何矩形
    this->showNormal();
    _isMaximized = false;
    this->setGeometry(_normalGeometry);
  } else {
    // 最大化时, 先保存当前窗口的几何矩形, 再最大化窗口
    _normalGeometry = this->geometry();
    this->showMaximized();
    _isMaximized = true;
  }
}

void MainWidget::minClicked() { this->showMinimized(); }