
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Rectangle {
    id: unitTransform
    property string description

    anchors.left: parent.left
    anchors.right: parent.right

    height: unitTransformTextMetrics.height + 2 * common.unitTransformMargin

    color: common.unitTransformBackground

    Component.onCompleted: {
        description = qsTr("%1 %2 = %3 %4").arg(thisValue).arg(abbreviation).arg(toValue).arg(toUnitAbbreviation)
    }

    TextMetrics {
        id: unitTransformTextMetrics
        font: unitTransformText.font
        text: unitTransformText.text
    }   // TextMetrics, id: unitTransformTextMetrics

    Text {
        id: unitTransformText

        anchors.margins: common.unitTransformMargin
        anchors.fill: parent

        renderType: Text.NativeRendering
        color: common.unitTransformColor

        font.bold: common.unitTransformBold
        font.pointSize: common.unitTransformPointSize

        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter

        text: description
    }   // Text, id: unitTransformText

    Image {
        id: unitTransformEditButton
        fillMode: Image.PreserveAspectFit
        anchors.margins: common.sectionTitleMargin
        anchors.top: parent.top
        anchors.right: unitTransformDeleteButton.left
        anchors.bottom: parent.bottom
        source: "pics/Edit.svg"

        width: common.smallButtonSide
        height: common.smallButtonSide

        MouseArea {
            anchors.fill: parent
            onClicked: { }
        }
    }   // Image, unitTransformEditButton

    Image {
        id: unitTransformDeleteButton
        fillMode: Image.PreserveAspectFit
        anchors.margins: common.sectionTitleMargin
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        source: "pics/Delete.svg"

        width: common.smallButtonSide
        height: common.smallButtonSide

        MessageDialog {
            id: unitTransformDeleteDlg
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

        MouseArea {
            anchors.fill: parent
            onClicked: {
                unitTransformDeleteDlg.open()
            }
        }
    }   // Image, unitTransformDeleteButton
}   // delegate, id: unitTransform
