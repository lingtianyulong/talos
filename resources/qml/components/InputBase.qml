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
    property string _icon: ""
    property string _buttonIcon: ""     // 功能性按钮图标，如：清除输入、显示密码
    property bool _focus: false
    property string _text: ""
    property string _placeholderText: ""

    property alias _inputText: inputField // 暴露内部控件

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        radius: 5
        border.color: "#cccccc"
        border.width: 1

        Text {
            id: iconText
            text: root._icon
            font.family: iconFont.name
            font.pixelSize: 18
            color: "#666666"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 10
            visible: true
        }

        FlatTextField {
            id: inputField
            anchors.left: iconText.right
            anchors.leftMargin: 10
            _width: parent.width - iconText.width - btn.width - 20
            _placeholderText: root._placeholderText
            _focus: root._focus
            text: root._text
            onTextChanged: {
                root._text = text;
            }
        }

        Button {
            id: btn
            text: root._buttonIcon
            width: 40
            height: parent ? parent.height : implicitHeight

            font.family: iconFont.name
            font.pixelSize: 18

            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            visible: true
            hoverEnabled: true

            background: Rectangle {
                color: btn.hovered ? "#eaeaea" : "transparent"
                radius: 0
                topRightRadius: 5
                bottomRightRadius: 5
                border.color: "#cccccc"
                border.width: 0
            }
        }
    }
}
