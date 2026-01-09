//
// Created by lingt on 2025/12/13.
//

#ifndef TALOS_IMAGEVIEWER_H
#define TALOS_IMAGEVIEWER_H
#include <QLabel>
#include <QWidget>
#include <QPointer>
#include <QVBoxLayout>

#include "graphicsview.h"

namespace controls::image_viewer {
    class ImageViewer : public QWidget {
        Q_OBJECT

    public:
        explicit ImageViewer(QWidget *parent = nullptr);

        ~ImageViewer() override;

        void setImage(const QImage &image);

        void setImage(const QPixmap &image);

    private:
        void initLayout(); //!< 初始化窗口布局

        QPointer<QVBoxLayout> _mainLayout = nullptr;
        QPointer<GraphicsView> _view = nullptr;
        QPointer<QLabel> _coorLabel = nullptr;        //!< 显示坐标
        QPointer<QLabel> _colorLabel = nullptr;       //!< 显示图像颜色值
        QPointer<QLabel> _factorLabel = nullptr;      //!< 缩放比例
    };
}

#endif //TALOS_IMAGEVIEWER_H
