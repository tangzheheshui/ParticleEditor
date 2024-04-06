import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
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
        id: groupBox1
        x: 824
        y: 140
        width: 200
        height: 410
        title: qsTr("属性")
    }

    GroupBox {
        id: groupBox2
        x: 5
        y: 599
        width: 998
        height: 150
        spacing: -16
        font.bold: true
        font.family: "Arial"
        title: qsTr("log")
    }

    GLWidget {
        id: gLWidget
        x: 204
        y: 130
        width: 614
        height: 455
    }

    Text {
        id: text1
        x: 32
        y: 18
        width: 146
        height: 45
        text: qsTr("Text")
        font.pixelSize: 27
    }

    GroupBox {
        id: groupBox
        x: 19
        y: 108
        width: 143
        height: 408
        padding: 0
        title: qsTr("场景节点")

        ListView {
            id: listView
            x: 12
            y: 31
            width: 106
            height: 265
            model: ListModel {
                ListElement {
                    name: "Grey"
                    colorCode: "grey"
                }

                ListElement {
                    name: "Red"
                    colorCode: "red"
                }

                ListElement {
                    name: "Blue"
                    colorCode: "blue"
                }

                ListElement {
                    name: "Green"
                    colorCode: "green"
                }
            }
            delegate: Item {
                x: 5
                width: 80
                height: 40
                Row {
                    id: row1
                    spacing: 10
                    Rectangle {
                        width: 40
                        height: 40
                        color: colorCode
                    }

                    Text {
                        text: name
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: true
                    }
                }
            }
        }

        Button {
            id: btnAdd
            x: 12
            y: 356
            width: 32
            height: 34
            text: qsTr("+")
            font.pointSize: 30
            onClicked: {
                inputDialog.open()
            }

            Dialog {
                id: inputDialog
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                parent: Overlay.overlay
                focus: true
                modal: true
                title: qsTr("Input")
                standardButtons: Dialog.Ok | Dialog.Cancel
                onAccepted: {
                    console.log(objName.text, objType.currentIndex);
                    myObject.updateMessage(objName.text);
                }
                ColumnLayout {
                    anchors.fill: parent
                    Label {
                        elide: Label.ElideRight
                        text: qsTr("name:")
                        Layout.fillWidth: true
                    }
                    TextField {
                        id: objName
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        focus: true
                        placeholderText: qsTr("Username")
                        Layout.fillWidth: true
                    }
                    Label {
                        elide: Label.ElideRight
                        font.kerning: true
                        text: qsTr("type:")
                        Layout.fillWidth: true
                    }
                    ComboBox {
                        id: objType
                        x: 30
                        y: 30
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        model: ListModel {
                            ListElement { text: "纹理矩形";}
                            ListElement { text: "3D模型";}
                        }
                    }
                }
            }
        }

        Button {
            id: btnMinus
            x: 98
            y: 357
            width: 32
            height: 34
            text: qsTr("-")
            font.pointSize: 30
            rotation: 0
        }
    }
}
