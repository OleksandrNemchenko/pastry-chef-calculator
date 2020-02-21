
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Item {
    anchors.left: parent.left
    anchors.right: parent.right

   // anchors.leftMargin: common.indent
    anchors.rightMargin: unitsView.rightMargin

    height: unitTitleRect.height + unitTransformView.height

    Rectangle {
        id: unitTitleRect

        anchors.left: parent.left
        anchors.right: parent.right

        height: unitTitleMetrics.height + 2 * common.unitMargin

        color: common.unitBackground

        TextMetrics {
            id: unitTitleMetrics
            font: unitTitle.font
            text: unitTitle.text
        }   // TextMetrics, id: unitTitleMetrics

        Text {
            id: unitTitle

            anchors.margins: common.sectionTitleMargin
            anchors.fill: parent

            renderType: Text.NativeRendering
            color: common.unitColor

            font.bold: common.unitBold
            font.pointSize: common.unitPointSize

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop

            text: qsTr("%1 (%2)"). arg(title). arg(abbreviation)
        } // Text, id: unitTitle

        Image {
            id: unitTransformAddButton
            fillMode: Image.PreserveAspectFit
            anchors.margins: common.sectionTitleMargin
            anchors.top: parent.top
            anchors.right: unitEditButton.left
            source: "pics/Add.svg"

            verticalAlignment: Image.AlignVCenter
            horizontalAlignment: Image.AlignRight

            width: common.smallButtonSide
            height: common.smallButtonSide

            AddUnitTransform {
                id: unitTransformAddDialog
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    unitTransformAddDialog.open()
                }
            }
        }   // Image, typeAddButton

        Image {
            id: unitEditButton
            fillMode: Image.PreserveAspectFit
            anchors.margins: common.sectionTitleMargin
            anchors.top: parent.top
            anchors.right: unitDeleteButton.left
            anchors.bottom: parent.bottom
            source: "pics/Edit.svg"

            width: common.smallButtonSide
            height: common.smallButtonSide

            MouseArea {
                anchors.fill: parent
                onClicked: { }
            }
        }   // Image, unitEditButton

        Image {
            id: unitDeleteButton
            fillMode: Image.PreserveAspectFit
            anchors.margins: common.sectionTitleMargin
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            source: "pics/Delete.svg"

            width: common.smallButtonSide
            height: common.smallButtonSide

            DeleteUnit {
                id: unitDeleteDlg
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    unitDeleteDlg.open()
                }
            }

            Component.onCompleted: {
                visible = !isSingle
            }
        }   // Image, unitDeleteButton
    }

// ***********************
// *** Type transforms ***
// ***********************

    ListModel {
        id: unitTransformModel

        function update(idUnit) {
            clear()
            append(PCCUnits.unitTransforms(idUnit))
        }
        Component.onCompleted: {
            update(idUnit)
        }
    }   // ListModel, id: unitTransformModel

    ListView {
        id: unitTransformView

        model: unitTransformModel

        interactive: false
        anchors.top: unitTitleRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: common.indent

        height: unitTransformModel.count * (common.unitTransformHeight + 2 * common.unitTransformMargin)

        delegate: UnitTransforms {
            id: unitTransform
        }   // delegate, id: unitTransform
    }   // ListView, id: unitTransformView
}   // delegate, id: unitDelegate
