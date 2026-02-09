#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QQuickWidget>
#include <QString>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog();

public slots:
    void handleClosed();
    void handleLogin(const QString& username, const QString& password);
    void handleRegister();
    void handleForgetPassword();
    void moveWindow();

protected:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::LoginDialog* ui;
    QQuickWidget* _quick = nullptr;
    bool _closingFromQml = false;
    QString _username;
    QString _password;
    QPoint _dragOffset;
};

#endif  // LOGINDIALOG_H
