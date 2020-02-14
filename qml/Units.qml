//  Copyright (C) 2015-2020 Virgil Security, Inc.
//
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//      (1) Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//      (2) Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in
//      the documentation and/or other materials provided with the
//      distribution.
//
//      (3) Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
//  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
//  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
//  Lead Maintainer: Virgil Security Inc. <support@virgilsecurity.com>


import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Rectangle {
    anchors.fill: parent

/*
    ListModel {
        id: someDataModel

        ListElement       { field: "a"; value : 10}
        ListElement       { field: "ad"; value : 230}

        Component.onCompleted: {
            append ({"field" : "f", "value": 2})
            append ({"field" : "dd", "value": 55})
        }
    }

    ListView {
        model: someDataModel
                anchors.fill: parent
        delegate: Text { text: field + " : " + value }
    }
}
*/

    ListView {
        id: view
        z: 1

        anchors.margins: 10
        anchors.fill: parent
        spacing: 10
        model: PCCUnits

        section {
            property: "type"
            delegate: Item {
                id: sectionHeader
                width: view.width
                height: 50

                Text {
                    id: sectionName
                    anchors.left: parent.left
                    renderType: Text.NativeRendering
                    color: "blue"
                    text: section
                }
//
//              Image {
//                  id: sectionCollapseButton
//                  anchors.right: sectionAddButton.left
//                  anchors.margins: common.dp(5)
//                  source: "pics/Collapse.svg"
//                  sourceSize.width: common.smallButtonSide
//                  sourceSize.height: common.smallButtonSide
//                  MouseArea {
//                      anchors.fill: parent
//                      onClicked: {
//                          listDelegate.height = listDelegate.visible ? 0 : 40
//                          listDelegate.visible = !listDelegate.visible
//                          console.log("=== CLICK!!! ===")
//                      }
//                  }
//              }
//
                Image {
                    id: sectionAddButton
                    anchors.right: parent.right
                    anchors.margins: common.dp(5)
                    source: "pics/Add.svg"
                    sourceSize.width: common.smallButtonSide
                    sourceSize.height: common.smallButtonSide
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { }
                    }
                }
            }
        }

        delegate: Item {
            id: listDelegate

            width: view.width
            height: 50

                Text {
                    id: unitTitle
                    anchors.left: parent.left
                    renderType: Text.NativeRendering
                    text: title + " (" + abbreviation + ")" + (isDefault ? qsTr(", по умолчанию") : "")
                }

                Image {
                    id: delegateDeleteButton
                    anchors.right: parent.right
                    anchors.margins: common.dp(5)
                    source: "pics/Delete.svg"
                    sourceSize.width: common.smallButtonSide
                    sourceSize.height: common.smallButtonSide
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {}
                    }
                }

                Image {
                    id: delegateEditButton
                    anchors.right: delegateDeleteButton.left
                    anchors.margins: common.dp(5)
                    source: "pics/Edit.svg"
                    sourceSize.width: common.smallButtonSide
                    sourceSize.height: common.smallButtonSide
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {}
                    }
                }

                ListModel {
                    id: unitTransformModel
                    Component.onCompleted: {
                        var amount = PCCUnits.unitTransformsAmount(dbId)
                        for (var step = 0; step < amount; ++step) {
                            append(PCCUnits.unitTransform(dbId, step))
                        }
                    }
                }
                ListView {
                    id: unitTransformView
                    model: unitTransformModel
                    anchors.top: unitTitle.bottom
                    anchors.left: parent.left
                    anchors.right: parent.righ
                    height: 50

                    delegate: Text {
                        text: thisValue + abbreviation + " ==> " + toValue + toUnitAbbreviation
                    }
                }

        }
    }

}
