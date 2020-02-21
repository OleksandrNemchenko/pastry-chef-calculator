
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

MessageDialog {
    visible: false
    title: qsTr("Удаление преобразования единиц")
    text: qsTr("Вы подтверждаете удаление преобразования?\n\"%1\""). arg(description)
    icon: StandardIcon.Question
    standardButtons: StandardButton.Yes | StandardButton.No

    onYes: {
        PCCUnits.unitTransformDelete(idUnit, idUnitTransform)
        unitTransformModel.clear(unitTransformIndex)
        unitTransformModel.append(PCCUnits.unitTransforms(idUnit))
    }
}
