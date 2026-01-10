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
  explicit LoginDialog(QWidget *parent = nullptr);
  ~LoginDialog();
  QString username() const;
  QString password() const;

protected:
  void showEvent(QShowEvent *event) override;
  void closeEvent(QCloseEvent *event) override;

private:
  void connectQmlSignals();

private slots:
  void handleCloseRequested();
  void handleLoginRequested(const QString &username, const QString &password);

private:
  Ui::LoginDialog *ui;
  QQuickWidget *_quick = nullptr;
  bool _closingFromQml = false;
  QString _username;
  QString _password;
};

#endif // LOGINDIALOG_H
