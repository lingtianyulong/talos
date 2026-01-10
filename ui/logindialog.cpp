#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMetaObject>
#include <QQuickItem>
#include <QQuickWidget>
#include <QUrl>
#include <QVBoxLayout>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoginDialog) {
  ui->setupUi(this);

  // 无边框窗口
  setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

  // 允许透明（必须）
  setAttribute(Qt::WA_TranslucentBackground);

  // 固定大小（与 QML 一致）
  setFixedSize(520, 480);

  // QQuickWidget
  _quick = new QQuickWidget(this);
  _quick->setResizeMode(QQuickWidget::SizeRootObjectToView);
  _quick->setClearColor(Qt::transparent);
  _quick->setSource(QUrl("qrc:/qml/login.qml"));

  // 布局
  auto *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(_quick);
  setLayout(layout);
}

LoginDialog::~LoginDialog() { delete ui; }

void LoginDialog::showEvent(QShowEvent *event) {
  QDialog::showEvent(event);
  if (auto *obj = _quick->rootObject()) {
    QMetaObject::invokeMethod(obj, "fadeIn");
  }
}

void LoginDialog::closeEvent(QCloseEvent *event) {
  QDialog::closeEvent(event);
  if (auto *obj = _quick->rootObject()) {
    QMetaObject::invokeMethod(obj, "fadeOut");
  }
}