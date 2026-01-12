//
// Created by lingt on 2025/12/9.
//

#ifndef TALOS_MESSAGEBOX_H
#define TALOS_MESSAGEBOX_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QPointer>
#include "../buttons/default_button.h"
#include "../buttons/primary_button.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class MessageBox;
}

QT_END_NAMESPACE

namespace controls::messagebox {
    using PrimaryButton = buttons::PrimaryButton;
    using DefaultButton = buttons::DefaultButton;

    class MessageBox : public QDialog {
        Q_OBJECT

    public:
        enum class Type {
            Information,
            Warning,
            Error,
            Question
        };

        enum class StandardButton : int {
            NoButton = 0x0,
            Ok = 0x1,
            Cancel = 0x2,
        };

        Q_DECLARE_FLAGS(StandardButtons, StandardButton)

        explicit MessageBox(QWidget *parent = nullptr);

        ~MessageBox() override;

        void setText(const QString &text);

        void setType(Type type);

        void setButtons(StandardButtons button);

        static StandardButton information(const QString &text, QWidget *parent = nullptr);

        static StandardButton warning(const QString &text, QWidget *parent = nullptr);

        static StandardButton error(const QString &text, QWidget *parent = nullptr);

        static StandardButton question(const QString &text, QWidget *parent = nullptr);

    private:
        void init();

        static StandardButton message(Type type, const QString &text, QWidget *parent = nullptr);

        Ui::MessageBox *ui;
        QPointer<QLabel> _label = new QLabel(this);

        QPointer<PrimaryButton> _confirmBtn = new PrimaryButton(tr("确定"), this);
        QPointer<DefaultButton> _cancleBtn = new DefaultButton(tr("取消"), this);
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(MessageBox::StandardButtons)
}

#endif //TALOS_MESSAGEBOX_H
