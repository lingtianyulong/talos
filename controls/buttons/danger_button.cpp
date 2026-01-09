//
// Created by lingt on 2025/11/25.
//

#include "danger_button.h"

namespace controls::buttons {
    DangerButton::DangerButton(QWidget *parent) : QPushButton(parent) {
        this->setProperty("btnType", "danger");
    }

    DangerButton::DangerButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
        this->setProperty("btnType", "danger");
    }

    DangerButton::~DangerButton() {

    }
}