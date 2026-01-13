//
// Created by lingt on 2025/5/19.
//

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QPushButton>
#include <QTimer>

#include "controls/image_viewer/imageviewer.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWidget;
}

QT_END_NAMESPACE

class MainWidget : public QWidget {
  Q_OBJECT

public:
  explicit MainWidget(QWidget *parent = nullptr);

  ~MainWidget() override;

  void onOpenFile();

  void onStartCapture();

  void onStopCapture();

protected:
  // void paintEvent(QPaintEvent *event) override;

private:
  void initUI();

  void initTitle();

  void initMenu();

  QPushButton *createTitleButton(const QString &title);

  Ui::MainWidget *ui;

  QPointer<QVBoxLayout> _mainLayout = nullptr;
  QPointer<QHBoxLayout> _titleLayout = nullptr;

  QFont _iconfont;

  QPointer<controls::image_viewer::ImageViewer> _view = nullptr;

  QPointer<QTimer> _timer = new QTimer(this);

  bool _captureInited = false; //!< 是否已经初始化抓屏模块
};

#endif // MAINWIDGET_H
