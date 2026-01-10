pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15

TextField {
    id: flat_input

    property string _objectName: ""
    property string _placeholderText: ""
    property real _width: -1
    property bool _focus: false
    property bool _cursorBlinkOn: true

    objectName: _objectName
    width: _width >= 0 ? _width : (parent ? parent.width : implicitWidth)
    height: 40
    placeholderText: _placeholderText
    placeholderTextColor: "#cccccc"
    font.pixelSize: 14
    color: "#333333"
    verticalAlignment: TextInput.AlignVCenter
    horizontalAlignment: TextInput.AlignLeft
    leftPadding: 4
    rightPadding: 4
    echoMode: TextInput.Normal
    inputMethodHints: Qt.ImhNone
    focus: _focus

    // 自定义光标
    cursorDelegate: Rectangle {
        width: 2
        color: "#666666"
        visible: flat_input.cursorVisible && flat_input._cursorBlinkOn
        height: flat_input.cursorRectangle.height
        x: flat_input.cursorRectangle.x
        y: flat_input.cursorRectangle.y
    }

    Timer {
        id: cursorBlinkTimer
        interval: 500
        repeat: true
        running: flat_input.activeFocus
        onTriggered: flat_input._cursorBlinkOn = !flat_input._cursorBlinkOn
    }

    onActiveFocusChanged: {
        if (flat_input.activeFocus) {
            _cursorBlinkOn = true;
            cursorBlinkTimer.start();
        } else {
            cursorBlinkTimer.stop();
            _cursorBlinkOn = false;
        }
    }

    background: Rectangle {
        color: "transparent"
        radius: 5
        border.color: "#cccccc"
        border.width: 1
    }
}
