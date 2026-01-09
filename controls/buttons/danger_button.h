//
// Created by lingt on 2025/11/25.
//

#ifndef TALOS_DANGER_BUTTON_H
#define TALOS_DANGER_BUTTON_H

#include <QPushButton>

/**
 * 扁平化风格按钮
 */
namespace controls::buttons {
    class DangerButton final : public QPushButton {
        Q_OBJECT

    public:
        explicit DangerButton(QWidget *parent = nullptr);
        explicit DangerButton(const QString &text, QWidget *parent = nullptr);
        ~DangerButton() override;
    };
}
#endif //TALOS_DANGER_BUTTON_H