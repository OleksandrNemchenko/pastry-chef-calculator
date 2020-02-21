
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Dialog {
    visible: false
    title: qsTr("Добавление новой единицы измерения")

    GridLayout {
        columns: 2

        Text {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("%1 : добавление новой единицы"). arg(section)
        }

        Text      { text: qsTr("Название:") }
        TextInput { id: unitTitle; text: qsTr("название") }

        Text      { text: qsTr("Аббревиатура:") }
        TextInput { id: unitAbbreviation; text: qsTr("аббревиатура") }
    }

    standardButtons: StandardButton.Ok | StandardButton.Cancel

    onAccepted: {
        PCCUnits.unitAdd(section, unitTitle.text, unitAbbreviation.text)
    }
}
