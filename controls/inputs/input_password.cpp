//
// Created by xiaoqiang on 2025/11/26.
//

#include "input_password.h"
#include "../../utils/font_helper.h"
#include <QRegularExpressionValidator>

namespace controls::inputs {
    InputPassword::InputPassword(QWidget *parent) : InputBase(parent) {
        InputPassword::init();
    }

    InputPassword::~InputPassword() {
    }

    void InputPassword::init() {
        InputBase::init();
        QFont font = utils::FontHelper::getFont(":/fonts/iconfont.ttf", 14);
        if (_label != nullptr) {
            _label->setFont(font);
            _label->setStyleSheet("color: deepskyblue; border: none");
            _label->setText(QString::fromStdString("\ue640"));
        }

        if (_lineEdit != nullptr) {
            _lineEdit->setEchoMode(QLineEdit::Password);
            // 禁止输入中文
            QRegularExpression reg("^[^\u4e00-\u9fa5]*$");
            _lineEdit->setValidator(new QRegularExpressionValidator(reg, this));
        }

        if (_btn != nullptr) {
            _btn->setFont(font);
            _btn->setText(QString::fromStdString("\ue669"));

            // 点击按钮时,清空输入的文本
            connect(_btn, &QPushButton::clicked, this, [this]() {
                auto mode = _lineEdit->echoMode();
                if (mode == QLineEdit::Password) {
                    _lineEdit->setFocus();
                    _lineEdit->setEchoMode(QLineEdit::Normal);
                    _btn->setText(QString::fromStdString("\ue60f"));
                }
                else if (mode == QLineEdit::Normal) {
                    _lineEdit->setFocus();
                    _lineEdit->setEchoMode(QLineEdit::Password);
                    _btn->setText(QString::fromStdString("\ue669"));
                }
            });
        }
    }

    void InputPassword::setText(const QString &text) {
        if (_lineEdit != nullptr) {
            _lineEdit->setText(text);
        }
    }

    QString InputPassword::text() const {
        if (_lineEdit != nullptr) {
            return _lineEdit->text();
        }
        return "";
    }
}
