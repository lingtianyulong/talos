import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Item {
    id: root
    width: 520
    height: 480

    // 整个窗口透明度
    // opacity: 0.0

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
            spacing: 16

            Text {
                text: "用户登录"
                font.pixelSize: 20
                font.bold: true
                color: "#333333"
                horizontalAlignment: Text.AlignHCenter
            }

            TextField {
                width: 260
                placeholderText: "用户名"
            }

            TextField {
                width: 260
                placeholderText: "密码"
                echoMode: TextInput.Password
            }

            Button {
                width: 260
                height: 40
                text: "登录"
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
