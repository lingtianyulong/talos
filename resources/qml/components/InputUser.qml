pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root
    width: _width >= 0 ? _width : (parent ? parent.width : implicitWidth)
    height: 40

    FontLoader {
        id: iconFont
        source: "qrc:/font/iconfont.ttf"
    }

    property real _width: -1
    property bool _focus: false

    InputBase {
        id: inputBase
        _width: parent.width
        _icon: "\ue606"
        _buttonIcon: "\ue7e9"
        _placeholderText: "请输入用户名"
        _focus: parent._focus
    }
}
