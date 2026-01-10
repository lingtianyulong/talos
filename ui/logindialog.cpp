#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMetaObject>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWidget>
#include <QScopedValueRollback>
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
  connectQmlSignals();
  connect(_quick, &QQuickWidget::statusChanged, this,
          [this](QQuickWidget::Status status) {
            if (status == QQuickWidget::Ready) {
              connectQmlSignals();
            }
          });

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
  if (_closingFromQml) {
    return;
  }
  if (auto *obj = _quick->rootObject()) {
    QMetaObject::invokeMethod(obj, "fadeOut");
  }
}

void LoginDialog::connectQmlSignals() {
  if (!_quick) {
    return;
  }
  if (auto *obj = _quick->rootObject()) {
    QObject::connect(obj, SIGNAL(closeRequested()), this,
                     SLOT(handleCloseRequested()), Qt::UniqueConnection);
    QObject::connect(obj, SIGNAL(loginRequested(QString, QString)), this,
                     SLOT(handleLoginRequested(QString, QString)),
                     Qt::UniqueConnection);
  }
}

void LoginDialog::handleCloseRequested() {
  if (_closingFromQml) {
    return;
  }
  QScopedValueRollback<bool> guard(_closingFromQml, true);
  close();
}

void LoginDialog::handleLoginRequested(const QString &username,
                                       const QString &password) {
  _username = username;
  _password = password;
  //   emit credentialsSubmitted(_username, _password);
}

QString LoginDialog::username() const { return _username; }

QString LoginDialog::password() const { return _password; }
