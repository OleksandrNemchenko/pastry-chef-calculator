import QtQuick 2.0

Rectangle {
    property alias text: headerText.text

    width: parent.width
    height: common.dp(50)

    Rectangle {
        anchors.fill: parent

        color: common.titleBackground

        Text {
            id: headerText

            anchors.fill: parent

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: common.titleFontBold
            font.pointSize: common.titleFontPointSize
            color: common.titleColor

        }   // Text

    }   // Rectangle
}   // Rectangle