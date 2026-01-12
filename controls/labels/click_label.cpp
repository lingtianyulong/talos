//
// Created by lingt on 2025/12/25.
//

#include "click_label.h"

#include <qevent.h>

namespace controls::labels {
    ClickLabel::ClickLabel(QWidget *parent) : QLabel(parent) {

    }

    ClickLabel::ClickLabel(const QString &text, QWidget *parent): QLabel(text, parent) {

    }

    ClickLabel::~ClickLabel() {

    }

    void ClickLabel::mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }
        QLabel::mousePressEvent(event);
    }

}
