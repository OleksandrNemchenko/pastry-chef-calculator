
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

MessageDialog {
    visible: false
    title: qsTr("Удаление единицы")
    text: qsTr("Вы подтверждаете удаление единицы измерения %1 (%2)?"). arg(title). arg(abbreviation)
    icon: StandardIcon.Question
    standardButtons: StandardButton.Yes | StandardButton.No

    onYes: {
        PCCUnits.unitDelete(idUnit)
    }

}
