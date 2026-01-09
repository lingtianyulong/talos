//
// Created by lingt on 2025/12/13.
//

#include "imageviewer.h"

namespace controls::image_viewer {
    ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent) {
        initLayout();
    }

    ImageViewer::~ImageViewer() {
    }

    void ImageViewer::initLayout() {
        if (_mainLayout == nullptr) {
            _mainLayout = new QVBoxLayout(this);
        }

        if (_view == nullptr) {
            _view = new GraphicsView(this);
        }
        _mainLayout->addWidget(_view);

        // 初始化状态栏布局
        QPointer layout = new QHBoxLayout(this);
        if (_coorLabel == nullptr) {
            _coorLabel = new QLabel("(x,y)", this);
        }
        if (_colorLabel == nullptr) {
            _colorLabel = new QLabel("(r, g, b)", this);
        }
        if (_factorLabel == nullptr) {
            _factorLabel = new QLabel("100%", this);
        }

        layout->addWidget(_coorLabel);
        layout->setSpacing(5);
        layout->addWidget(_colorLabel);
        layout->setSpacing(5);
        layout->addWidget(_factorLabel);
        layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        _mainLayout->addLayout(layout);
        this->setLayout(_mainLayout);

        connect(_view, &GraphicsView::coorChanged, [this](const QPointF &pos) {
            auto p = pos.toPoint();
            QString info = QString("(x:%1, y:%2)").arg(p.x()).arg(p.y());
            _coorLabel->setText(info);
        });

        connect(_view, &GraphicsView::colorChanged, [this](const QColor &color) {
            QString info = QString("(r: %1, g: %2, b: %3)").arg(color.red()).arg(color.green()).arg(color.blue());
            _colorLabel->setText(info);
        });

        connect(_view, &GraphicsView::zoomChanged, [this](qreal factor) {
            qreal f = factor * 100.0;
            QString info = "缩放比例:" + QString::number(f, 'f', 2) + "%";
            _factorLabel->setText(info);
        });
    }

    void ImageViewer::setImage(const QImage &image) {
        if (_view != nullptr) {
            _view->setPixmap(QPixmap::fromImage(image));
        }
    }

    void ImageViewer::setImage(const QPixmap &image) {
        if (_view != nullptr) {
            _view->setPixmap(image);
        }
    }
}
