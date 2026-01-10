//
// Created by lingt on 2025/5/27.
//

#ifndef FONT_HELPER_H
#define FONT_HELPER_H

#include <QString>
#include <QFont>

namespace utils {

class FontHelper {
public:

    /**
     * @brief 加载应用程序字体
     * @param fontPath 字体文件路径（支持资源路径）
     * @return 字体家族名称，如果加载失败返回空字符串
     */
    static QString loadApplicationFont(const QString& fontPath);

    /**
     * @brief 创建图标字体
     * @param fontFamily 字体家族名称
     * @param size 字体大小
     * @return QFont 对象
     */
    static QFont createIconFont(const QString& fontFamily, int size = 16);

    /**
     * @brief 检查字体是否已加载
     * @param fontFamily 字体家族名称
     * @return 如果字体已加载返回 true，否则返回 false
     */
    static bool isFontLoaded(const QString& fontFamily);

    /**
     * @brief 获取默认后备字体
     * @return 默认字体家族名称
     */
    static QString getDefaultFallbackFont();

    /**
     *
     * @param fontPath 字体文件存储路径
     * @param fontSize 字体尺寸
     * @return 所需要的字体
     */
    static QFont getFont(const QString& fontPath, int fontSize);

private:
    FontHelper() = default;
    ~FontHelper() = default;
};

} // namespace Utils

#endif //FONT_HELPER_H
