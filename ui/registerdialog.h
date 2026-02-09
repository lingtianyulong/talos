//
// Created by lingt on 2025/7/26.
//

#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include "../../controls/inputs/input_password.h"
#include "../../controls/inputs/input_user.h"

#include <QDialog>

using RegisterUserInput = controls::inputs::InputUser;
using RegisterPasswordInput = controls::inputs::InputPassword;

QT_BEGIN_NAMESPACE

namespace Ui {
class registerdialog;
}

QT_END_NAMESPACE

class RegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget* parent = nullptr, int flag = 0);

    ~RegisterDialog() override;

    void setUserName(const QString& userName);

private:
    void init();
    void registerClicked();

    QPointer<RegisterUserInput> _userInput = new RegisterUserInput(this);
    QPointer<RegisterPasswordInput> _passwordInput = new RegisterPasswordInput(this);

    Ui::registerdialog* ui;

    int _flag = 0;  //!< 0 注册用户, 1 修改密码
};

#endif  // REGISTERDIALOG_H
