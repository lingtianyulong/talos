pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

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
    property string _text: ""

    InputBase {
        id: inputBase
        _width: parent.width
        _icon: "\ue640"
        _buttonIcon: "\ue669"
        _placeholderText: "请输入密码"
        _focus: parent._focus
    }
}
