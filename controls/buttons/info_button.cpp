//
// Created by lingt on 2025/11/25.
//

#include "info_button.h"

namespace controls::buttons {
    InfoButton::InfoButton(QWidget *parent) : QPushButton(parent) {
        this->setProperty("btnType", "info");
    }

    InfoButton::InfoButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
        this->setProperty("btnType", "info");
    }

    InfoButton::~InfoButton() {

    }
}