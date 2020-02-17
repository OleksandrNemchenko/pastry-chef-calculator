
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Rectangle {
    anchors.fill: parent

    ListModel {
        id: pricesData

        ListElement {
            text: "мука, 1 кг: 10 грн"
        }

        ListElement {
            text: "яйца, 1 дес.: 64 грн"
        }

        ListElement {
            text: "молоко, 1 л: 25 грн"
        }
    }

    ListView {
        id: view

        anchors.margins: 10
        anchors.fill: parent
        spacing: 5
        model: pricesData

        delegate: Item {
            id: listDelegate

            width: view.width
            height: common.dp(15)
                Text {
                id: text1
                    anchors.left: parent.left
                    renderType: Text.NativeRendering
                    text: model.text
                }
                Image {
                    anchors.right: parent.right
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
