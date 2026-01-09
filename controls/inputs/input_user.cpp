//
// Created by lingt on 2025/11/26.
//

#include "input_user.h"

#include "../../utils/font_helper.h"

namespace controls::inputs {
    InputUser::InputUser(QWidget *parent) : InputBase(parent) {
        InputUser::init();
    }

    InputUser::~InputUser() {

    }

    void InputUser::init() {
        InputBase::init();
        QFont font = utils::FontHelper::getFont(":/fonts/iconfont.ttf", 14);
        if (_label != nullptr) {
            _label->setFont(font);
            _label->setStyleSheet("color: deepskyblue; border: none");
            _label->setText(QString::fromStdString("\ue606"));
        }

        if (_btn != nullptr) {
            _btn->setFont(font);
            _btn->setText(QString::fromStdString("\ue7e9"));

            // 点击按钮时,清空输入的文本
            connect(_btn, &QPushButton::clicked, this, [this]() {
                _lineEdit->clear();
                _lineEdit->setFocus();
                _btn->setVisible(false);
            });
        }

    }

    void InputUser::setText(const QString& text) {
        if (_lineEdit != nullptr) {
            _lineEdit->setText(text);
        }
    }

    QString InputUser::text() const {
        if (_lineEdit != nullptr) {
            return _lineEdit->text();
        }
        return "";
    }


}
