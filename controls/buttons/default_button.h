//
// Created by lingt on 2025/11/25.
//

#ifndef TALOS_DEFAULT_BUTTON_H
#define TALOS_DEFAULT_BUTTON_H

#include <QPushButton>

/**
 * 扁平化风格按钮
 */
namespace controls::buttons {
    class DefaultButton final : public QPushButton {
        Q_OBJECT

    public:
        explicit DefaultButton(QWidget *parent = nullptr);
        explicit DefaultButton(const QString &text, QWidget *parent = nullptr);
        ~DefaultButton() override;
    };
}

#endif //TALOS_DEFAULT_BUTTON_H