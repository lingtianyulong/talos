//
// Created by lingt on 2025/12/9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MessageBox.h" resolved

#include "messagebox.h"
#include "ui_MessageBox.h"
#include <QHBoxLayout>

using namespace controls::buttons;

namespace controls::messagebox {
    MessageBox::MessageBox(QWidget *parent) : QDialog(parent), ui(new Ui::MessageBox) {
        ui->setupUi(this);
        // 设置图标
        this->setWindowIcon(QIcon(":/icons/icon.ico"));
        this->setFixedSize(400, 150);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, Qt::white);
        this->setAutoFillBackground(true);
        this->setPalette(palette);

        setWindowFlag(Qt::Dialog);

        init();

        setButtons(StandardButton::NoButton);
    }

    MessageBox::~MessageBox() {
        delete ui;
    }

    void MessageBox::init() {
        QPointer mainLayout = new QVBoxLayout(this);

        QPointer textLayout = new QHBoxLayout(this);
        textLayout->setContentsMargins(10, 10, 10, 10);
        textLayout->addWidget(_label);

        QPointer btnLayout = new QHBoxLayout(this);
        _confirmBtn->setFixedSize(100, 35);
        _cancleBtn->setFixedSize(100, 35);

        connect(_confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
        connect(_cancleBtn, &QPushButton::clicked, this, &QDialog::reject);

        btnLayout->setAlignment(Qt::AlignRight);
        btnLayout->addWidget(_cancleBtn);
        btnLayout->addSpacing(10);
        btnLayout->addWidget(_confirmBtn);
        btnLayout->setContentsMargins(50, 0, 10, 5);

        mainLayout->addLayout(textLayout);
        mainLayout->addStretch();
        mainLayout->addLayout(btnLayout);
        this->setLayout(mainLayout);
    }

    void MessageBox::setText(const QString &text) {
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(12);
        _label->setFont(font);
        _label->setWordWrap(true);
        _label->setText(text);
        _label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        _label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);      //!< 允许高度随内容变化
        _label->setMaximumWidth(350);       //!< 设置最大宽度
    }

    void MessageBox::setButtons(StandardButtons button) {
        _confirmBtn->setVisible(button.testFlag(StandardButton::Ok));
        _cancleBtn->setVisible(button.testFlag(StandardButton::Cancel));
    }


    void MessageBox::setType(Type type) {
        switch (type) {
            case Type::Information:
                this->setWindowTitle(tr("提示"));
                setButtons(StandardButton::Ok);
                break;
            case Type::Warning:
                this->setWindowTitle(tr("警告"));
                setButtons(StandardButton::Ok | StandardButton::Cancel);
                break;
                case Type::Error:
                this->setWindowTitle(tr("错误"));
                setButtons(StandardButton::Ok);
                break;
            case Type::Question:
                setWindowTitle(tr("确认"));
                setButtons(StandardButton::Ok | StandardButton::Cancel);
                break;
            default:
                break;
        }
    }

    MessageBox::StandardButton MessageBox::message(Type type, const QString &text, QWidget *parent) {
        auto box = new MessageBox(parent);
        box->setText(text);
        box->setType(type);
        box->setAttribute(Qt::WA_DeleteOnClose);
        auto ret = box->exec();
        return static_cast<StandardButton>(ret);
    }

    MessageBox::StandardButton MessageBox::information(const QString &text, QWidget *parent) {
        return message(Type::Information, text, parent);
    }

    MessageBox::StandardButton MessageBox::warning(const QString &text, QWidget *parent) {
        return message(Type::Warning, text, parent);
    }

    MessageBox::StandardButton MessageBox::error(const QString &text, QWidget *parent) {
        return message(Type::Error, text, parent);
    }

    MessageBox::StandardButton MessageBox::question(const QString &text, QWidget *parent) {
        return message(Type::Question, text, parent);
    }

}
