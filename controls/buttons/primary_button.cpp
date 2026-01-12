//
// Created by lingt on 2025/11/25.
//
#include "primary_button.h"

namespace controls::buttons {
    PrimaryButton::PrimaryButton(QWidget *parent) : QPushButton(parent) {
        this->setProperty("btnType", "primary");
    }

    PrimaryButton::PrimaryButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
        this->setProperty("btnType", "primary");
    }

    PrimaryButton::~PrimaryButton() {

    }
}