pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

TextField {
    id: flat_input

    property string _objectName: ""
    property string _placeholderText: ""
    property real _width: -1
    property bool _focus: false
    property bool _cursorBlinkOn: true

    objectName: _objectName
    width: _width >= 0 ? _width : (parent ? parent.width : implicitWidth)
    height: parent ? parent.height : implicitHeight
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

    // ======== 自定义光标 ========
    cursorDelegate: Rectangle {
        width: 2
        color: "#666666"
        visible: flat_input.cursorVisible && flat_input._cursorBlinkOn
        height: flat_input.cursorRectangle.height
        x: flat_input.cursorRectangle.x
        y: flat_input.cursorRectangle.y
    }

    // ======== 光标闪烁 ========
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

    // ======== 背景扁平化 ========
    background: Rectangle {
        color: "transparent"
        radius: 0
        border.color: "#cccccc"
        border.width: 0
    }
}
