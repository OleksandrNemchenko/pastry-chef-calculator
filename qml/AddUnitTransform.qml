
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Dialog {
    visible: false
    title: qsTr("Добавление нового преобразования")

    GridLayout {
        columns: 3

        Text      { text: qsTr("1-ая величина:") }
        TextInput { id: unitFromValue; text: "1" }
        Text      { text: abbreviation }

        Text      { text: qsTr("2-ая величина:") }
        TextInput { id: unitToValue; text: "1" }
        ComboBox {
            id: unitTo
            textRole: "title"
            validator: DoubleValidator {
                bottom: 0
            }
            model: PCCUnits.unitTransformsToSelection(idUnit)
        }
    }

    standardButtons: StandardButton.Ok | StandardButton.Cancel

    onAccepted: {
        PCCUnits.unitTransformAdd(idUnit, unitTo.model[unitTo.currentIndex].unitId, unitFromValue.text, unitToValue.text)
        unitTransformModel.update(idUnit)
    }
}
