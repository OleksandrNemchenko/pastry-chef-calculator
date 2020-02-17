
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Rectangle {
    anchors.fill: parent

    ListModel {
        id: receiptsListData

        ListElement {
            title: "яблочный пирог"
        }

        ListElement {
            title: "шоколадный торт"
        }

        ListElement {
            title: "Наполеон"
        }
    }

    ComboBox {
        id: receiptsList
        anchors.margins: 10
        anchors.top: parent.top
        width: parent.width
        height: common.dp(30)
        model: receiptsListData
    }

    ListModel {
        id: receiptComponentsListData
        ListElement {
            component: "корж"
            element: "мука"
            amount: "10 кг"
        }

        ListElement {
            component: "корж"
            element: "молоко"
            amount: "100 мл"
        }

        ListElement {
            component: "крем"
            element: "сливки"
            amount: "10 мл"
        }
    }

    ListView {

        anchors.top: receiptsList.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 10
        clip: true
        model: receiptComponentsListData

        section.property: "component"
        section.delegate: Rectangle {
            width: parent.width
            height: common.dp(20)

                Text {
                    id: componentName
                    anchors.left: parent.left
                    renderType: Text.NativeRendering
                    font.bold: true
                    text: "корж"
                }

                Image {
                    id: deleteButton
                    anchors.right: parent.right
                    anchors.margins: common.dp(5)
                    source: "pics/Delete.svg"
                    sourceSize.width: common.smallButtonSide
                    sourceSize.height: common.smallButtonSide
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {}
                    }
                }

                Image {
                    id: editButton
                    anchors.right: deleteButton.left
                    anchors.margins: common.dp(5)
                    source: "pics/Edit.svg"
                    sourceSize.width: common.smallButtonSide
                    sourceSize.height: common.smallButtonSide
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {}
                    }
                }
        }

                delegate: Rectangle {
                    width: parent.width
                    height: common.dp(20)
                    x: parent.x + common.dp(10)

                        Text {
                            id: componentName
                            anchors.left: parent.left
                            renderType: Text.NativeRendering
                    text: element + ", " + amount
                        }

                        Image {
                            id: deleteButton
                            anchors.right: parent.right
                            anchors.margins: common.dp(5)
                            source: "pics/Delete.svg"
                            sourceSize.width: common.smallButtonSide
                            sourceSize.height: common.smallButtonSide
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {}
                            }
                        }

                        Image {
                            id: editButton
                            anchors.right: deleteButton.left
                            anchors.margins: common.dp(5)
                            source: "pics/Edit.svg"
                            sourceSize.width: common.smallButtonSide
                            sourceSize.height: common.smallButtonSide
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {}
                            }
                        }

    }
}
}
