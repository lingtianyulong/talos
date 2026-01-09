//
// Created by lingt on 2025/11/26.
//

#include "input_base.h"

#include <QHBoxLayout>

namespace controls::inputs {
    InputBase::InputBase(QWidget *parent)
        : QWidget(parent)
          , _label(new QLabel(this))
          , _lineEdit(new QLineEdit(this))
          , _btn(new QPushButton(this)) {
        setAttribute(Qt::WA_StyledBackground, true);
    }

    InputBase::~InputBase() {
        if (_label != nullptr) {
            delete _label;
            _label = nullptr;
        }

        if (_lineEdit != nullptr) {
            delete _lineEdit;
            _lineEdit = nullptr;
        }

        if (_btn != nullptr) {
            delete _btn;
            _btn = nullptr;
        }
    }

    void InputBase::init() {
        if (_label == nullptr) {
            _label = new QLabel(this);
        }

        if (_lineEdit == nullptr) {
            _lineEdit = new QLineEdit(this);
        }

        if (_btn == nullptr) {
            _btn = new QPushButton(this);
        }

        _label->setStyleSheet(
            "QLabel {"
            "background-color: transparent;"
            "border: none;"
            "font-size: 14px;"
            "height: 40px;"
            "width: 40px;"
            "}");
        _label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);

        _lineEdit->setStyleSheet(
            "QLineEdit {"
            "background-color: transparent;"
            "border: none;"
            "font-size: 14px;"
            "height: 40px;"
            "}");

        // 只有编辑时,才显示清空按钮
        connect(_lineEdit, &QLineEdit::textEdited, this, [this](const QString &text) {
            if (!text.isEmpty()) {
                _btn->setVisible(true);
            } else {
                _btn->setVisible(false);
            }
        });

        _btn->setStyleSheet(
            "QPushButton {"
            "background-color: transparent;"
            "border: none;"
            "font-size: 20px;"
            "width: 40px;"
            "height: 40px;"
            "color: #d8d8d8;"
            "}"
            "QPushButton:hover {"
            "background: #eaeaea;"
            "border-color: #c6c6c6;"
            "}");

        _btn->setVisible(false);

        QPointer layout = new QHBoxLayout(this);
        layout->setContentsMargins(5, 0, 0, 0);
        layout->addWidget(_label);
        layout->addWidget(_lineEdit);
        layout->addWidget(_btn);

        this->setStyleSheet(
            "QWidget{"
            "background-color: transparent;"
            "border: 1px solid #d8d8d8;"
            "border-radius: 5px;"
            "}");
        this->setLayout(layout);
    }
}
