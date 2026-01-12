//
// Created by xiaoqiang on 2025/11/26.
//

#ifndef TALOS_INPUT_PASSWORD_H
#define TALOS_INPUT_PASSWORD_H

#include "input_base.h"

namespace controls::inputs {
    class InputPassword : public InputBase {
        Q_OBJECT

    public:
        explicit InputPassword(QWidget *parent = nullptr);
        ~InputPassword() override;
        void init() override;
        void setText(const QString& text) override;
        QString text() const override;
    };
}

#endif //TALOS_INPUT_PASSWORD_H
