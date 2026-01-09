//
// Created by lingt on 2025/11/25.
//

#ifndef TALOS_WARNING_BUTTON_H
#define TALOS_WARNING_BUTTON_H

#include <QPushButton>

/**
 * 扁平化风格按钮
 */
namespace controls::buttons {
    class WarningButton final : public QPushButton {
        Q_OBJECT

    public:
        explicit WarningButton(QWidget *parent = nullptr);
        explicit WarningButton(const QString &text, QWidget *parent = nullptr);
        ~WarningButton() override;
    };
}


#endif //TALOS_WARNING_BUTTON_H