//
// Created by lingt on 2025/11/25.
//

#ifndef TALOS_SUCCESS_BUTTON_H
#define TALOS_SUCCESS_BUTTON_H

#include <QPushButton>

/**
 * 扁平化风格按钮
 */
namespace controls::buttons {
    class SuccessButton final : public QPushButton {
        Q_OBJECT

    public:
        explicit SuccessButton(QWidget *parent = nullptr);
        explicit SuccessButton(const QString &text, QWidget *parent = nullptr);
        ~SuccessButton() override;
    };
}

#endif //TALOS_SUCCESS_BUTTON_H