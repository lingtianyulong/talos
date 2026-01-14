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
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
  void initUI();
  void initTitle();
  void initMenu();
  bool isInTitleArea(const QPoint &pos) const;

  void closeClicked();
  void maxClicked();
  void minClicked();

  QPushButton *createTitleButton(const QString &title);

  Ui::MainWidget *ui;

  QPointer<QVBoxLayout> _mainLayout = nullptr;
  QPointer<QHBoxLayout> _titleLayout = nullptr;

  QFont _iconfont;

  QPointer<controls::image_viewer::ImageViewer> _view = nullptr;

  QPointer<QTimer> _timer = new QTimer(this);

  bool _captureInited = false; //!< 是否已经初始化抓屏模块
  bool _isMaximized = false;   //!< 是否最大化
  QRect _normalGeometry;       //!< 正常状态下的几何矩形
  bool _isDragging = false;    //!< 是否正在拖拽
  QPoint _pressPos;            //!< 鼠标按下时的位置
};

#endif // MAINWIDGET_H
