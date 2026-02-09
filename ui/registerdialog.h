//
// Created by lingt on 2025/7/26.
//

#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

// -------------------
// 使用前置声明
// 减少头文件依赖
// 加快编译速度
// 避免循环依赖
// -------------------
namespace controls::inputs {
class InputUser;
class InputPassword;
}  // namespace controls::inputs

#include <QDialog>
#include <QPointer>

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

    QPointer<RegisterUserInput> _userInput = nullptr;
    QPointer<RegisterPasswordInput> _passwordInput = nullptr;

    Ui::registerdialog* ui;

    int _flag = 0;  //!< 0 注册用户, 1 修改密码
};

#endif  // REGISTERDIALOG_H
