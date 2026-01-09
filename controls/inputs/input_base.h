//
// Created by lingt on 2025/11/26.
//

#ifndef TALOS_INPUT_BASE_H
#define TALOS_INPUT_BASE_H
#include <QLineEdit>
#include <QLabel>
#include <QPointer>
#include <QPushButton>

namespace controls::inputs {
    class InputBase : public QWidget {
        Q_OBJECT

    public:
        explicit InputBase(QWidget *parent = nullptr);

        ~InputBase() override;

        virtual void setText(const QString &text) = 0;
        virtual QString text() const = 0;
        virtual void init();
    protected:
        QPointer<QLabel> _label;            //!< 输入框类型 Label, 如密码输入框,则该 label 显示为🔒
        QPointer<QLineEdit> _lineEdit;      //!< 输入框

        //!< 操作按钮, 若 lineEdit 为普通输入, 则点击按钮, 清除输入内容
        //!< 若 lineEdit 为密码模式,则点击按钮,显示输入按钮
        QPointer<QPushButton> _btn;
    };
}
#endif //TALOS_INPUT_BASE_H