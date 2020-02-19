
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Item {
    property bool firstDelegate : false
    property int typeId
    anchors.left: parent.left
    anchors.right: parent.right
    height: firstDelegate ? typeRectangle.height : 2 * typeRectangle.height

    Rectangle {
        id: additionalTypeSectionSpace
        height: firstDelegate ? 0 : typeRectangle.height

        anchors.top: parent.top
    }

    Rectangle {
        id: typeRectangle

        anchors.top: additionalTypeSectionSpace.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: unitsView.rightMargin

        height: typeNameMetrics.height + 2 * common.sectionTitleMargin

        color: common.sectionTitleBackground

        TextMetrics {
            id: typeNameMetrics
            font: typeName.font
            text: typeName.text
        }   // TextMetrics, id: typeNameMetrics

        Text {
            id: typeName

            anchors.margins: common.sectionTitleMargin
            anchors.fill: parent

            renderType: Text.NativeRendering
            color: common.sectionTitleColor

            font.bold: common.sectionTitleBold
            font.pointSize: common.sectionTitlePointSize

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            text: section
        }   // Text, typeName

        Image {
            id: unitAddButton
            fillMode: Image.PreserveAspectFit
            anchors.margins: common.sectionTitleMargin
            anchors.top: parent.top
            x: parent.width - 3 * width - 3 * common.sectionTitleMargin
            source: "pics/Add.svg"

            verticalAlignment: Image.AlignVCenter
            horizontalAlignment: Image.AlignRight

            width: common.smallButtonSide
            height: common.smallButtonSide

            Dialog {
                visible: false
                id: unitAddDialog
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

            MouseArea {
                anchors.fill: parent
                onClicked: { unitAddDialog.open()
                }
            }
        }   // Image, unitAddButton
    } // Rectangle
}   // delegate, id: typeSection
