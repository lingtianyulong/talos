import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Effects
import "./components"

Item {
    id: root
    width: 520
    height: 480

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

            FlatTextField {
                _objectName: "user_input"
                _width: parent.width
                _placeholderText: "请输入用户名"
                _focus: true
            }

            FlatTextField {
                _objectName: "pwd_input"
                _width: parent.width
                _placeholderText: "请输入密码"
                _focus: true
                echoMode: TextInput.Password
            }

            Button {
                id: loginButton
                width: 200
                height: 50
                text: "登录"
                anchors.horizontalCenter: parent.horizontalCenter
                contentItem: Text {
                    text: loginButton.text
                    font: loginButton.font
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    color: "#409eff"
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
        onStopped: Qt.quit() // 通知关闭窗口
    }
}
