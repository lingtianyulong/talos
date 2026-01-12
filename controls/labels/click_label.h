//
// Created by lingt on 2025/12/25.
//

#ifndef TALOS_CLICK_LABEL_H
#define TALOS_CLICK_LABEL_H
#include <QLabel>

namespace controls::labels {
    class ClickLabel : public QLabel {
        Q_OBJECT

    public:
        explicit ClickLabel(QWidget *parent = nullptr);

        explicit ClickLabel(const QString &text, QWidget *parent = nullptr);

        ~ClickLabel() override;

    signals:
        void clicked();

    protected:
        void mousePressEvent(QMouseEvent *event) override;
    };
}

#endif //TALOS_CLICK_LABEL_H
