//
// Created by lingt on 2025/11/26.
//

#ifndef TALOS_INPUT_USER_H
#define TALOS_INPUT_USER_H

#include "input_base.h"

namespace controls::inputs {
    class InputUser : public InputBase {
        Q_OBJECT
    public:
        explicit InputUser(QWidget *parent = nullptr);
        ~InputUser() override;
        void init() override;
        void setText(const QString& text) override;
        QString text() const override;
    };
}

#endif //TALOS_INPUT_USER_H