//
// Created by lingt on 2025/12/11.
//

#include "graphicsview.h"

#include <qevent.h>
#include <QPointer>

namespace controls::image_viewer {
    GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent) {
        // 关闭垂直和水平滚动条
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        setScene(new QGraphicsScene(this));

        // 设置鼠标追踪
        setMouseTracking(true);
        // 禁止 Qt 自动填充背景，让它调用 drawBackground()
        setBackgroundBrush(Qt::NoBrush);
        // 保证背景始终重绘（非常重要）
        setViewportUpdateMode(FullViewportUpdate);
    }

    GraphicsView::~GraphicsView() {
    }

    void GraphicsView::paintEvent(QPaintEvent *event) {
        QPainter painter(viewport());
        painter.save();

        drawMyBackground(&painter, viewport()->rect());
        if (!_pixmap.isNull()) {
            // 绘制 pixmap 效率是 drawImage 的 10 倍
            painter.drawPixmap(_dstRect, _pixmap, _srcRect);
            if (_curRatio > _showLineRatio) {
                // 绘制像素点辅助线
                drawPixelDivider(&painter);
            }
        }

        painter.restore();
        QGraphicsView::paintEvent(event);
    }

    void GraphicsView::wheelEvent(QWheelEvent *event) {
        if (_pixmap.isNull()) {
            return;
        }

        auto pos = mapPosToImage(event->position());
        if (!isPosInImage(pos)) {
            return;
        }

        auto delta = event->pixelDelta();
        if (delta.isNull()) {
            delta = event->angleDelta();
        }

        if (delta.y() > 0) {
            zoomByAnchor(ZoomType::ZoomIn, event->position());
        } else if (delta.y() < 0) {
            zoomByAnchor(ZoomType::ZoomOut, event->position());
        }
        // 缩放比例
        emit zoomChanged(_curRatio);
        viewport()->update();
        QGraphicsView::wheelEvent(event);
    }

    void GraphicsView::mousePressEvent(QMouseEvent *event) {
    }

    void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
        if (_pixmap.isNull()) {
            return;
        }
        auto pos = mapPosToImage(event->position());
        if (!isPosInImage(pos)) {
            emit coorChanged(QPointF(0, 0));
            emit colorChanged(QColor(0, 0, 0));
            return;
        }

        emit coorChanged(pos);
        QColor color = imageColor(pos);
        emit colorChanged(color);

        QGraphicsView::mouseMoveEvent(event);
    }

    void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    }

    void GraphicsView::resizeEvent(QResizeEvent *event) {
    }

    void GraphicsView::drawMyBackground(QPainter *painter, const QRectF &rect) {
        if (painter == nullptr) {
            return;
        }
        painter->fillRect(rect, _brush1);
        for (int i = 0; i < rect.height(); i += _blockSize) {
            // 隔行错位
            int offset = (i / _blockSize % 2 == 0) ? 0 : _blockSize;
            for (int j = 0; j < rect.width(); j += _blockSize << 1) {
                painter->fillRect(QRectF(j + offset, i, _blockSize, _blockSize), _brush2);
            }
        }
    }

    void GraphicsView::setPixmap(const QPixmap &img) {
        if (img.isNull()) {
            return;
        }
        _pixmap = img.copy();
        _srcRect = QRectF(0, 0, img.width(), img.height());
        fitViewport();
        emit zoomChanged(_curRatio);
        viewport()->update();
    }

    /**
     * 函数介绍：图像自适应当前视图区域的大小
     * 输入参数：viewRect,当前视图区的尺寸
     * 输出参数：无
     * 返回值：无
     */
    void GraphicsView::fitViewport() {
        // 如果图像为空,则直接返回
        if (_pixmap.isNull()) {
            return;
        };
        auto viewRect = geometry();

        // 视图区与图像的宽度比
        qreal wRatio = viewRect.width() / static_cast<qreal>(_pixmap.width());
        // 视图区与图像的高度比
        qreal hRatio = viewRect.height() / static_cast<qreal>(_pixmap.height());

        if (wRatio > hRatio) {
            _curRatio = hRatio;

            qreal height = _pixmap.height() * hRatio;
            qreal width = _pixmap.width() * hRatio;
            qreal x = abs((width - viewRect.width()) / 2);

            // 矩形框的值的设置,有顺序要求？
            _dstRect = QRectF(x, 0, width, height);
        } else {
            _curRatio = wRatio;
            qreal width = _pixmap.width() * wRatio;
            qreal height = _pixmap.height() * wRatio;
            qreal y = abs((height - viewRect.height()) / 2);

            _dstRect = QRectF(0, y, width, height);
        }
    }

    std::optional<QImage> GraphicsView::image() const {
        if (_pixmap.isNull()) {
            return _pixmap.toImage();
        }
        return std::nullopt;
    }

    bool GraphicsView::isPosInImage(const QPointF &pos) const {
        return !_pixmap.isNull() && pos.x() >= 0 && pos.x() <= _pixmap.width() && pos.y() >= 0 && pos.y() <= _pixmap.
               height();
    }

    /**
     * 函数介绍：将视图区(客户区)的坐标,映射到图像上
     * 输入参数：鼠标在视图区(客户区)的当前坐标
     * 输出参数：无
     * 返回值：图像上当前点的坐标
     */
    QPointF GraphicsView::mapPosToImage(const QPointF &viewPoint) {
        QPointF imgPoint;
        qreal x = (viewPoint.x() - _dstRect.x()) / _curRatio;
        qreal y = (viewPoint.y() - _dstRect.y()) / _curRatio;

        imgPoint.setX(x);
        imgPoint.setY(y);

        return imgPoint;
    }

    /**
     * 函数介绍：将图像上的点映射到视图区(客户区)
     * 输入参数：imgPoint,鼠标在图像中的当前坐标
     * 输出参数：无
     * 返回值：视图区(客户区)的坐标
     */
    QPointF GraphicsView::mapPosToViewport(const QPointF &imgPoint) {
        QPointF viewPoint;
        viewPoint.setX(imgPoint.x() * _curRatio + _dstRect.left());
        viewPoint.setY(imgPoint.y() * _curRatio + _dstRect.top());
        return viewPoint;
    }

    void GraphicsView::zoomIn() {
        if (_pixmap.isNull()) {
            return;
        }

        if (_curRatio * _scaleFactor < _maxRatio) {
            _curRatio *= _scaleFactor;
        } else {
            _curRatio = _maxRatio;
        }

        qreal x = (_dstRect.width() - _pixmap.width() * _curRatio) / 2;
        qreal y = (_dstRect.height() - _pixmap.height() * _curRatio) / 2;
        qreal width = _pixmap.width() * _curRatio;
        qreal height = _pixmap.height() * _curRatio;
        _dstRect = QRectF(x, y, width, height);
    }
    /**
     * 函数介绍：在当前视图区域范围内,将图像缩小一级
     * 输入参数：viewRect,当前视图区域范围
     * 输出参数：无
     * 返回值：无
     */
    void GraphicsView::zoomOut() {
        if (_pixmap.isNull()) {
            return;
        }

        if (_curRatio / _scaleFactor > _minRatio) {
            _curRatio /= _scaleFactor;
        } else {
            _curRatio = _minRatio;
        }

        qreal x = (_dstRect.width() - _pixmap.width() * _curRatio) / 2;
        qreal y = (_dstRect.height() - _pixmap.height() * _curRatio) / 2;
        qreal width = _pixmap.width() * _curRatio;
        qreal height = _pixmap.height() * _curRatio;
        _dstRect = QRectF(x, y, width, height);
    }

    /**
     * 函数介绍：以当前鼠标点所在的点为锚点,在视图区域范围内,对图像进行缩放操作
     * 输入参数：zoomType,图像的缩放类型
     *                  ZoomIn, 放大图像
     *                  ZoomOut, 缩小图像
     *          viewPoint, 锚点坐标
     * 输出参数：无
     * 返回值：无
     */
    void GraphicsView::zoomByAnchor(ZoomType zoomType, const QPointF &anchor) {
        // 根据锚点在视图区的位置,图像的目标视窗和当前放大倍率,推算点在原图中的位置
        QPointF ptInImage(0, 0); // 点在图像中的位置
        ptInImage.setX((anchor.x() - _dstRect.x()) / _curRatio);
        ptInImage.setY((anchor.y() - _dstRect.y()) / _curRatio);

        // 放大图像
        if (zoomType == ZoomType::ZoomIn) {
            if (_curRatio * _scaleFactor < _maxRatio) {
                _curRatio *= _scaleFactor;
            } else {
                _curRatio = _maxRatio;
            }
        } else if (zoomType == ZoomType::ZoomOut) {
            if (_curRatio / _scaleFactor > _minRatio) {
                _curRatio /= _scaleFactor;
            } else {
                _curRatio = _minRatio;
            }
        }

        // 放大倍率改变后,调整目标视窗大小
        _dstRect.setX((anchor.x() - ptInImage.x() * _curRatio));
        _dstRect.setY((anchor.y() - ptInImage.y() * _curRatio));
        _dstRect.setWidth(_pixmap.width() * _curRatio);
        _dstRect.setHeight(_pixmap.height() * _curRatio);
    }

    /**
     * 函数介绍：以单个像素为单位,绘制像素分割线
     * 输入参数：painter
     * 输出参数：无
     * 返回值：无
     */
    void GraphicsView::drawPixelDivider(QPainter *painter) {
        // 将分割线颜色设置为黄色
        painter->setPen(Qt::yellow);

        painter->drawRect(_dstRect); // 画边框

        // QRect 中 x,y,h,w 与 top,left,right,bottom 含义不同

        // 按行扫描
        for (qreal row = _dstRect.top(); row < _dstRect.bottom(); row += _curRatio) {
            QPointF pt1(_dstRect.left(), row);
            QPointF pt2(_dstRect.right(), row);
            painter->drawLine(pt1, pt2);
        }

        // 按列扫描
        for (qreal col = _dstRect.left(); col < _dstRect.right(); col += _curRatio) {
            QPointF pt1(col, _dstRect.top());
            QPointF pt2(col, _dstRect.bottom());
            painter->drawLine(pt1, pt2);
        }
    }

    /**
     * 获取鼠标所在位置(点)的图像的颜色值
     * @param pt
     * @return QColor 图像上的像素点的颜色值
     */
    QColor GraphicsView::imageColor(const QPointF &pt) const {
        if (_pixmap.isNull()) {
            return QColor(0, 0, 0);
        }

        if (!isPosInImage(pt)) {
            return QColor(0, 0, 0);
        }
        // 转 QImage（注意，这一步不会有明显性能开销，因为 Qt 6 内部用共享数据）
        QImage img = _pixmap.toImage();

        // 获取像素点 (x, y) 的颜色
        QRgb rgb = img.pixel(pt.x(), pt.y());
        QColor color = QColor::fromRgb(rgb);
        return color;
    }
}
