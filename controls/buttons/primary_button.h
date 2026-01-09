//
// Created by lingt on 2025/11/25.
//

#ifndef TALOS_PRIMARY_BUTTON_H
#define TALOS_PRIMARY_BUTTON_H

#include <QPushButton>

/**
 * 扁平化风格按钮
 */
namespace controls::buttons {
    class PrimaryButton final : public QPushButton {
        Q_OBJECT

    public:
        explicit PrimaryButton(QWidget *parent = nullptr);
        explicit PrimaryButton(const QString &text, QWidget *parent = nullptr);
        ~PrimaryButton() override;
    };
}


#endif //TALOS_PRIMARY_BUTTON_H