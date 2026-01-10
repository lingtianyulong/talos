import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Effects
import "./components"

Item {
    id: root
    width: 520
    height: 480
    signal closeRequested
    signal loginRequested(string username, string password)

    FontLoader {
        id: iconFont
        source: "qrc:/font/iconfont.ttf"
    }

    // 整个窗口透明度
    opacity: 0.0

    // ===== 背景 =====
    Rectangle {
        anchors.fill: parent
        radius: 12
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "#6A7AEF"
            }
            GradientStop {
                position: 1.0
                color: "#7B5CB8"
            }
        }
    }

    Button {
        id: closeButton
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 16
        implicitWidth: 32
        implicitHeight: 32
        width: implicitWidth
        height: implicitHeight
        font.family: iconFont.name
        font.pixelSize: 14
        text: "\ue624"
        hoverEnabled: true
        onClicked: root.fadeOut()
        background: Rectangle {
            anchors.fill: parent
            color: closeButton.hovered ? Qt.rgba(1, 1, 1, 0.4) : "transparent"
            radius: width / 2
            border.color: "white"
            border.width: 1
        }
        contentItem: Text {
            anchors.fill: parent
            text: closeButton.text
            font: closeButton.font
            color: "#ffffff"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    // ===== 卡片 =====
    Rectangle {
        width: 420
        height: 360
        radius: 10
        color: "white"
        anchors.centerIn: parent

        // 阴影
        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowBlur: 0.35
            shadowColor: "#40000000"
            shadowVerticalOffset: 8
        }

        Column {
            anchors.centerIn: parent
            width: parent.width - 80
            spacing: 16

            Text {
                width: parent.width
                text: "用户登录"
                font.pixelSize: 20
                font.bold: true
                color: "#333333"
                horizontalAlignment: Text.AlignHCenter
            }

            InputUser {
                id: usernameField
                _width: parent.width
                _focus: true
            }

            InputPassword {
                id: passwordField
                _width: parent.width
            }

            Button {
                id: loginButton
                width: 200
                height: 50
                text: "登录"
                anchors.horizontalCenter: parent.horizontalCenter
                hoverEnabled: true

                contentItem: Text {
                    text: loginButton.text
                    font: loginButton.font
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                onClicked: root.loginRequested(usernameField._text, passwordField._text)

                background: Rectangle {
                    color: {
                        if (loginButton.pressed) {
                            return "#3a8ee6";
                        } else if (loginButton.hovered) {
                            return "#66b1ff";
                        } else {
                            return "#409eff";
                        }
                    }
                    radius: 5
                    border.width: 0
                }
            }
        }
    }

    // ===== 淡入 =====
    function fadeIn() {
        fadeInAnim.start();
    }

    // ===== 淡出 =====
    function fadeOut() {
        fadeOutAnim.start();
    }

    PropertyAnimation {
        id: fadeInAnim
        target: root
        property: "opacity"
        from: 0
        to: 1
        duration: 300
        easing.type: Easing.OutCubic
    }

    PropertyAnimation {
        id: fadeOutAnim
        target: root
        property: "opacity"
        from: 1
        to: 0
        duration: 300
        easing.type: Easing.InCubic
        onStopped: root.closeRequested()
    }
}
