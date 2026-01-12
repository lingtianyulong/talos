//
// Created by lingt on 2025/11/25.
//

#include "warning_button.h"

namespace controls::buttons {
    WarningButton::WarningButton(QWidget *parent) : QPushButton(parent) {
        this->setProperty("btnType", "warning");
    }

    WarningButton::WarningButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
        this->setProperty("btnType", "warning");
    }

    WarningButton::~WarningButton() {

    }
}