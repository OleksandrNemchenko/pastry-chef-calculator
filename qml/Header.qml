import QtQuick 2.0
import QtQuick.Layouts 1.5

Rectangle {
    property alias text: header.text

    width: parent.width
    height: header.y + header.height

    Text {
        id: header
        font.bold: true
        font.pointSize: 30
        color: "black"
    }

    Rectangle {
        id: addButton
        width: parent.width
        anchors.top: header.bottom

                Image {
                    anchors.left: parent.left
                    anchors.margins: common.dp(5)
                    source: "pics/Add.svg"
                    sourceSize.width: common.smallButtonSide
                    sourceSize.height: common.smallButtonSide
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {}
                    }
                }

    }
}