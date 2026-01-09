//
// Created by lingt on 2025/11/25.
//

#ifndef TALOS_INFO_BUTTON_H
#define TALOS_INFO_BUTTON_H

#include <QPushButton>

/**
 * 扁平化风格按钮
 */
namespace controls::buttons {
    class InfoButton final : public QPushButton {
        Q_OBJECT

    public:
        explicit InfoButton(QWidget *parent = nullptr);
        explicit InfoButton(const QString &text, QWidget *parent = nullptr);
        ~InfoButton() override;
    };
}

#endif //TALOS_INFO_BUTTON_H