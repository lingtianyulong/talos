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
    property bool _sanitizingInput: false

    function stripChineseCharacters(value) {
        if (!value || value.length === 0) {
            return "";
        }
        // 过滤常见的中日韩字符及全角符号，避免输入中文
        return value.replace(/[\u2E80-\u9FFF\uF900-\uFAFF\uFF00-\uFFEF]/g, "");
    }

    InputBase {
        id: inputPassword
        _width: parent.width
        _icon: "\ue640"
        _buttonIcon: {
            if (_inputField.echoMode === TextInput.Password) {
                return "\ue669";
            } else {
                return "\ue60f";
            }
        }
        _placeholderText: "请输入密码"
        _focus: parent._focus
        _text: parent._text
        _inputField.echoMode: TextInput.Password
        _inputField.onTextChanged: {
            if (!parent._sanitizingInput) {
                const sanitizedText = parent.stripChineseCharacters(_text);
                if (sanitizedText !== _text) {
                    parent._sanitizingInput = true;
                    const removedCount = _text.length - sanitizedText.length;
                    const targetCursor = Math.max(0, _inputField.cursorPosition - removedCount);
                    _inputField.text = sanitizedText;
                    _inputField.cursorPosition = targetCursor;
                    parent._sanitizingInput = false;
                    return;
                }
            }
            if (_text.length > 0) {
                _button.visible = true;
            } else {
                _button.visible = false;
            }
            parent._text = _text;
        }
        _button.onClicked: {
            if (_inputField.echoMode === TextInput.Password) {
                _inputField.echoMode = TextInput.Normal;
            } else {
                _inputField.echoMode = TextInput.Password;
            }
        }
    }
}
