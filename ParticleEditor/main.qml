import QtQuick 2.15
import QtQuick.Controls 2.15
import CustomOpenGL 1.0

Item {
    width: 1024
    height: 768

    Rectangle {
        id: rectangle
        x: 5
        y: 18
        width: 586
        height: 68
        color: "#ffffff"
    }

    GroupBox {
        id: groupBox
        x: 5
        y: 140
        width: 200
        height: 390
        visible: true
        spacing: -16
        title: qsTr("Group Box")
    }

    GroupBox {
        id: groupBox1
        x: 824
        y: 140
        width: 200
        height: 390
        title: qsTr("Group Box")
        spacing: -16
    }

    GroupBox {
        id: groupBox2
        x: 5
        y: 599
        width: 998
        height: 150
        title: qsTr("Group Box")
        spacing: -16
    }

    GLWidget {
        id: gLWidget
        x: 204
        y: 130
        width: 612
        height: 455
    }

    Text {
        id: text1
        x: 243
        y: 46
        width: 146
        height: 45
        text: qsTr("Text")
        font.pixelSize: 27
    }
}
