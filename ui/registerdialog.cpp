//
// Created by lingt on 2025/7/26.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_registerdialog.h" resolved

#include "registerdialog.h"

#include "config/url_config/api_config.h"
#include "controls/buttons/default_button.h"
#include "controls/buttons/primary_button.h"
#include "controls/messagebox/messagebox.h"
#include "db/db_util.h"
#include "encrypt/encrypt_util.h"
#include "logger/logger.h"
#include "models/user.h"
#include "ui_registerdialog.h"
#include "utils/net_helper.h"

#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPointer>
#include <QPushButton>

using namespace talos;
using namespace controls;
using namespace controls::messagebox;

/**
 *
 * @param parent
 * @param flag 0 注册用户, 1 修改用户
 */
RegisterDialog::RegisterDialog(QWidget* parent, int flag)
    : QDialog(parent), ui(new Ui::registerdialog) {
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/icons/icon.ico"));
    _flag = flag;
    if (_flag == 0) {
        this->setWindowTitle("注册用户");
    } else if (_flag == 1) {
        this->setWindowTitle("修改密码");
    }

    // 设置 dialog 的背景色
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(255, 255, 255));  // RGB
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    this->setFixedWidth(400);
    this->setFixedHeight(200);
    init();
}

RegisterDialog::~RegisterDialog() {
    if (ui != nullptr) {
        delete ui;
        ui = nullptr;
    }
}

void RegisterDialog::init() {
    // 主窗口布局
    QPointer mainLayout(new QVBoxLayout(this));
    _userInput->setFixedHeight(40);
    _passwordInput->setFixedHeight(40);

    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(50, 20, 50, 0);
    mainLayout->addWidget(_userInput);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(_passwordInput);

    QPointer btnLayout = new QHBoxLayout;
    QString btnText = "";
    if (_flag == 0) {
        btnText = tr("注册");
    } else if (_flag == 1) {
        btnText = tr("修改密码");
    }
    QPointer primaryBtn = new buttons::PrimaryButton(btnText, this);
    QPointer defaultBtn = new buttons::DefaultButton(tr("取消"), this);

    connect(primaryBtn, &buttons::PrimaryButton::clicked, this, &RegisterDialog::registerClicked);
    connect(defaultBtn, &buttons::DefaultButton::clicked, [this] { close(); });

    primaryBtn->setFixedSize(100, 40);
    defaultBtn->setFixedSize(100, 40);

    btnLayout->setContentsMargins(30, 10, 30, 10);
    btnLayout->addWidget(defaultBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(primaryBtn);
    mainLayout->addStretch();
    mainLayout->addLayout(btnLayout);
}

void RegisterDialog::setUserName(const QString& userName) {
    _userInput->setText(userName);
}

void RegisterDialog::registerClicked() {
    QString username = _userInput->text();
    QString password = _passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        MessageBox::warning(tr("请输入用户名或密码"), this);
        reject();
        return;
    }

    auto encrypt = encrypt::EncryptUtil::encrypt(password.toUtf8().constData());
    models::User user(username.toStdString(), encrypt);

    if (encrypt) {
        encrypt::EncryptUtil::release(const_cast<char*>(encrypt));
    }

    auto json = user.to_json();
    std::string url = std::format("{}{}", url::BASE_URL, url::REGISTER);
    auto resp = utils::http_post(url.c_str(), json.c_str());

    QString response(resp);
    if (resp) {
        utils::free_http_response(resp);
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        QString errorString = QString("Json parse error %1").arg(error.errorString());
        Logger::Error(errorString.toStdString());
        return;
    }

    auto obj = doc.object();
    if (obj.contains("success") && obj["success"].toBool()) {
        MessageBox::information("注册账号成功!", this);
    }
}
