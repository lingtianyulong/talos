//
// Created by lingt on 2025/11/25.
//
#include "default_button.h"

namespace controls::buttons {
    DefaultButton::DefaultButton(QWidget *parent) : QPushButton(parent) {
        this->setProperty("btnType", "default");
    }

    DefaultButton::DefaultButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
        this->setProperty("btnType", "default");
    }

    DefaultButton::~DefaultButton() {

    }
}