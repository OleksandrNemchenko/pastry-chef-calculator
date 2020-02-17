
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Rectangle {
    property int margin: common.dp(5)

    id: dbStatusView
    x: margin
    y: header.y + margin
    width: application.width - 2*margin
    height: application.height - 2*margin
    radius: margin
    z: 2

    Text {
        id: dbStatusViewText
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.bold: true
        font.pointSize: 20
        wrapMode: Text.WordWrap
    }

    function showDbState(hasDbError, lastDbError)
    {
        if (!hasDbError) {
            visible = false
            dbStatusViewText.visible = false
        } else {
            color = "#C0400000"
            visible = true

            dbStatusViewText.color = "#FF8080"
            dbStatusViewText.visible = true
            dbStatusViewText.text = qsTr("Ошибка запуска приложения:\n") + lastDbError
        }
    }
}
