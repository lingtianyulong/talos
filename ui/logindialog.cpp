#include "logindialog.h"
#include "registerdialog.h"
#include "ui_logindialog.h"
#include <QMetaObject>
#include <QPointer>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWidget>
#include <QScopedValueRollback>
#include <QUrl>
#include <QVBoxLayout>


LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoginDialog) {
  ui->setupUi(this);

  this->setWindowIcon(QIcon(":/icons/icon.ico"));

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
  if (_quick->rootObject()) {
    _quick->rootObject()->setProperty("backend", QVariant::fromValue(this));
  }

  // connectQmlSignals();
  // connect(_quick, &QQuickWidget::statusChanged, this,
  //         [this](QQuickWidget::Status status) {
  //           if (status == QQuickWidget::Ready) {
  //             connectQmlSignals();
  //           }
  //         });

  // 布局
  QPointer layout = new QVBoxLayout(this);
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
  if (_closingFromQml) {
    return;
  }
  if (auto *obj = _quick->rootObject()) {
    QMetaObject::invokeMethod(obj, "fadeOut");
  }
}

void LoginDialog::handleClosed() {
  if (_closingFromQml) {
    return;
  }
  QScopedValueRollback<bool> guard(_closingFromQml, true);
  close();
}

void LoginDialog::handleLogin(const QString &username,
                              const QString &password) {
  _username = username;
  _password = password;
  accept();
}

void LoginDialog::handleRegister() {
  RegisterDialog registerDialog(this, 0);
  registerDialog.exec();
}

void LoginDialog::handleForgetPassword() {
  RegisterDialog registerDialog(this, 1);
  registerDialog.exec();
}

QString LoginDialog::username() const { return _username; }

QString LoginDialog::password() const { return _password; }

void LoginDialog::startDrag(const QPoint &pos) { _dragOffset = pos; }

void LoginDialog::dragWindow(const QPoint &pos) {
  Q_UNUSED(pos);
  move(QCursor::pos() - _dragOffset);
}
