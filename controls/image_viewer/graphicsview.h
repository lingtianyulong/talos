//
// Created by lingt on 2025/12/11.
//

#ifndef TALOS_GRAPHICSVIEW_H
#define TALOS_GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QProperty>

namespace controls::image_viewer {
class GraphicsView : public QGraphicsView {
  Q_OBJECT

public:
  explicit GraphicsView(QWidget *parent);

  ~GraphicsView() override;

signals:
  void coorChanged(const QPointF &pos); //!< 坐标

  void colorChanged(const QColor &color); //!< 颜色

  void zoomChanged(qreal factor); //!< 缩放比例

protected:
  // 绘图事件
  void paintEvent(QPaintEvent *event) override;

  // 鼠标事件
  void wheelEvent(QWheelEvent *event) override;

  void mousePressEvent(QMouseEvent *event) override;

  void mouseMoveEvent(QMouseEvent *event) override;

  void mouseReleaseEvent(QMouseEvent *event) override;

  // 尺寸改变事件
  void resizeEvent(QResizeEvent *event) override;

public:
  enum class ZoomType {
    ZoomIn,
    ZoomOut,
  };

  void setPixmap(const QPixmap &img);

  void fitViewport(); //!< 图片自适应视图区

  std::optional<QImage> image() const; //!< 返回 QImage

  QColor imageColor(const QPointF &pt) const;

private:
  bool isPosInImage(const QPointF &pos) const; //!< 判断鼠标位置是否在图像范围内

  QPointF
  mapPosToImage(const QPointF &viewPoint); //!< 将视图区的坐标映射到图像上

  QPointF
  mapPosToViewport(const QPointF &imgPoint); //!< 将图像上的坐标,映射回视图区

  void zoomByAnchor(ZoomType zoomType, const QPointF &anchor);

  void zoomIn(); //!< 放大图像

  void zoomOut(); //!< 缩小图像

  // 绘制背景
  void drawMyBackground(QPainter *painter, const QRectF &rect);

  void drawPixelDivider(QPainter *painter); //!< 绘制辅助线

  QBrush _brush1 = QBrush(Qt::gray);                //!< 背景画刷
  QBrush _brush2 = QBrush(QColor(105, 105, 105));   //!< 背景画刷
  int _blockSize = 20;                              //!< 背景方格尺
  QBrush _backBrush = QBrush(QColor(88, 101, 216)); //!< 背景画刷

  QPixmap _pixmap;
  QRectF _srcRect; //!< 源图像视图区
  QRectF _dstRect; //!< 目标图像视图区(缩放、平移等操作后的结果图像)
  float _curRatio; //!< 当前缩放比例

  constexpr static qreal _maxRatio = 150.0;   //!< 最大缩放比例
  constexpr static qreal _minRatio = 0.1;     //!< 最小缩放比例
  constexpr static qreal _scaleFactor = 1.25; //!< 单次缩放系数
  constexpr static qreal _showLineRatio = 30.0; //!< 显示像素辅助线的缩放比例
};
} // namespace controls::image_viewer

#endif // TALOS_GRAPHICSVIEW_H
