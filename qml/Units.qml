
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3

Rectangle {
    anchors.fill: parent

    ListView {
        id: unitsView
        property int rightMargin: unitsViewScrollBar.width

        anchors.margins: common.listMargin
        anchors.fill: parent

        model: PCCUnits

        ScrollBar.vertical: ScrollBar {
            id: unitsViewScrollBar
            active: true
        }

// ********************
// *** Type section ***
// ********************

        section {
            property: "type"

            delegate: Item {
                id: typeSection
                property bool firstDelegate : false
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
                        id: typeAddButton
                        fillMode: Image.PreserveAspectFit
                        anchors.margins: common.sectionTitleMargin
                        anchors.top: parent.top
                        x: parent.width - 3 * width - 3 * common.sectionTitleMargin
                        source: "pics/Add.svg"

                        verticalAlignment: Image.AlignVCenter
                        horizontalAlignment: Image.AlignRight

                        width: common.smallButtonSide
                        height: common.smallButtonSide

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                            }
                        }
                    }   // Image, typeAddButton
                } // Rectangle
            }   // delegate, id: typeSection
        }   // section

// ********************
// *** Type element ***
// ********************

        delegate: Item {
            id: unitDelegate

            anchors.left: parent.left
            anchors.right: parent.right

//            anchors.leftMargin: common.indent
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

                    text: title + " (" + abbreviation + ")" + (isDefault ? qsTr(", по умолчанию") : "")
                } // Text, id: unitTitle

                Image {
                    id: unitAddButton
                    fillMode: Image.PreserveAspectFit
                    anchors.margins: common.sectionTitleMargin
                    anchors.top: parent.top
                    anchors.right: unitEditButton.left
                    source: "pics/Add.svg"

                    verticalAlignment: Image.AlignVCenter
                    horizontalAlignment: Image.AlignRight

                    width: common.smallButtonSide
                    height: common.smallButtonSide

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {

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

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                        }
                    }
                }   // Image, unitDeleteButton
            }

// ***********************
// *** Type transforms ***
// ***********************

            ListModel {
                id: unitTransformModel
                Component.onCompleted: {
                    append(PCCUnits.unitTransforms(idUnit))
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

                delegate: Rectangle {
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

            }   // ListView, id: unitTransformView
        }   // delegate, id: unitDelegate
    }   // ListView, id: unitsView
}
