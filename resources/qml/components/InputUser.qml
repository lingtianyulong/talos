pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root
    width: _width >= 0 ? _width : (parent ? parent.width : implicitWidth)
    height: 40

    // 加载 iconfont 字体
    FontLoader {
        id: iconFont
        source: "qrc:/font/iconfont.ttf"
    }

    property real _width: -1
    property bool _focus: false
    property string _text: ""

    InputBase {
        id: inputUser
        _width: parent.width
        _icon: "\ue606"
        _buttonIcon: "\ue7e9"
        _placeholderText: "请输入用户名"
        _focus: parent._focus
        _text: parent._text
        _inputField.onTextChanged: {
            _button.visible = true;
            parent._text = _text;
        }
        _button.onClicked: {
            _inputField.text = "";
            _button.visible = false;
        }
    }
}
