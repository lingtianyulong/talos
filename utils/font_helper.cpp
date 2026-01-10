//
// Created by lingt on 2025/5/27.
//

#include "font_helper.h"
#include <QFontDatabase>
#include <QDebug>

namespace utils {

QString FontHelper::loadApplicationFont(const QString& fontPath) {
    int fontId = QFontDatabase::addApplicationFont(fontPath);
    if (fontId == -1) {
        qWarning() << "Failed to load font from path:" << fontPath;
        return QString();
    }

    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    qDebug() << "Loaded font families from" << fontPath << ":" << fontFamilies;

    if (!fontFamilies.isEmpty()) {
        QString fontFamily = fontFamilies.at(0);
        qDebug() << "Using font family:" << fontFamily;
        return fontFamily;
    }
    qWarning() << "No font families found for loaded font:" << fontPath;
    return QString();
}

QFont FontHelper::createIconFont(const QString& fontFamily, int size) {
    QFont iconFont(fontFamily, size);
    return iconFont;
}

bool FontHelper::isFontLoaded(const QString& fontFamily) {
    // QFontDatabase fontDb;
    // QStringList families = fontDb.families();
    QStringList families = QFontDatabase::families();
    return families.contains(fontFamily);
}

QString FontHelper::getDefaultFallbackFont() {
    return "Arial";
}

QFont FontHelper::getFont(const QString& fontPath, int fontSize) {
    QString fontFamily = loadApplicationFont(fontPath);
    if (fontFamily.isEmpty()) {
        qWarning() << "Failed to load font from path:" << fontPath;
        fontFamily = getDefaultFallbackFont();
    }

    return QFont(fontFamily, fontSize);
}


} // namespace Utils
