#include "logindialog.h"
#include "config/url_config/api_config.h"
#include "controls/messagebox/messagebox.h"
#include "logger/logger.h"
#include "models/user.h"
#include "registerdialog.h"
#include "ui_logindialog.h"
#include "utils/net_helper.h"
#include "utils/secure_store_helper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include <QMetaObject>
#include <QPointer>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWidget>
#include <QScopedValueRollback>
#include <QUrl>
#include <QVBoxLayout>

using namespace talos;
using namespace controls;
using namespace controls::messagebox;

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
  reject();
  close();
}

void LoginDialog::handleLogin(const QString &username,
                              const QString &password) {

  if (username.isEmpty() || password.isEmpty()) {
    MessageBox::warning("用户名或密码为空, 请选输入用户名或密码!", this);
    reject();
  }

  auto pwd = password.toStdString();
  models::User user(username.toStdString(), pwd);

  std::string login_url = std::format("{}{}", url::BASE_URL, url::LOGIN);
  auto resp = utils::http_post(login_url.c_str(), user.to_json().c_str(), "");
  if (!resp) {
    accept();
    return;
  }

  QString response(resp);
  utils::free_http_response(resp);

  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8(), &error);
  if (error.error != QJsonParseError::NoError) {
    QString errorString = QString("Json parse error: %1").arg(error.error);
    Logger::Error(errorString.toStdString());
    reject();
  }

  auto obj = doc.object();
  if (obj.contains("success") && obj["success"].toBool() == true) {
    auto token = obj["token"].toString();
    if (!token.isEmpty()) {
      if (utils::secure_store_set("talos", "access_token",
                                  token.toUtf8().constData()) != 0) {
        Logger::Error("Store token failed.");
      }
    }
  } else {
    auto msg = obj["message"].toString();
    MessageBox::warning(msg, this);
  }
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

void LoginDialog::moveWindow() { this->windowHandle()->startSystemMove(); }
