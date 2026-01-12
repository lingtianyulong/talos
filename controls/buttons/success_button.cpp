//
// Created by lingt on 2025/11/25.
//
#include "success_button.h"

namespace controls::buttons {
    SuccessButton::SuccessButton(QWidget *parent) : QPushButton(parent) {
        this->setProperty("btnType", "success");
    }

    SuccessButton::SuccessButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
        this->setProperty("btnType", "success");
    }

    SuccessButton::~SuccessButton() {

    }
}